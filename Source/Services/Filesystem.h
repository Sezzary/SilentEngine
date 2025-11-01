#pragma once

namespace Silent::Services
{
    constexpr char JSON_FILE_EXT[]     = ".json";
    constexpr char PNG_FILE_EXT[]      = ".png";
    constexpr char SAVEGAME_FILE_EXT[] = ".savegame";
    
    constexpr uint JSON_INDENT_SIZE = 4;

    /** @brief Filesystem manager. */
    class FilesystemManager
    {
    private:
        // =======
        // Fields
        // =======

        std::filesystem::path _appDir         = {}; /** Application folder. */
        std::filesystem::path _assetsDir      = {}; /** Game assets folder. */
        std::filesystem::path _workDir        = {}; /** Workspace folder. */
        std::filesystem::path _savegameDir    = {}; /** Savegame folder. */
        std::filesystem::path _screenshotsDir = {}; /** Screenshots folder. */
        std::filesystem::path _shadersDir     = {}; /** Shaders folder. */

    public:
        // =============
        // Constructors
        // =============

        FilesystemManager() = default;
        
        // ========
        // Getters
        // ========

        const std::filesystem::path& GetAppDirectory() const;
        const std::filesystem::path& GetAssetsDirectory() const;
        const std::filesystem::path& GetWorkDirectory() const;
        const std::filesystem::path& GetSavegameDirectory() const;
        const std::filesystem::path& GetScreenshotsDirectory() const;
        const std::filesystem::path& GetShadersDirectory() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the filesystem. */
        void Initialize();
    };
}
