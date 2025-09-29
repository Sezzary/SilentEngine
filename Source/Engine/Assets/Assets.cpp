#include "Framework.h"
#include "Engine/Assets/Assets.h"

#include "Engine/Assets/Parsers/Tmd.h"
#include "Engine/Assets/Parsers/Tim.h"
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
        { "",     AssetType::Xa }   // @todo Should be given .XA extension when exporting from ROM.
    };

    static const auto PARSER_FUNCS = std::unordered_map<AssetType, std::function<std::shared_ptr<void>(const std::filesystem::path& file)>>
    {
        { AssetType::Tim, ParseTim },
        { AssetType::Tmd, ParseTmd }
    };

    const std::shared_ptr<Asset> AssetManager::GetAsset(int assetIdx) const
    {
        // Get asset.
        if (assetIdx < 0 || assetIdx >= _assets.size())
        {
            Log("Attempted to get invalid asset " + std::to_string(assetIdx) + ".", LogLevel::Warning, LogMode::Debug);
            return nullptr;
        }
        const auto asset = _assets[assetIdx];

        return asset;
    }

    const std::shared_ptr<Asset> AssetManager::GetAsset(const std::string& assetName) const
    {
        // Check if asset exists.
        auto it = _assetIdxs.find(assetName);
        if (it == _assetIdxs.end())
        {
            Log("Attempted to get unregistered asset '" + assetName + "'.", LogLevel::Warning, LogMode::Debug);
            return nullptr;
        }

        // Get asset by index.
        const auto& [keyName, assetIdx] = *it;
        return GetAsset(assetIdx);
    }

    std::vector<std::string> AssetManager::GetLoadedAssetNames() const
    {
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

    bool AssetManager::IsBusy() const
    {
        return _loadingCount > 0;
    }

    void AssetManager::Initialize(const std::filesystem::path& assetsPath)
    {
        // Collect files.
        auto files = std::vector<std::filesystem::path>{};
        for (auto& entry : std::filesystem::recursive_directory_iterator(assetsPath))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry.path());
            }
        }

        // Sort files alphabetically.
        std::sort(files.begin(), files.end());
        
        // Register assets.
        _assets.reserve(files.size());
        for (int i = 0; i < files.size(); i++)
        {
            const auto& file = files[i];

            // Check if type is known.
            auto ext = ToUpper(file.extension().string());
            if (ASSET_TYPES.find(ext) == ASSET_TYPES.end())
            {
                Log("Unknown asset type for file '" + file.string() + "'.", LogLevel::Warning, LogMode::Debug);
                continue;
            }

            // HEAP ALLOC: Create asset entry.
            _assets.emplace_back(std::make_shared<Asset>());

            // Define asset entry.
            auto asset   = _assets.back();
            asset->Name  = std::filesystem::relative(file, assetsPath).string();
            asset->Type  = ASSET_TYPES.at(ext);
            asset->File  = file;
            asset->Size  = std::filesystem::file_size(file);
            asset->State = AssetState::Unloaded;
            asset->Data  = nullptr;

            // Add to asset index map.
            _assetIdxs[asset->Name] = i;
        }

        Log("Registered " + std::to_string(_assets.size()) + " assets.", LogLevel::Info, LogMode::Debug);
    }

    std::future<void> AssetManager::LoadAsset(int assetIdx)
    {
        // Get asset.
        if (assetIdx < 0 || assetIdx >= _assets.size())
        {
            Log("Attempted to load invalid asset " + std::to_string(assetIdx) + ".", LogLevel::Warning, LogMode::Debug);
            return GenerateReadyFuture();
        }
        auto& asset = _assets[assetIdx];

        // Check if already loading or loaded.
        if (asset->State == AssetState::Loading || asset->State == AssetState::Loaded)
        {
            Log("Attempted to load already loaded asset `" + asset->Name + "`.", LogLevel::Warning, LogMode::Debug);
            return GenerateReadyFuture();
        }

        // Check if file is valid.
        if (!std::filesystem::exists(asset->File))
        {
            Log("Attempted to load asset " + std::to_string(assetIdx) + " from invalid file '" + asset->File.string() + "'.", LogLevel::Error);

            asset->State = AssetState::Error;
            return GenerateReadyFuture();
        }

        // Set loading state.
        asset->State = AssetState::Loading;
        _loadingCount++;

        // Load asynchronously.
        return g_Executor.AddTask([&]()
        {
            // Get parser function.
            auto parserFuncIt = PARSER_FUNCS.find(asset->Type);
            if (parserFuncIt == PARSER_FUNCS.end())
            {
                Log("Attempted to load asset " + std::to_string(assetIdx) + " with no parser function for asset type " + std::to_string((int)asset->Type) + ".",
                    LogLevel::Error);

                asset->State = AssetState::Error;
                _loadingCount--;
                return;
            }
            const auto& parserFunc = parserFuncIt->second;

            // Parse asset data from file.
            try
            {
                asset->Data  = parserFunc(asset->File);
                asset->State = AssetState::Loaded;

                Log("Loaded asset " + std::to_string(assetIdx) + ".", LogLevel::Info, LogMode::Debug);
            }
            catch (const std::exception& ex)
            {
                Log("Failed to parse file for asset " + std::to_string(assetIdx) + ": " + ex.what(), LogLevel::Error);

                asset->State = AssetState::Error;
            }
            _loadingCount--;
        });
    }

    std::future<void> AssetManager::LoadAsset(const std::string& assetName)
    {
        // Check if asset exists.
        auto it = _assetIdxs.find(assetName);
        if (it == _assetIdxs.end())
        {
            Log("Attempted to load unregistered asset '" + assetName + "'.", LogLevel::Warning, LogMode::Debug);
            return GenerateReadyFuture();
        }

        // Load asset by index.
        const auto& [keyName, assetIdx] = *it;
        return LoadAsset(assetIdx);
    }

    void AssetManager::UnloadAsset(int assetIdx)
    {
        // Get asset.
        if (assetIdx < 0 || assetIdx >= _assets.size())
        {
            Log("Attempted to unload invalid asset " + std::to_string(assetIdx) + ".", LogLevel::Warning, LogMode::Debug);
            return;
        }
        auto& asset = _assets[assetIdx];

        // Check if already unloaded.
        if (asset->State == AssetState::Unloaded)
        {
            Log("Attempted to unload already unloaded asset " + std::to_string(assetIdx) + ".", LogLevel::Warning, LogMode::Debug);
            return;
        }

        // Unload.
        asset->State = AssetState::Unloaded;
        asset->Data  = nullptr;

        Log("Unloaded asset " + std::to_string(assetIdx) + ".", LogLevel::Info, LogMode::Debug);
    }

    void AssetManager::UnloadAsset(const std::string& assetName)
    {
        // Check if asset exists.
        auto it = _assetIdxs.find(assetName);
        if (it == _assetIdxs.end())
        {
            Log("Attempted to unload unregistered asset '" + assetName + "'.", LogLevel::Warning, LogMode::Debug);
            return;
        }

        // Unload asset by index.
        const auto& [keyName, assetIdx] = *it;
        UnloadAsset(assetIdx);
    }

    void AssetManager::UnloadAllAssets()
    {
        for (auto& asset : _assets)
        {
            // @todo Interrupt if something is loading?

            asset->State = AssetState::Unloaded;
            asset->Data.reset();
        }

        Log("All assets unloaded.", LogLevel::Info, LogMode::Debug);
    }
}
