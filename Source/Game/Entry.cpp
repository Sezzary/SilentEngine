#include "Framework.h"
#include "Psx.h"
#include "Game/Entry.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Game/Bodyprog/Demo.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sys/GameMain.h"
#include "Game/Bodyprog/Sys/Joy.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Services/Clock.h"
#include "Renderer/Renderer.h"
#include "Utils/Translator.h"

using namespace Silent::Renderer;
using namespace Silent::Utils;

namespace Silent::Game
{
    void UpdateGame()
    {
        constexpr q19_12 FIXED_DELTA_TIME      = Q12(1.0f / (float)Services::TICKS_PER_SECOND);
        constexpr q19_12 GRAVITY_SPEED_PER_SEC = Q12(9.8f);

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

        // Set clear color.
        renderer.SetClearColor(Color::From8Bit(g_GameWork.background2dColor.r,
                                               g_GameWork.background2dColor.g,
                                               g_GameWork.background2dColor.b));
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
        g_DeltaTimeRaw = FIXED_DELTA_TIME;
        g_GravitySpeed = Q12_MULT(FIXED_DELTA_TIME, GRAVITY_SPEED_PER_SEC);
    }
}
