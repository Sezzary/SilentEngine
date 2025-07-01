#pragma once

#include "Engine/Input/Input.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Services/Assets/Assets.h"
#include "Engine/Services/Filesystem.h"
#include "Engine/Services/Options.h"
#include "Engine/Services/Time.h"
#include "Engine/Services/Toasts.h"
#include "Engine/Services/Savegame/Savegame.h"

namespace Silent
{
    using namespace Assets;
    using namespace Input;
    using namespace Renderer;
    using namespace Services;

    struct ApplicationWork
    {
        // TODO: `g_SysWork`, `g_GameWork`, and probably other globals would go here.

        AssetManager                  Assets     = AssetManager();
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
        FilesystemManager& GetFilesystem();
        ToastManager&      GetToasts();
        InputManager&      GetInput();
        OptionsManager&    GetOptions();
        RendererBase&      GetRenderer();
        SavegameManager&   GetSavegame();
        TimeManager&       GetTime();

        Vector2i GetWindowResolution() const;

        // Utilities

        void Initialize();
        void Deinitialize();
        void Run();

        void ToggleFullscreen();
        void ToggleCursor();

    private:
        // Helpers

        void Update();
        void Render();
        void PollEvents();
    };

    extern ApplicationManager g_App;
}
