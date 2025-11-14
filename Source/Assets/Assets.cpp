#include "Framework.h"
#include "Assets/Assets.h"

#include "Application.h"
#include "Assets/Parsers/Tim.h"
#include "Assets/Parsers/Tmd.h"
#include "Utils/Parallel.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Assets
{
    static const auto ASSET_TYPES = std::unordered_map<std::string, AssetType>
    {
        { ".TIM", AssetType::Tim },
        { ".VAB", AssetType::Vab },
        { ".BIN", AssetType::Bin },
        { ".DMS", AssetType::Dms },
        { ".ANM", AssetType::Anm },
        { ".PLM", AssetType::Plm },
        { ".IPD", AssetType::Ipd },
        { ".ILM", AssetType::Ilm },
        { ".TMD", AssetType::Tmd },
        { ".DAT", AssetType::Dat },
        { ".KDT", AssetType::Kdt },
        { ".CMP", AssetType::Cmp },
        { "",     AssetType::Xa  } // @todo Should be given .XA extension when exporting from ROM.
    };

    static const auto PARSER_FUNCS = std::unordered_map<AssetType, std::function<std::shared_ptr<void>(const std::filesystem::path& file)>>
    {
        { AssetType::Tim, ParseTim },
        { AssetType::Tmd, ParseTmd }
    };

    const std::string& AssetManager::GetAssetName(int assetIdx) const
    {
        // Get asset.
        if (assetIdx < 0 || assetIdx >= _assets.size())
        {
            Debug::Log(fmt::format("Attempted to get name of invalid asset {}.", assetIdx), Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return EMPTY_STRING;
        }
        const auto asset = _assets[assetIdx];

        return asset->Name;
    }

    std::vector<std::string> AssetManager::GetLoadedAssetNames() const
    {
        // Run through registered assets.
        auto names = std::vector<std::string>{};
        for (const auto asset : _assets)
        {
            if (asset->State == AssetState::Loaded)
            {
                names.push_back(asset->Name);
            }
        }

        return names;
    }

    const std::shared_ptr<Asset> AssetManager::GetAsset(int assetIdx)
    {
        // Get asset.
        if (assetIdx < 0 || assetIdx >= _assets.size())
        {
            Debug::Log(fmt::format("Attempted to get invalid asset {}.", assetIdx), Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return nullptr;
        }
        const auto asset = _assets[assetIdx];

        // Load if not preloaded.
        if (asset->State != AssetState::Loaded)
        {
            Debug::Log(fmt::format("Getting non-preloaded asset `{}`. Loading in place.", GetAssetName(assetIdx)),
                       Debug::LogLevel::Warning, Debug::LogMode::Debug);

            LoadAsset(assetIdx).wait();
        }

        // Check if loading failed.
        if (asset->State == AssetState::Error)
        {
            Debug::Log(fmt::format("Failed to get asset `{}`.", GetAssetName(assetIdx)), Debug::LogLevel::Error, Debug::LogMode::Debug);
            return nullptr;
        }
        return asset;
    }

    const std::shared_ptr<Asset> AssetManager::GetAsset(const std::string& assetName)
    {
        // Check if asset exists.
        auto it = _idxs.find(assetName);
        if (it == _idxs.end())
        {
            Debug::Log(fmt::format("Attempted to get invalid asset `{}`.", assetName), Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return nullptr;
        }

        // Get asset by index.
        const auto& [keyName, assetIdx] = *it;
        return GetAsset(assetIdx);
    }

    bool AssetManager::IsBusy() const
    {
        return _loadingCount > 0;
    }

    void AssetManager::Initialize(const std::filesystem::path& assetsPath)
    {
        // Collect files sorted alphabetically.
        auto files = std::vector<std::filesystem::path>{};
        for (auto& entry : std::filesystem::recursive_directory_iterator(assetsPath))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry.path());
            }
        }
        Sort(files);

        // Register assets.
        _assets.reserve(files.size());
        for (int i = 0; i < files.size(); i++)
        {
            const auto& file = files[i];

            // Check if type is known.
            auto ext = ToUpper(file.extension().string());
            if (ASSET_TYPES.find(ext) == ASSET_TYPES.end())
            {
                Debug::Log(fmt::format("Unknown asset type for file `{}`.", file.string()), Debug::LogLevel::Warning, Debug::LogMode::Debug);
                continue;
            }

            // @heapalloc Create asset entry.
            _assets.emplace_back(std::make_shared<Asset>());

            // Define asset entry.
            auto asset   = _assets.back();
            asset->Name  = std::filesystem::relative(file, assetsPath).string();
            asset->Type  = ASSET_TYPES.at(ext);
            asset->File  = file;
            asset->Size  = std::filesystem::file_size(file);
            asset->State = AssetState::Unloaded;
            asset->Data  = nullptr;

            // Add asset index and name to maps.
            _idxs[asset->Name] = i;
            _names[i]          = asset->Name;
        }

        // Create fallback ready future.
        _loadFutures[NO_VALUE] = GenerateReadyFuture();

        Debug::Log(fmt::format("Registered {} assets.", _assets.size()), Debug::LogLevel::Info, Debug::LogMode::Debug);
    }

    const std::future<void>& AssetManager::LoadAsset(int assetIdx)
    {
        auto& executor = g_App.GetExecutor();

        // Get asset.
        if (assetIdx < 0 || assetIdx >= _assets.size())
        {
            Debug::Log(fmt::format("Attempted to load invalid asset {}.", assetIdx), Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return _loadFutures[NO_VALUE];
        }
        auto& asset = _assets[assetIdx];

        // Check if loading or loaded.
        if (asset->State == AssetState::Loading || asset->State == AssetState::Loaded)
        {
            return _loadFutures[assetIdx];
        }

        // Check if file is valid.
        if (!std::filesystem::exists(asset->File))
        {
            Debug::Log(fmt::format("Attempted to load asset `{}` from invalid file `{}`.", asset->Name, asset->File.string()),
                       Debug::LogLevel::Error, Debug::LogMode::Debug);

            asset->State = AssetState::Error;
            return _loadFutures[assetIdx];
        }

        // Set loading state.
        asset->State = AssetState::Loading;
        _loadingCount++;

        // Load asynchronously.
        _loadFutures[assetIdx] = executor.AddTask([&]()
        {
            // Get parser function.
            auto parserFuncIt = PARSER_FUNCS.find(asset->Type);
            if (parserFuncIt == PARSER_FUNCS.end())
            {
                Debug::Log(fmt::format("Attempted to load asset `{}` with no parser function for asset type {}.", asset->Name, (int)asset->Type),
                           Debug::LogLevel::Error);

                asset->State = AssetState::Unloaded;
                _loadingCount--;
                return;
            }
            const auto& parserFunc = parserFuncIt->second;

            // Parse asset data from file.
            try
            {
                asset->Data  = parserFunc(asset->File);
                asset->State = AssetState::Loaded;

                Debug::Log(fmt::format("Loaded asset `{}`.", asset->Name), Debug::LogLevel::Info, Debug::LogMode::Debug);
            }
            catch (const std::exception& ex)
            {
                asset->Data  = nullptr;
                asset->State = AssetState::Error;

                Debug::Log(fmt::format("Failed to parse file for asset `{}`: {}", asset->Name, ex.what()), Debug::LogLevel::Error);
            }
            _loadingCount--;
        });

        return _loadFutures[assetIdx];
    }

    const std::future<void>& AssetManager::LoadAsset(const std::string& assetName)
    {
        // Check if asset exists.
        int* assetIdx = Find(_idxs, assetName);
        if (assetIdx == nullptr)
        {
            Debug::Log(fmt::format("Attempted to load unregistered asset `{}`.", assetName), Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return _loadFutures[NO_VALUE];
        }

        // Load asset by index.
        return LoadAsset(*assetIdx);
    }

    void AssetManager::UnloadAsset(int assetIdx)
    {
        // Get asset.
        if (assetIdx < 0 || assetIdx >= _assets.size())
        {
            Debug::Log(fmt::format("Attempted to unload invalid asset {}.", assetIdx), Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return;
        }
        auto& asset = _assets[assetIdx];

        // Check if already unloaded.
        if (asset->State == AssetState::Unloaded)
        {
            return;
        }

        // Unload.
        asset->State = AssetState::Unloaded;
        asset->Data  = nullptr;

        // Remove load future.
        _loadFutures.erase(assetIdx);

        Debug::Log(fmt::format("Unloaded asset `{}`.", GetAssetName(assetIdx)), Debug::LogLevel::Info, Debug::LogMode::Debug);
    }

    void AssetManager::UnloadAsset(const std::string& assetName)
    {
        // Check if asset exists.
        auto it = _idxs.find(assetName);
        if (it == _idxs.end())
        {
            Debug::Log(fmt::format("Attempted to unload unregistered asset `{}`.", assetName), Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return;
        }

        // Unload asset by index.
        const auto& [keyName, assetIdx] = *it;
        UnloadAsset(assetIdx);
    }

    void AssetManager::UnloadAllAssets()
    {
        // Run through registered assets.
        for (auto& asset : _assets)
        {
            if (asset->State == AssetState::Unloaded)
            {
                continue;
            }

            // Unload.
            asset->State = AssetState::Unloaded;
            asset->Data  = nullptr;

            // Remove load future.
            _loadFutures.erase(_idxs[asset->Name]);
        }
    }
}
