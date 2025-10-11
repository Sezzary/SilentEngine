#pragma once

#include "Engine/Assets/Parsers/Tim.h"
#include "Engine/Assets/Parsers/Tmd.h"

namespace Silent::Assets
{
    /** @brief Loaded asset types. Used in `Asset`. */
    enum class AssetType
    {
        Tim, /** "Texture IMage"                | PsyQ SDK texture data. */
        Vab, /** "Voice Audio Bank"             | PsyQ SDK audio container data. */
        Bin, /** "BINary"                       | Original compiled logic overlay data. */
        Dms, /** "Demo Motion Sequence"?        | Cutscene keyframe data. */
        Anm, /** "ANiMation"                    | Animation data. */
        Plm, /** "Polygon List Model"?          | Global static model data. */
        Ipd, /** "Instance Polygon Data"?       | Local static model data. */
        Ilm, /** "Indexed List Model"?          | Linked model data. */
        Tmd, /** "Three-dimensional Model Data" | PsyQ SDK 3D model data. Only used for inventory items. */
        Dat, /** "Demo dATa"?                   | Demo playback data. */
        Kdt, /** "Key Data Tracker"?            | Konami MIDI tracker data. */
        Cmp, /** "CoMPressed" or "CoMPiled"?    | Unknown. */
        Xa,  /** "eXtended Audio"               | PSX XA audio stream. */

        Count
    };

    /** @brief Loaded asset states. Used in `Asset`. */
    enum class AssetState
    {
        Unloaded,
        Loading,
        Loaded,
        Error
    };

    /** @brief Loaded asset data and metadata. */
    struct Asset
    {
        std::string             Name = {};                    /** Filename relative to assets folder. */
        AssetType               Type = AssetType::Tim;        /** File type. */
        std::filesystem::path   File = {};                    /** Absolute system file path. */
        uint64                  Size = 0;                     /** Raw file size in bytes. */

        std::atomic<AssetState> State = AssetState::Unloaded; /** Thread-safe load state. */
        std::shared_ptr<void>   Data  = nullptr;              /** Parsed data. */
    };

    /** @brief Central manager for asset streaming. */
    class AssetManager
    {
    private:
        // =======
        // Fields
        // =======

        std::vector<std::shared_ptr<Asset>>  _assets       = {}; /** Registered assets. */
        std::unordered_map<std::string, int> _assetIdxs    = {}; /** Key = asset name, value = asset index. */
        std::atomic<uint>                    _loadingCount = 0;  /** Number of currently loading assets. */

    public:
        // =============
        // Constructors
        // =============

        AssetManager() = default;

        // ========
        // Getters
        // ========

        /** @brief Gets a loaded asset via a file index.
         *
         * @param assetIdx Asset file index.
         * @return Pointer to an `Asset` object if the asset is loaded, `nullptr` otherwise.
         */
        const std::shared_ptr<Asset> GetAsset(int assetIdx) const;

        /** @brief Gets a loaded asset via a filename.
         *
         * @param assetName Asset filename.
         * @return Pointer to an `Asset` object if the asset is loaded, `nullptr` otherwise.
         */
        const std::shared_ptr<Asset> GetAsset(const std::string& assetName) const;

        /** @brief Gets a vector containing the names of all loaded assets.
         *
         * @return Vector of all loaded asset names.
         */
        std::vector<std::string> GetLoadedAssetNames() const;

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if assets are currently being loaded.
         *
         * @return Busy status.
         */
        bool IsBusy() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the asset manager.
         *
         * @param assetsPath Assets folder path on the system.
         */
        void Initialize(const std::filesystem::path& assetsPath);

        /** @brief Loads an asset by index.
         *
         * @param assetIdx Index of the asset to load.
         * @return `std::future` of the asset's load status.
         */
        std::future<void> LoadAsset(int assetIdx);

        /** @brief Loads an asset by name.
         *
         * @param assetName Name of the asset to load.
         * @return `std::future` of the asset's load status.
         */
        std::future<void> LoadAsset(const std::string& assetName);

        /** @brief Unloads an asset by index.
         *
         * @param assetIdx Index of the asset to unload.
         */
        void UnloadAsset(int assetIdx);

        /** @brief Unloads an asset by name.
         *
         * @param assetName Name of the asset to unload.
         */
        void UnloadAsset(const std::string& assetName);

        /** @brief Unloads all currently loaded assets. */
        void UnloadAllAssets();
    };

    /** @brief Gets the typed data of a loaded asset.
     *
     * @tparam T Loaded asset type to cast the asset data to.
     * @param data Data to retrieve.
     * @return Typed loaded asset data.
     * @throws `std::runtime_error` if `data` is `nullptr`.
     */
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
