#pragma once

#include "Assets/AssetStreamer.h"
#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Services/Clock.h"
#include "Services/Filesystem.h"
#include "Services/Options.h"
#include "Services/Savegame.h"
#include "Services/Toasts.h"
#include "Utils/Font.h"
#include "Utils/Parallel.h"
#include "Utils/Translator.h"
#include "Utils/Video.h"

namespace Silent
{
    using namespace Assets;
    using namespace Audio;
    using namespace Input;
    using namespace Renderer;
    using namespace Services;
    using namespace Utils;

    constexpr char APP_NAME[]    = "Silent Engine";
    constexpr char APP_VERSION[] = "0.1.0";

    /** @brief Application workspace. Keeps all subsystems. */
    struct ApplicationWork
    {
        AssetStreamer                 Assets     = AssetStreamer();
        AudioManager                  Audio      = AudioManager();
        ClockManager                  Clock      = ClockManager();
        ParallelExecutor              Executor   = ParallelExecutor();
        FilesystemManager             Filesystem = FilesystemManager();
        FontManager                   Fonts      = FontManager();
        InputManager                  Input      = InputManager();
        OptionsManager                Options    = OptionsManager();
        std::unique_ptr<RendererBase> Renderer   = nullptr;
        SavegameManager               Savegame   = SavegameManager();
        TranslationManager            Translator = TranslationManager();
        ToastManager                  Toaster    = ToastManager();
        VideoPlayer                   Video      = VideoPlayer();
    };

    class ApplicationManager
    {
    private:
        // =======
        // Fields
        // =======

        SDL_Window* _window     = nullptr;
        bool        _isDrawTick = false;
        bool        _isPaused   = false;
        bool        _quit       = false;
        
        ApplicationWork   _work           = {};
        ParallelExecutor  _renderExecutor = ParallelExecutor(1);
        std::future<void> _renderFuture   = std::future<void>();
        Vector2           _mouseWheelAxis = Vector2::Zero;

    public:
        // =============
        // Constructors
        // =============

        /** Creates a default uninitialized instance. */
        ApplicationManager() = default;

        // ========
        // Getters
        // ========

        /** @brief Gets the asset streamer subsystem instance.
         *
         * @return Asset streamer instance.
         */
        AssetStreamer& GetAssets();

        /** @brief Gets the audio subsystem instance.
         *
         * @return Audio instance.
         */
        AudioManager& GetAudio();

        /** @brief Gets the clock subsystem instance.
         *
         * @return Clock instance.
         */
        ClockManager& GetClock();

        /** @brief Gets the executor subsystem instance.
         *
         * @return Executor instance.
         */
        ParallelExecutor& GetExecutor();

        /** @brief Gets the filesystem subsystem instance.
         *
         * @return Filesystem instance.
         */
        FilesystemManager& GetFilesystem();

        /** @brief Gets the fonts subsystem instance.
         *
         * @return Fonts instance.
         */
        FontManager& GetFonts();

        /** @brief Gets the input subsystem instance.
         *
         * @return Input instance.
         */
        InputManager& GetInput();

        /** @brief Gets the options subsystem instance.
         *
         * @return Options instance.
         */
        OptionsManager& GetOptions();

        /** @brief Gets the renderer subsystem instance.
         *
         * @return Renderer instance.
         */
        RendererBase& GetRenderer();

        /** @brief Gets the savegame subsystem instance.
         *
         * @return Savegame instance.
         */
        SavegameManager& GetSavegame();

        /** @brief Gets the toaster subsystem instance.
         *
         * @return Toaster instance.
         */
        ToastManager& GetToaster();

        /** @brief Gets the translator subsystem instance.
         *
         * @return Translator instance.
         */
        TranslationManager& GetTranslator();

        /** @brief Gets the video player instance.
         *
         * @return Video player instance.
         */
        VideoPlayer& GetVideo();

        /** @brief Gets the window resolution.
         *
         * @return Window resolution.
         */
        Vector2i GetWindowResolution() const;

        /** @brief Gets the DPI-aware window content scale.
         *
         * @return Window scale.
         */
        float GetDpiScale() const;

        // ==========
        // Inquirers
        // ==========

        /** @brief Determines if the active tick can be used to submit immediate-mode objects to draw.
         * Used to prevent drawing stale objects when the game logic is catching up.
         *
         * @return `true` if the active tick is a draw tick, `false` otherwise
         */
        bool IsDrawTick() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the application and its subsystems. */
        void Initialize();

        /** @brief Gracefully deinitializes the application and its subsystems. */
        void Deinitialize();

        /** @brief Runs the application loop at at a fixed timestep. */
        void Run();

        /** @brief Prompts the application to run the shutdown procedure. */
        void Quit();

        /** @brief Toggles between fullscreen and windowed modes. */
        void ToggleFullscreen();

        /** @brief Toggles the mouse cursor on and off. */
        void ToggleCursor();

        /** @brief Toggles the debug menu on and off. */
        void ToggleDebugMenu();

        /** @brief Gets text in the system clipboard.
         *
         * @return Clipboard text.
         */
        std::string GetClipboardText();

        /** @brief Sets text in the system clipboard.
         *
         * @param text Text to set.
         */
        void SetClipboardText(const std::string& text);

    private:
        // ========
        // Helpers
        // ========

        /** @brief Updates the application state. */
        void Update();

        /** @brief Renders the application scene. */
        void Render();

        /** @brief Polls window events to handle quitting, window resizing, toggling fullscreen mode, and connecting or disconnecting a gamepad.
         *
         * @note Additionally polls mouse wheel input as a workaround to SDL's input device query limitations.
         */
        void PollEvents();
    };

    extern ApplicationManager g_App;
}
