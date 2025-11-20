#pragma once

#include "Assets/Assets.h"
#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Savegame/Savegame.h"
#include "Services/Clock.h"
#include "Services/Filesystem.h"
#include "Services/Options.h"
#include "Services/Toasts.h"
#include "Utils/Font.h"
#include "Utils/Parallel.h"
#include "Utils/Translator.h"

namespace Silent
{
    using namespace Assets;
    using namespace Audio;
    using namespace Input;
    using namespace Renderer;
    using namespace Savegame;
    using namespace Services;
    using namespace Utils;

    constexpr char APP_NAME[]    = "Silent Engine";
    constexpr char APP_VERSION[] = "0.1.0";

    /** @brief Application workspace. Keeps all subsystems. */
    struct ApplicationWork
    {
        AssetManager                  Assets     = AssetManager();
        AudioManager                  Audio      = AudioManager();
        FilesystemManager             Filesystem = FilesystemManager();
        FontManager                   Fonts      = FontManager();
        InputManager                  Input      = InputManager();
        OptionsManager                Options    = OptionsManager();
        ParallelExecutor              Executor   = ParallelExecutor();
        std::unique_ptr<RendererBase> Renderer   = nullptr;
        SavegameManager               Savegame   = SavegameManager();
        ClockManager                  Clock      = ClockManager();
        TranslationManager            Translator = TranslationManager();
        ToastManager                  Toaster    = ToastManager();
    };

    class ApplicationManager
    {
    private:
        // =======
        // Fields
        // =======

        SDL_Window* _window   = nullptr; /** Application window. */
        bool        _isPaused = false;   /** "Window out of focus" state. */
        bool        _quit     = false;   /** Quit procedure state. */

        ApplicationWork _work           = {};            /** Subsystem workspace. */
        Vector2         _mouseWheelAxis = Vector2::Zero; /** Mouse wheel axis input. */

    public:
        // =============
        // Constructors
        // =============

        /** Constructs a default uninitialized `ApplicationManager`. */
        ApplicationManager() = default;

        // ========
        // Getters
        // ========

        AssetManager&       GetAssets();
        AudioManager&       GetAudio();
        ParallelExecutor&   GetExecutor();
        FilesystemManager&  GetFilesystem();
        FontManager&        GetFonts();
        InputManager&       GetInput();
        OptionsManager&     GetOptions();
        RendererBase&       GetRenderer();
        SavegameManager&    GetSavegame();
        ClockManager&       GetClock();
        TranslationManager& GetTranslator();
        ToastManager&       GetToaster();

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

        /** @brief Updates the game render buffer. */
        void UpdateRenderBuffer();

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
