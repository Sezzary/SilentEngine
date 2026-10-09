#pragma once

#include "Assets/Loaders/Anm.h"
#include "Assets/Loaders/Dat.h"
#include "Assets/Loaders/Dms.h"
#include "Assets/Loaders/Ilm.h"
#include "Assets/Loaders/Ipd.h"
#include "Assets/Loaders/Plm.h"
#include "Assets/Loaders/Png.h"
#include "Assets/Loaders/Tim.h"
#include "Assets/Loaders/Tmd.h"

namespace Silent::Assets
{
    /** @brief Streamable asset types. Used in `Asset`. */
    enum class AssetType
    {
        Tim, /** "Texture IMage"                 | PsyQ SDK texture image. */
        Vab, /** "Voice Audio Bank"              | PsyQ SDK audio bank. */
        Bin, /** "BINary"                        | Original logic overlay data. @unused PSX-specific. */
        Dms, /** "Demo Motion Sequence"?         | Cutscene data. */
        Anm, /** "ANiMation"                     | ILM animation data. */
        Plm, /** "Prop Linear Mesh"?             | Global map prop model data. Identical to ILM. */
        Ipd, /** "Instanced Polygon model Data"? | Map model and collision data. */
        Ilm, /** "Interactive Linear Mesh"?      | Skeletal character model data. Identical to PLM. */
        Tmd, /** "3D Model Data"                 | PsyQ SDK 3D model data.. */
        Dat, /** "Demo dATa"                     | Demo state or playback data. */
        Kdt, /** "Key Data Tracker"              | Konami MIDI tracker data. */
        Cmp, /** "CoMPressed"                    | Compressed data. @unused PSX-specific. */
        Xa,  /** "eXtended Audio"                | PSX ADPCM audio stream. @unused PSX-specific. */
        Str, /** "video STReam"                  | PSX video stream. @unused PSX-specific. */
        Png
    };

    /** @brief Streamable asset states. Used in `Asset`. */
    enum class AssetState
    {
        Unloaded,
        Loading,
        Loaded,
        Error
    };

    /** @brief Streamable asset. */
    struct Asset
    {
        std::string             Name  = {};                   /** Filename relative to assets folder. */
        int                     Idx   = 0;                    /** Registration index. */
        AssetType               Type  = AssetType::Tim;       /** File type. */
        stdfs::path             File  = {};                   /** Absolute system file path. */
        uint64                  Size  = 0;                    /** Raw file size in bytes. */
        std::atomic<AssetState> State = AssetState::Unloaded; /** Thread-safe load state. */
        std::shared_ptr<void>   Data  = nullptr;              /** Thread-safe parsed data. */

        /** @brief Gets the typed asset data. The asset must be loaded before calling.
         *
         * @tparam T Loaded asset type to cast the asset data to.
         * @return Typed loaded asset data.
         * @throws `std::runtime_error` if `Data` is `nullptr`.
         */
        template <typename T>
        std::shared_ptr<const T> GetData() const
        {
            if (Data == nullptr)
            {
                throw std::runtime_error(Fmt("Attempted to get data for unloaded asset `{}`.", Name));
            }

            return std::reinterpret_pointer_cast<T>(Data);
        }
    };

    /** @brief Asset streamer. */
    class AssetStreamer
    {
    private:
        // =======
        // Fields
        // =======

        std::unordered_map<std::string, std::unique_ptr<Asset>> _assets       = {}; /** Key = asset name, value = asset. */
        std::atomic<int>                                        _loadingCount = 0;  /** Number of currently loading assets. */
        std::unordered_map<int, std::string>                    _names        = {}; /** Key = asset index, value = asset name. */
        
        std::unordered_map<std::string, std::future<void>> _loadFutures  = {}; /** Key = asset name, value = load future. */
        std::mutex                                         _loadMutex    = {};
        std::mutex                                         _unloadMutex  = {};

    public:
        // =============
        // Constructors
        // =============

        AssetStreamer() = default;

        // ========
        // Getters
        // ========

        /** @brief Gets the name of an asset by index.
         *
         * @param idx Asset index.
         * @return Asset name.
         */
        const std::string& GetName(int idx) const;

        /** @brief Gets the index of an asset by name.
         *
         * @param name Asset name.
         * @return Asset index.
         */
        int GetIdx(const std::string& name) const;

        /** @brief Gets a vector containing the names of all loaded assets.
         *
         * @return Vector of all loaded asset names.
         */
        std::vector<std::string> GetLoadedNames() const;

        /** @brief Gets the number of assets currently being loaded.
         *
         * @return Number of assets being loaded.
         */
        int GetLoadingCount() const;

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
        void Initialize(const stdfs::path& assetsPath);

        /** @brief Loads an asset by name.
         *
         * @param name Name of the asset to load.
         * @return `std::future` of the asset's load status.
         */
        const std::future<void>& Load(const std::string& name);

        /** @brief Loads an asset by index.
         *
         * @todo Only for temporary compatibility with original file table.
         *
         * @param assetIdx Index of the asset to load.
         * @return `std::future` of the asset's load status.
         */
        const std::future<void>& Load(int idx);

        /** @brief Unloads an asset by name.
         *
         * @param name Name of the asset to unload.
         */
        void Unload(const std::string& name);

        /** @brief Unloads all currently loaded assets. */
        void UnloadAll();

        // ==========
        // Operators
        // ==========

        const Asset* operator[](const std::string& name);
    };
}
