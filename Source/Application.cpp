#include "Framework.h"
#include "Application.h"

#include "Assets/Assets.h"
#include "Assets/Fonts.h"
#include "Assets/Locales.h"
#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Game/main.h"
#include "Renderer/Renderer.h"
#include "Savegame/Savegame.h"
#include "Services/Filesystem.h"
#include "Services/Options.h"
#include "Services/Time.h"
#include "Services/Toasts.h"
#include "Utils/Font.h"
#include "Utils/Parallel.h"
#include "Utils/Translator.h"

using namespace Silent::Assets;
using namespace Silent::Audio;
using namespace Silent::Input;
using namespace Silent::Renderer;
using namespace Silent::Savegame;
using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent
{
    ApplicationManager g_App = ApplicationManager();

    AssetManager& ApplicationManager::GetAssets()
    {
        return _work.Assets;
    }

    AudioManager& ApplicationManager::GetAudio()
    {
        return _work.Audio;
    }

    FilesystemManager& ApplicationManager::GetFilesystem()
    {
        return _work.Filesystem;
    }

    InputManager& ApplicationManager::GetInput()
    {
        return _work.Input;
    }

    OptionsManager& ApplicationManager::GetOptions()
    {
        return _work.Options;
    }

    RendererBase& ApplicationManager::GetRenderer()
    {
        return *_work.Renderer;
    }

    SavegameManager& ApplicationManager::GetSavegame()
    {
        return _work.Savegame;
    }

    TimeManager& ApplicationManager::GetTime()
    {
        return _work.Time;
    }

    ToastManager& ApplicationManager::GetToaster()
    {
        return _work.Toaster;
    }

    TranslationManager& ApplicationManager::GetTranslator()
    {
        return _work.Translator;
    }

    Vector2i ApplicationManager::GetWindowResolution() const
    {
        auto res = Vector2i::Zero;
        SDL_GetWindowSizeInPixels(_window, &res.x, &res.y);
        return res;
    }

        auto fonts = FontManager();
    void ApplicationManager::Initialize()
    {
        _quit = false;

        // Filesystem.
        _work.Filesystem.Initialize();

        // Debug.
        Debug::Initialize();
        Debug::Log("Starting " + std::string(APP_NAME) + " " + APP_VERSION + "...");

        // Options.
        _work.Options.Initialize();
        _work.Options.Load();

        // SDL.
        if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
        {
            throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
        }

        // Collect window flags.
        int fullscreenFlag = _work.Options->EnableFullscreen ? SDL_WINDOW_FULLSCREEN : 0;
        int maximizedFlag  = _work.Options->EnableMaximized  ? SDL_WINDOW_MAXIMIZED  : 0;
        int flags          = SDL_WINDOW_RESIZABLE | fullscreenFlag | maximizedFlag;

        // Create window.
        _window = SDL_CreateWindow(APP_NAME, _work.Options->WindowedSize.x, _work.Options->WindowedSize.y, flags);
        if (_window == nullptr)
        {
            throw std::runtime_error("Failed to create window: " + std::string(SDL_GetError()));
        }

        // Assets.
        _work.Assets.Initialize(_work.Filesystem.GetAssetsDirectory() / ASSETS_PSX_DIR_NAME);
        _work.Translator.Initialize(_work.Filesystem.GetAssetsDirectory() / ASSETS_LOCALES_DIR_NAME, LOCALE_NAMES);

        // Fonts.
        fonts.LoadFont(_work.Filesystem.GetAssetsDirectory() / ASSETS_FONTS_DIR_NAME / "FreeSerif.otf", 32, " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");

        // Renderer.
        _work.Renderer = CreateRenderer(RendererType::SdlGpu);
        if (_work.Renderer == nullptr)
        {
            throw std::runtime_error("Failed to create renderer.");
        }
        _work.Renderer->Initialize(*_window);

        // Audio.
        _work.Audio.Initialize();

        // Input.
        _work.Input.Initialize();

        // Finish.
        Debug::Log("Startup complete.");
    }

    void ApplicationManager::Deinitialize()
    {
        Debug::Log("Shutting down...");

        // Workspace.
        _work.Audio.Deinitialize();
        _work.Input.Deinitialize();
        _work.Renderer->Deinitialize();

        // SDL.
        SDL_DestroyWindow(_window);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        SDL_Quit();

        // Finish.
        Debug::Log("Shutdown complete.");
    }

    void ApplicationManager::Run()
    {
        _work.Time.Initialize();

        while (!_quit)
        {
            _work.Time.Update();

            // Step game state and render.
            Update();
            Render();

            _work.Time.WaitForNextTick();
        }
    }

    void ApplicationManager::Quit()
    {
        _quit = true;
    }

    void ApplicationManager::ToggleFullscreen()
    {
        if (SDL_SetWindowFullscreen(_window, !_work.Options->EnableFullscreen))
        {
            Debug::Log("Toggled fullscreen mode.", Debug::LogLevel::Info, Debug::LogMode::All, true);
            return;
        }

        Debug::Log("Failed to toggle fullscreen mode: " + std::string(SDL_GetError()), Debug::LogLevel::Warning);
    }

    void ApplicationManager::ToggleCursor()
    {
        // Show.
        if ((!_work.Options->EnableFullscreen || _work.Options->EnableDebugGui) && !SDL_CursorVisible())
        {
            if (!SDL_ShowCursor())
            {
                Debug::Log("Failed to show cursor: " + std::string(SDL_GetError()), Debug::LogLevel::Warning);
            }

            // Move cursor to window center.
            auto pos = GetWindowResolution().ToVector2() / 2.0f;
            SDL_WarpMouseInWindow(_window, pos.x, pos.y);
        }
        // Hide.
        else if (_work.Options->EnableFullscreen && SDL_CursorVisible())
        {
            if (!SDL_HideCursor())
            {
                Debug::Log("Failed to hide cursor: " + std::string(SDL_GetError()), Debug::LogLevel::Warning);
            }
        }
    }

    void ApplicationManager::ToggleDebugGui()
    {
        if (!_work.Options->EnableDebugMode)
        {
            return;
        }

        _work.Options->EnableDebugGui = !_work.Options->EnableDebugGui;
        Debug::g_Work.Page            = _work.Options->EnableDebugGui ? Debug::Page::Renderer : Debug::Page::None;
        ToggleCursor();

        Debug::Log("Toggled debug mode.", Debug::LogLevel::Info, Debug::LogMode::All, true);
    }

    void ApplicationManager::Update()
    {
        // Poll events and input.
        PollEvents();
        _work.Input.Update(*_window, _mouseWheelAxis);

        // Update game state.
        Entry();

        // Update audio.
        _work.Audio.Update();

        // Update debug and toasts.
        Debug::Update();
        _work.Toaster.Update();
    }

    void ApplicationManager::Render()
    {
        if (_work.Time.GetTicks() <= 0)
        {
            return;
        }

        // Render scene.
        _work.Renderer->Update();
    }

    void ApplicationManager::PollEvents()
    {
        auto event = SDL_Event{};
        while (SDL_PollEvent(&event))
        {
            if (_work.Options->EnableDebugGui)
            {
                ImGui_ImplSDL3_ProcessEvent(&event);
            }

            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                {
                    Quit();
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED:
                {
                    // Ignore if fullscreen or maximized.
                    auto windowFlags = SDL_GetWindowFlags(_window);
                    if (windowFlags & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_MAXIMIZED))
                    {
                        break;
                    }

                    // Get new resolution.
                    auto res = Vector2i::Zero;
                    SDL_GetWindowSizeInPixels(_window, &res.x, &res.y);
                    if (_work.Options->WindowedSize == res)
                    {
                        break;
                    }

                    // Update options.
                    _work.Options->WindowedSize = res;
                    _work.Options.Save();

                    // Update render size.
                    _work.Renderer->SignalResize();
                    break;
                }
                case SDL_EVENT_WINDOW_MAXIMIZED:
                case SDL_EVENT_WINDOW_RESTORED:
                {
                    // Update options.
                    auto windowFlags               = SDL_GetWindowFlags(_window);
                    _work.Options->EnableMaximized = windowFlags & SDL_WINDOW_MAXIMIZED;
                    _work.Options.Save();

                    // Update render size.
                    _work.Renderer->SignalResize();
                    break;
                }
                case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
                case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
                {
                    // Update options.
                    _work.Options->EnableFullscreen = event.type == SDL_EVENT_WINDOW_ENTER_FULLSCREEN;
                    _work.Options.Save();

                    // Show/hide cursor.
                    ToggleCursor();

                    // Update render size.
                    _work.Renderer->SignalResize();
                    break;
                }
                case SDL_EVENT_MOUSE_WHEEL:
                {
                    _mouseWheelAxis = Vector2(event.wheel.x, event.wheel.y);
                    break;
                }
                case SDL_EVENT_GAMEPAD_ADDED:
                {
                    // Attempt connecting gamepad.
                    _work.Input.ConnectGamepad(event.cdevice.which);
                    break;
                }
                case SDL_EVENT_GAMEPAD_REMOVED:
                {
                    // Attempt disconnecting gamepad.
                    _work.Input.DisconnectGamepad(event.cdevice.which);
                    break;
                }
            }
        }
    }
}
