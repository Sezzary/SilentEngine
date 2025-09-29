#pragma once

#include "Engine/Audio/Audio.h"
#include "Engine/Assets/Assets.h"
#include "Engine/Input/Input.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Savegame/Savegame.h"
#include "Engine/Services/Filesystem.h"
#include "Engine/Services/Options.h"
#include "Engine/Services/Time.h"
#include "Engine/Services/Toasts.h"

namespace Silent
{
    using namespace Assets;
    using namespace Audio;
    using namespace Input;
    using namespace Renderer;
    using namespace Services;

    /** @brief Application workspace. Keeps all subsystems. */
    struct ApplicationWork
    {
        AssetManager                  Assets     = AssetManager();
        AudioManager                  Audio      = AudioManager();
        FilesystemManager             Filesystem = FilesystemManager();
        InputManager                  Input      = InputManager();
        OptionsManager                Options    = OptionsManager();
        std::unique_ptr<RendererBase> Renderer   = nullptr;
        SavegameManager               Savegame   = SavegameManager();
        TimeManager                   Time       = TimeManager();
        ToastManager                  Toasts     = ToastManager();
    };

    class ApplicationManager
    {
    private:
        // Fields

        bool            _isRunning = false;
        ApplicationWork _work      = {};
        SDL_Window*     _window    = nullptr;

        Vector2 _mouseWheelAxis = Vector2::Zero;

    public:
        // Constructors

        ApplicationManager() = default;

        // Getters

        AssetManager&      GetAssets();
        AudioManager&      GetAudio();
        FilesystemManager& GetFilesystem();
        InputManager&      GetInput();
        OptionsManager&    GetOptions();
        RendererBase&      GetRenderer();
        SavegameManager&   GetSavegame();
        TimeManager&       GetTime();
        ToastManager&      GetToasts();

        /** @brief Gets the window resolution.
         *
         * @return Window resolution
         */
        Vector2i GetWindowResolution() const;

        // Utilities

        /** @brief Initializes the application and its subsystems. */
        void Initialize();

        /** @brief Gracefully deinitializes the application and its subsystems. */
        void Deinitialize();

        /** @brief Runs the application. */
        void Run();

        /** @brief Toggles between fullscreen and windowed modes. */
        void ToggleFullscreen();

        /** @brief Toggles the mouse cursor on and off. */
        void ToggleCursor();

    private:
        // Helpers

        void Update();
        void Render();
        void PollEvents();
    };

    extern ApplicationManager g_App;
}
