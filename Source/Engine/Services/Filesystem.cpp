#include "Framework.h"
#include "Engine/Services/Filesystem.h"

namespace Silent::Services
{
    const std::filesystem::path& FilesystemManager::GetAppFolder() const
    {
        return _appFolder;
    }

    const std::filesystem::path& FilesystemManager::GetAssetsFolder() const
    {
        return _assetsFolder;
    }

    const std::filesystem::path& FilesystemManager::GetWorkFolder() const
    {
        return _workFolder;
    }

    const std::filesystem::path& FilesystemManager::GetScreenshotsFolder() const
    {
        return _screenshotsFolder;
    }

    void FilesystemManager::Initialize()
    {
        char*  buffer = nullptr;
        size_t length = 0;

        // Set app path.
        _appFolder = std::filesystem::current_path();

        // Set assets path.
        _assetsFolder = _appFolder / ASSETS_FOLDER_NAME;

        // Set work and screenshots paths.
        switch (OS_TYPE)
        {
            case OsType::Windows:
            {
                // Set `APPDATA` path for work.
                if (_dupenv_s(&buffer, &length, "APPDATA") == 0 && buffer != nullptr)
                {
                    auto path   = std::filesystem::path(buffer);
                    _workFolder = path / APP_NAME; 
                }

                // Set `Pictures` folder path for screenshots.
                if (_dupenv_s(&buffer, &length, "USERPROFILE") == 0 && buffer != nullptr)
                {
                    auto path          = std::filesystem::path(buffer);
                    _screenshotsFolder = path / "Pictures" / SCREENSHOTS_FOLDER_NAME;
                }
                break;
            }

            case OsType::MacOs:
            {
                // Set `HOME` path for work.
                if (_dupenv_s(&buffer, &length, "HOME") == 0 && buffer != nullptr)
                {
                    auto path   = std::filesystem::path(buffer);
                    _workFolder = path / APP_NAME; 
                }

                // Set `Pictures` folder path for screenshots.
                if (buffer != nullptr)
                {
                    auto path          = std::filesystem::path(buffer);
                    _screenshotsFolder = path / "Pictures" / SCREENSHOTS_FOLDER_NAME;
                }
                break;
            }

            case OsType::Linux:
            {
                // Set `HOME` path for work.
                if (_dupenv_s(&buffer, &length, "HOME") == 0 && buffer != nullptr)
                {
                    auto path   = std::filesystem::path(buffer);
                    _workFolder = path / APP_NAME; 
                }

                // Set `Pictures` folder path for screenshots.
                if (buffer != nullptr)
                {
                    auto path          = std::filesystem::path(buffer);
                    _screenshotsFolder = path / "Pictures" / SCREENSHOTS_FOLDER_NAME;
                }
                break;
            }
        }

        // Check app folder.
        if (_appFolder.empty())
        {
            throw std::runtime_error("Failed to define app folder path.");
        }

        // Check assets folder.
        if (!std::filesystem::exists(_assetsFolder))
        {
            throw std::runtime_error("Assets folder not found in application directory.");
        }

        // Check screenshots folder.
        if (_screenshotsFolder.empty())
        {
            throw std::runtime_error("Failed to define screenshots folder path.");
        }

        // Check work folder.
        if (_workFolder.empty())
        {
            throw std::runtime_error("Failed to define work folder path.");
        }
        
        // Create work folder.
        std::filesystem::create_directories(_workFolder);
        std::filesystem::create_directories(_workFolder / SAVEGAME_FOLDER_NAME);
    }
}
