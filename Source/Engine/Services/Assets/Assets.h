#pragma once

namespace Silent::Assets
{
    enum class AssetType
    {
        Tim, // PsyQ SDK texture data.
        Vab, // PsyQ SDK audio container data.
        Bin, // Original compiled logic overlay data.
        Dms, // Cutscene keyframe data.
        Anm, // Animation data.
        Plm, // Global static model data.
        Ipd, // Local static model data.
        Ilm, // Skeletal model data.
        Tmd, // PsyQ SDK 3D model data. Used exclusively for inventory.
        Dat, // Demo playback data.
        Kdt, // Konami MIDI tracker data.
        Cmp, // Unknown. "Compressed"?
        Xa,  // Audio data.

        Count
    };

    enum class AssetState
    {
        Unloaded,
        Loading,
        Loaded,
        Error
    };

    struct Asset
    {
        std::string           Name = {};             // Filename relative to assets folder.
        AssetType             Type = AssetType::Tim; // File type.
        std::filesystem::path File = {};             // Absolute file path.
        uint64                Size = 0;              // Raw file size in bytes.

        std::atomic<AssetState> State = AssetState::Unloaded; // Thread-safe load state.
        std::shared_ptr<void>   Data  = nullptr;              // Parsed data.
    };

    class AssetManager
    {
    private:
        // Fields

        std::vector<std::shared_ptr<Asset>>  _assets       = {};
        std::unordered_map<std::string, int> _assetIdxs    = {}; // Key = asset name, value = asset index.
        std::atomic<uint>                    _loadingCount = 0;

    public:
        // Constructors

        AssetManager() = default;

        // Getters

        const std::shared_ptr<Asset> GetAsset(int assetIdx) const;
        const std::shared_ptr<Asset> GetAsset(const std::string& assetName) const;
        std::vector<std::string>     GetLoadedAssetNames() const;

        // Inquirers

        bool IsBusy() const;

        // Utilities

        void              Initialize(const std::filesystem::path& assetsPath);
        std::future<void> LoadAsset(int assetIdx);
        std::future<void> LoadAsset(const std::string& assetName);
        void              UnloadAsset(int assetIdx);
        void              UnloadAsset(const std::string& assetName);
        void              UnloadAllAssets();
    };

    template <typename T>
    std::shared_ptr<T> GetAssetData(std::shared_ptr<void> data)
    {
        if (data == nullptr)
        {
            throw std::runtime_error("Attempted to get data for unloaded asset.");
        }

        return std::reinterpret_pointer_cast<T>(data);
    }
}
