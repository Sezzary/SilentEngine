#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Sys/GameMain.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Game/Bodyprog/MemCard.h"
#include "Game/Bodyprog/Demo.h"
#include "Game/Bodyprog/Events/GameSysStates.h"
#include "Game/Bodyprog/GameBoot/GameLoad.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Bodyprog/Sys/Joy.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"
#include "Game/Screens/BKonami/BKonami.h"
#include "Game/Screens/Options/Options.h"
#include "Game/Screens/SaveLoad/SaveLoad.h"
#include "Renderer/Renderer.h"

using namespace Silent::Assets;

namespace Silent::Game
{
    s32 g_Demo_FrameCount = 0;
    s32 g_WarmBootTimer   = 0;

    static s32 g_PrevVBlanks = 0;

    // Audio task for `SD_Call` meant to load some VAB audio.
    u16 g_BaseVabAudiosTaskId[] =
    {
        160,
        162,
        0
    };

    // @todo Remove checklist when all are working.
    void (*g_GameStateUpdateFuncs[])() =
    {
        GameState_Boot_Update,                     // @todo
        GameState_KonamiLogo_Update,               // @todo
        GameState_KcetLogo_Update,                 // @todo
        GameState_MovieIntroFadeIn_Update,         // @todo
        GameState_AutoLoadSavegame_Update,         // @todo
        GameState_MovieIntroAlternate_Update,
        GameState_MovieIntro_Update,
        GameState_MainMenu_Update,                 // @todo
        GameState_LoadSavegameScreen_Update,       // @todo
        GameState_MovieOpening_Update,
        GameState_LoadScreen_Update,               // @todo
        GameState_InGame_Update,                   // @todo
        GameState_MapEvent_Update,                 // @todo
        GameState_ExitMovie_Update,                // @todo
        nullptr,//GameState_ItemScreens_Update,    // @todo
        nullptr,//GameState_PaperMapScreen_Update, // @todo
        GameState_LoadSavegameScreen_Update,       // @todo
        GameState_DebugMoviePlayer_Update,         // @todo
        GameState_Options_Update,                  // @todo
        GameState_LoadStatusScreen_Update,         // @todo
        GameState_LoadMapScreen_Update,            // @todo
        nullptr,//GameState_Credits_Update         // @todo
    };

    void GameState_Boot_Update() // 0x80032D1C
    {
        e_GameState gameState;
        s32         vabAudioTaskId;

        const auto& assets   = g_App.GetAssets();
        auto&       renderer = g_App.GetRenderer();

        switch (g_GameWork.gameStateSteps[0])
        {
            case 0:
                g_GameWork.background2dColor.r = 0;
                g_GameWork.background2dColor.g = 0;
                g_GameWork.background2dColor.b = 0;

                Game_StateStepIncrement(0);
                break;

            case 1:
                //if (!Sd_AudioStreamingCheck())
                {
                    vabAudioTaskId = g_BaseVabAudiosTaskId[g_GameWork.gameStateSteps[1]];
                    if (vabAudioTaskId != 0)
                    {
                        SD_Call(vabAudioTaskId);
                        g_GameWork.gameStateSteps[1]++;
                    }
                    else
                    {
                        Game_StateStepIncrement(0);
                    }
                }
                break;

            case 2:
                Fs_QueueStartReadTim(FILE_1ST_KONAMI_TIM);

                ScreenFade_Start(true, false, false);
                g_GameWork.gameStateSteps[0]++;
                break;

            case 3:
                if (ScreenFade_IsFinished())
                {
                    // @todo Avoid blocking.
                    //Fs_QueueWaitForEmpty();
                    if (assets.IsBusy())
                    {
                        break;
                    }

                    gameState = g_GameWork.gameState;

                    g_SysWork.gameStateCounter     = 0;
                    g_SysWork.gameStateStepCounter = 0;

                    g_GameWork.gameStateSteps[1] = 0;
                    g_GameWork.gameStateSteps[2] = 0;

                    SysWork_StateSetNext(SysState_Gameplay);

                    g_GameWork.gameStateSteps[0] = gameState;
                    g_GameWork.gameState         = (e_GameState)((int)gameState + 1);
                    g_GameWork.gameStatePrev     = gameState;
                    g_GameWork.gameStateSteps[0] = 0;
                }
                break;
        }

        Screen_BackgroundImgDraw("Textures/SplashScreen.png", true, NO_VALUE);

        // Update luma fade.
        renderer.SetLumaFade(Q8_TO_FLT(g_ScreenFadeProgress), false);
    }
}
