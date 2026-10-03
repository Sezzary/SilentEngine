#pragma once

namespace Silent::Services
{
    constexpr char ASSETS_DIR_NAME[]             = "Assets";
    constexpr char ASSETS_AUDIO_DIR_NAME[]       = "Audio";
    constexpr char ASSETS_FONTS_DIR_NAME[]       = "Fonts";
    constexpr char ASSETS_LOCALES_DIR_NAME[]     = "Locales";
    constexpr char ASSETS_SHADERS_DIR_NAME[]     = "Shaders";
    constexpr char ASSETS_STREAM_DIR_NAME[]      = "Stream";
    constexpr char ASSETS_VIDEO_DIR_NAME[]       = "Video";
    constexpr char SAVEGAME_DIR_NAME[]           = "Savegame";
    constexpr char SAVEGAME_SLOT_DIR_NAME_BASE[] = "Slot ";
    constexpr char SCREENSHOTS_DIR_NAME[]        = "Screenshots";

    constexpr char ASSETS_LOCALE_FILENAME[]          = "Locale";
    constexpr char ASSETS_LOCALE_METADATA_FILENAME[] = "Metadata";
    constexpr char OPTIONS_FILENAME[]                = "Options";
    constexpr char SCREENSHOT_FILENAME_BASE[]        = "Screenshot_";

    constexpr char BAK_FILE_EXT[]         = ".bak";
    constexpr char JSON_FILE_EXT[]        = ".json";
    constexpr char PNG_FILE_EXT[]         = ".png";
    constexpr char SAVEGAME_FILE_EXT[]    = ".save";
    constexpr char SPV_SHADER_FILE_EXT[]  = ".spv"; // @todo Use these in `Pipeline.cpp`.
    constexpr char MSL_SHADER_FILE_EXT[]  = ".msl";
    constexpr char DXIL_SHADER_FILE_EXT[] = ".dxil";
    
    constexpr int JSON_INDENT_SIZE = 4;

    /** @brief Filesystem manager. */
    class FilesystemManager
    {
    private:
        // =======
        // Fields
        // =======

        stdfs::path _appDir         = {};
        stdfs::path _assetsDir      = {};
        stdfs::path _workDir        = {};
        stdfs::path _savegameDir    = {};
        stdfs::path _screenshotsDir = {};

    public:
        // =============
        // Constructors
        // =============

        FilesystemManager() = default;

        // ========
        // Getters
        // ========

        const stdfs::path& GetAppDirectory() const;
        const stdfs::path& GetAssetsDirectory() const;
        const stdfs::path& GetWorkDirectory() const;
        const stdfs::path& GetSavegameDirectory() const;
        const stdfs::path& GetScreenshotsDirectory() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the filesystem. */
        void Initialize();
    };
}
