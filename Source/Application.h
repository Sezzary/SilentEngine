#pragma once

#include "Assets/Assets.h"
#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Savegame/Savegame.h"
#include "Services/Filesystem.h"
#include "Services/Options.h"
#include "Services/Time.h"
#include "Services/Toasts.h"

namespace Silent
{
    using namespace Assets;
    using namespace Audio;
    using namespace Input;
    using namespace Renderer;
    using namespace Savegame;
    using namespace Services;

    constexpr char APP_NAME[]    = "Silent Engine";
    constexpr char APP_VERSION[] = "0.1.0";

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
        // =======
        // Fields
        // =======

        bool            _quit   = false;   /** Quit procedure state. */
        ApplicationWork _work   = {};      /** Subsystem workspace. */
        SDL_Window*     _window = nullptr; /** Application window. */

        Vector2 _mouseWheelAxis = Vector2::Zero; /** Mouse wheel axis input. */

    public:
        // =============
        // Constructors
        // =============

        ApplicationManager() = default;

        // ========
        // Getters
        // ========

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

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the application and its subsystems. */
        void Initialize();

        /** @brief Gracefully deinitializes the application and its subsystems. */
        void Deinitialize();

        /** @brief Runs the application loop. */
        void Run();

        /** @brief Prompts the application to run the shutdown procedure. */
        void Quit();

        /** @brief Toggles between fullscreen and windowed modes. */
        void ToggleFullscreen();

        /** @brief Toggles the mouse cursor on and off. */
        void ToggleCursor();

        /** @brief Toggles the debug GUI on and off. */
        void ToggleDebugGui();

    private:
        // ========
        // Helpers
        // ========

        /** @brief Updates the game application at a fixed timestep. */
        void Update();

        /** @brief Renders the application scene. */
        void Render();

        /** @brief Polls window events to handle quitting, window resizing, toggling fullscreen mode, and connecting or disconnecting a gamepad.
         *
         * @note Additionally polls mouse wheel input as a workaround to input device query limitations.
         */
        void PollEvents();
    };

    extern ApplicationManager g_App;
}
