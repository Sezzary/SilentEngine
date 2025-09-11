#pragma once

namespace Silent::Services
{
    enum class OsType
    {
        Windows,
        MacOs,
        Linux
    };

    constexpr char APP_NAME[]    = "Silent Engine";
    constexpr char APP_VERSION[] = "0.1.0";

#if defined(_WIN32) || defined(_WIN64)
    constexpr auto OS_TYPE = OsType::Windows;
#elif defined(__APPLE__)
    constexpr auto OS_TYPE = OsType::MacOs;
#elif defined(__linux__)
    constexpr auto OS_TYPE = OsType::Linux;
#endif

    constexpr char ASSETS_FOLDER_NAME[]                  = "Assets";
    constexpr char SAVEGAME_SLOT_FILE_FOLDER_NAME_BASE[] = "File ";
    constexpr char SAVEGAME_FOLDER_NAME[]                = "Savegame";
    constexpr char SCREENSHOTS_FOLDER_NAME[]             = "Screenshots";
    constexpr char SHADERS_FOLDER_NAME[]                 = "Shaders";
    constexpr char SAVEGAME_SLOT_FOLDER_NAME_BASE[]      = "Slot ";

    constexpr char SCREENSHOT_FILENAME_BASE[] = "Screenshot_";
    constexpr char OPTIONS_FILENAME[]         = "Options";
    
    constexpr char JSON_FILE_EXT[]     = ".json";
    constexpr char PNG_FILE_EXT[]      = ".png";
    constexpr char SAVEGAME_FILE_EXT[] = ".savegame";
    
    constexpr uint JSON_INDENT_SIZE = 4;

    /** @brief Central filesystem manager. */
    class FilesystemManager
    {
    private:
        // Fields

        std::filesystem::path _appFolder         = {};
        std::filesystem::path _assetsFolder      = {};
        std::filesystem::path _workFolder        = {};
        std::filesystem::path _screenshotsFolder = {};

    public:
        // Constructors

        FilesystemManager() = default;

        // Getters

        const std::filesystem::path& GetAppFolder() const;
        const std::filesystem::path& GetAssetsFolder() const;
        const std::filesystem::path& GetWorkFolder() const;
        const std::filesystem::path& GetScreenshotsFolder() const;

        // Utilities

        void Initialize();
    };
}
