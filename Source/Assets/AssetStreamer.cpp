#include "Framework.h"
#include "Assets/AssetStreamer.h"

#include "Application.h"
#include "Assets/Loaders/Anm.h"
#include "Assets/Loaders/Dat.h"
#include "Assets/Loaders/Dms.h"
#include "Assets/Loaders/Ilm.h"
#include "Assets/Loaders/Ipd.h"
#include "Assets/Loaders/Plm.h"
#include "Assets/Loaders/Png.h"
#include "Assets/Loaders/Tim.h"
#include "Assets/Loaders/Tmd.h"
#include "Utils/Parallel.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Assets
{
    using ParseFunc           = std::function<std::shared_ptr<void>(const stdfs::path& file)>;
    using QueueGpuUploadFunc  = std::function<void(const Asset& asset)>;
    using QueueGpuReleaseFunc = std::function<void(const Asset& asset)>;

    /** @brief Streamable asset file loader. */
    struct AssetLoader
    {
        ParseFunc           Parse           = nullptr;
        QueueGpuUploadFunc  QueueGpuUpload  = nullptr;
        QueueGpuReleaseFunc QueueGpuRelease = nullptr;
    };

    static const auto ASSET_TYPES = std::unordered_map<std::string, AssetType>
    {
        { ".TIM", AssetType::Tim },
        { ".VAB", AssetType::Vab },
        { ".BIN", AssetType::Bin }, // @unused PSX-specific.
        { ".DMS", AssetType::Dms },
        { ".ANM", AssetType::Anm },
        { ".PLM", AssetType::Plm },
        { ".IPD", AssetType::Ipd },
        { ".ILM", AssetType::Ilm },
        { ".TMD", AssetType::Tmd },
        { ".DAT", AssetType::Dat },
        { ".KDT", AssetType::Kdt },
        { ".CMP", AssetType::Cmp }, // @unused PSX-specific.
        { ".XA",  AssetType::Xa  }, // @unused PSX-specific.
        { ".STR", AssetType::Str }, // @unused PSX-specific.
        { ".PNG", AssetType::Png }
    };

    static const auto ASSET_LOADERS = std::unordered_map<AssetType, AssetLoader>
    {
        { AssetType::Anm, { ParseAnm }                                        },
        { AssetType::Dat, { ParseDat }                                        },
        { AssetType::Dms, { ParseDms }                                        },
        { AssetType::Ilm, { ParseIlm, QueueIlmGpuUpload, QueueIlmGpuRelease } },
        { AssetType::Ipd, { ParseIpd, QueueIpdGpuUpload, QueueIpdGpuRelease } },
        { AssetType::Plm, { ParsePlm, QueuePlmGpuUpload, QueuePlmGpuRelease } },
        { AssetType::Png, { ParsePng, QueuePngGpuUpload, QueuePngGpuRelease } },
        { AssetType::Tim, { ParseTim, QueueTimGpuUpload, QueueTimGpuRelease } },
        { AssetType::Tmd, { ParseTmd, QueueTmdGpuUpload, QueueTmdGpuRelease } }
    };

    const std::string& AssetStreamer::GetName(int idx) const
    {
        const auto* name = Find(_names, idx);
        if (name == nullptr)
        {
            return EMPTY_STRING;
        }

        return *name;
    }

    int AssetStreamer::GetIdx(const std::string& name) const
    {
        const auto* assetPtr = Find(_assets, name);
        if (assetPtr == nullptr)
        {
            return NO_VALUE;
        }
        const auto& asset = *assetPtr;

        return asset->Idx;
    }

    std::vector<std::string> AssetStreamer::GetLoadedNames() const
    {
        // Run through registered assets.
        auto names = std::vector<std::string>{};
        for (const auto& [keyName, asset] : _assets)
        {
            if (asset->State == AssetState::Loaded)
            {
                names.push_back(asset->Name);
            }
        }

        return names;
    }

    int AssetStreamer::GetLoadingCount() const
    {
        return _loadingCount;
    }

    bool AssetStreamer::IsBusy() const
    {
        return _loadingCount > 0;
    }

    void AssetStreamer::Initialize(const stdfs::path& assetsPath)
    {
        // Collect files sorted alphabetically.
        auto files = std::vector<stdfs::path>{};
        for (auto& entry : stdfs::recursive_directory_iterator(assetsPath))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry.path().generic_string());
            }
        }
        Sort(files);

        // Register assets.
        _assets.reserve(files.size());
        for (int i = 0; i < files.size(); i++)
        {
            const auto& file = files[i];

            // Check if type is streamable.
            auto ext = ToUpper(file.extension().string());
            if (Find(ASSET_TYPES, ext) == nullptr)
            {
                //Debug::Log(Fmt("Attempted to register non-stremable asset for file `{}`.", file.string()),
                //           Debug::LogLevel::Warning, Debug::LogMode::Debug);
                continue;
            }

            // @heapalloc Create asset entry.
            auto name = stdfs::relative(file, assetsPath).generic_string();
            _assets[name] = std::make_unique<Asset>();

            // Define asset entry.
            auto& asset = *_assets[name];
            asset.Name  = name;
            asset.Idx   = i;
            asset.Type  = ASSET_TYPES.at(ext);
            asset.File  = file;
            asset.Size  = stdfs::file_size(file);
            asset.State = AssetState::Unloaded;
            asset.Data  = nullptr;

            // Track asset index/name association.
            _names[i] = asset.Name;
        }

        // Create fallback ready future.
        _loadFutures[""] = GenerateReadyFuture();

        Debug::Log(Fmt("Registered {} streamable assets.", _assets.size()),
                   Debug::LogLevel::Info, Debug::LogMode::Debug);
    }

    const std::future<void>& AssetStreamer::Load(const std::string& name)
    {
        auto& executor = g_App.GetExecutor();

        // @lock Restrict load process access.
        {
            auto lock = ParallelLock(_loadMutex);

            // Get asset.
            const auto* assetPtr = Find(_assets, name);
            if (assetPtr == nullptr)
            {
                Debug::Log(Fmt("Attempted to load missing streamable asset `{}`.", name),
                           Debug::LogLevel::Warning, Debug::LogMode::Debug);
                return _loadFutures[""];
            }
            const auto& asset = *assetPtr;

            // Check if loading or loaded.
            if (asset->State == AssetState::Loading ||
                asset->State == AssetState::Loaded)
            {
                return _loadFutures[asset->Name];
            }

            // Check if file is valid.
            if (!stdfs::exists(asset->File))
            {
                Debug::Log(Fmt("Attempted to load streamable asset `{}` from missing file `{}`.",
                               asset->Name, asset->File.string()),
                           Debug::LogLevel::Error, Debug::LogMode::Debug);

                asset->State = AssetState::Error;
                return _loadFutures[asset->Name];
            }

            // Set loading state.
            asset->State = AssetState::Loading;
            _loadingCount++;

            // Load asynchronously.
            _loadFutures[asset->Name] = executor.AddTask([&]()
            {
                // Get loader.
                const auto* loader = Find(ASSET_LOADERS, asset->Type);
                if (loader == nullptr)
                {
                    Debug::Log(Fmt("Attempted to load streamable asset `{}` with no loader for asset type {}.",
                                   asset->Name, (int)asset->Type),
                               Debug::LogLevel::Error);

                    asset->State = AssetState::Unloaded;
                    _loadingCount--;
                    return;
                }

                // Load asset data from file.
                try
                {
                    asset->State = AssetState::Loaded;

                    // Parse file.
                    if (loader->Parse != nullptr)
                    {
                        asset->Data = loader->Parse(asset->File);
                    }

                    // Queue GPU resource upload.
                    if (loader->QueueGpuUpload != nullptr)
                    {
                        loader->QueueGpuUpload(*asset);
                    }

                    Debug::Log(Fmt("Loaded streamable asset `{}`.", asset->Name),
                               Debug::LogLevel::Info, Debug::LogMode::Debug);
                }
                catch (const std::exception& ex)
                {
                    asset->State = AssetState::Error;
                    asset->Data  = nullptr;
    
                    Debug::Log(Fmt("Failed to parse file for streamable asset `{}`: {}", asset->Name, ex.what()),
                               Debug::LogLevel::Error);
                }
                _loadingCount--;
            });

            return _loadFutures[asset->Name];
        }
    }

    const std::future<void>& AssetStreamer::Load(int idx)
    {
        const auto* name = Find(_names, idx);
        if (name == nullptr)
        {
            Debug::Log(Fmt("Attempted to load missing streamable asset by index `{}`.", idx),
                       Debug::LogLevel::Error, Debug::LogMode::Debug);
            return _loadFutures[""];
        }

        return Load(*name);
    }

    void AssetStreamer::Unload(const std::string& name)
    {
        // @lock Restrict unload process access.
        {
            auto lock = ParallelLock(_unloadMutex);

            // Get asset.
            const auto* assetPtr = Find(_assets, name);
            if (assetPtr == nullptr)
            {
                Debug::Log(Fmt("Attempted to unload missing streamable asset `{}`.", name),
                           Debug::LogLevel::Warning, Debug::LogMode::Debug);
                return;
            }
            auto& asset = *assetPtr;

            // Check if already unloaded.
            if (asset->State == AssetState::Unloaded)
            {
                return;
            }

            // Queue GPU resource release.
            const auto* loader = Find(ASSET_LOADERS, asset->Type);
            if (loader != nullptr && loader->QueueGpuRelease != nullptr)
            {
                loader->QueueGpuRelease(*asset);
            }

            // Unload.
            asset->State = AssetState::Unloaded;
            asset->Data  = nullptr;

            // Remove load future.
            _loadFutures.erase(asset->Name);

            Debug::Log(Fmt("Unloaded streamable asset `{}`.", asset->Name),
                       Debug::LogLevel::Info, Debug::LogMode::Debug);
        }
    }

    void AssetStreamer::UnloadAll()
    {
        // Run through registered assets.
        for (auto& [keyName, asset] : _assets)
        {
            Unload(keyName);
        }
    }

    const Asset* AssetStreamer::operator[](const std::string& name)
    {
        // Get asset.
        const auto* assetPtr = Find(_assets, name);
        if (assetPtr == nullptr)
        {
            Debug::Log(Fmt("Attempted to get missing streamable asset `{}`.", name),
                       Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return nullptr;
        }
        const auto& asset = *assetPtr;

        // Load if not preloaded.
        if (asset->State != AssetState::Loaded)
        {
            //Debug::Log(Fmt("Getting non-preloaded streamable asset `{}`. Loading in place.", asset->Name),
            //           Debug::LogLevel::Warning, Debug::LogMode::Debug);

            Load(asset->Name).wait();
        }

        // Check if loading failed.
        if (asset->State == AssetState::Error)
        {
            Debug::Log(Fmt("Failed to get streamable asset `{}`.", asset->Name),
                       Debug::LogLevel::Error, Debug::LogMode::Debug);
            return nullptr;
        }
        return &*asset;
    }
}
