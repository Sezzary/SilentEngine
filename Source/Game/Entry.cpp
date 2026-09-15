#include "Framework.h"
#include "Psx.h"
#include "Game/Entry.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Demo.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sys/GameMain.h"
#include "Game/Bodyprog/Sys/Joy.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Services/Clock.h"
#include "Utils/Translator.h"

using namespace Silent::Assets;
using namespace Silent::Utils;

namespace Silent::Game
{
    static void MainLoop()
    {
        constexpr q19_12 DELTA_TIME_30_FPS     = Q12(1.0f / (float)Services::TICKS_PER_SECOND);
        constexpr q19_12 GRAVITY_SPEED_PER_SEC = Q12(9.8f);

        // Run game loop.
        static bool isInitComplete = false;
        if (isInitComplete)
        {
            const auto& clock    = g_App.GetClock();
            auto&       renderer = g_App.GetRenderer();

            g_TickCount++;

            // Update input.
            //Demo_ControllerDataUpdate();
            //Joy_ControllerDataUpdate();

            /*if (MainLoop_ShouldWarmReset() == 2)
            {
                Game_WarmBoot();
                continue;
            }*/

            g_SysWork.bgmStatusFlags = BgmStatusFlag_None;

            // Call update function for current GameState.
            if (g_GameStateUpdateFuncs[g_GameWork.gameState])
            {
                g_GameStateUpdateFuncs[g_GameWork.gameState]();
            }

            Demo_Update();
            Demo_GameRandSeedSet();

            /*if (MainLoop_ShouldWarmReset() == 2)
            {
                Game_WarmBoot();
                continue;
            }*/

            Screen_FadeUpdate();
            //Sd_TaskPoolExecute();

            /*if (Sd_AudioStreamingCheck() == AudioStreamingState_None)
            {
                Fs_QueueUpdate();
            }*/

            /*func_80089128();
            func_8008D78C(); // Camera update?*/

            // Handle demo presentation interval.
            if (g_SysWork.sysState & SysFlag_DemoActive)
            {
                Demo_PresentIntervalUpdate();
            }

            // Update delta time.
            g_DeltaTime    =
            g_DeltaTimeRaw = DELTA_TIME_30_FPS;
            g_GravitySpeed = Q12_MULT(DELTA_TIME_30_FPS, GRAVITY_SPEED_PER_SEC);

            // Set clear color.
            renderer.SetClearColor(Color::From8Bit(g_GameWork.background2dColor.r,
                                                   g_GameWork.background2dColor.g,
                                                   g_GameWork.background2dColor.b));
        }
        // Initialize engine.
        else
        {
            //sd_init();

            isInitComplete = true;
        }
    }

    void UpdateGame()
    {
        constexpr q23_8 FADE_STEP = Q8(1 / 32.0f);

        static bool isInitComplete = false;

        const auto& translator = g_App.GetTranslator();

        // Run game.
        if (isInitComplete)
        {
            MainLoop();
        }
        // Initialize.
        else
        {
            // @temp Skip boot.
            g_GameWork.gameState = (e_GameState)7;
            isInitComplete = true;
            return;

            auto& assets   = g_App.GetAssets();
            auto& renderer = g_App.GetRenderer();

            // Set clear color.
            renderer.SetClearColor(Color::Clear);

            // Load `Textures/SplashScreen.png` ("There are violent and disturbing images in this game.").
            assets.Load("Textures/SplashScreen.png").wait();

            // Fade in `Textures/SplashScreen.png` with luma-multiply blending.
            static q23_8 fade = Q8(0.0f);
            if (fade >= Q8_COLOR(1.0f))
            {
                isInitComplete = true;
            }
            else
            {
                Screen_BackgroundImgDraw("Textures/SplashScreen.png", true, NO_VALUE);

                // @todo Style it properly.
                Gfx_StringPositionSet(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 7) * 3);
                Gfx_StringColorSet(StringColorId_White);
                Gfx_StringDraw(translator(KEY_INTRO_WARNING));

                // Update luma fade.
                renderer.SetLumaFade(Q8_TO_FLT(fade), false);
                fade = std::clamp(fade + FADE_STEP, Q8(0.0f), Q8(1.0f));
            }
        }
    }
}
