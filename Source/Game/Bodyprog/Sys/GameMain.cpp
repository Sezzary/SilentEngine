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

    void (*g_GameStateUpdateFuncs[])() =
    {
        GameState_Boot_Update,
        GameState_KonamiLogo_Update,
        GameState_KcetLogo_Update,
        GameState_MovieIntroFadeIn_Update,
        GameState_AutoLoadSavegame_Update,
        GameState_MovieIntroAlternate_Update,
        GameState_MovieIntro_Update,
        GameState_MainMenu_Update,
        GameState_LoadSavegameScreen_Update,
        GameState_MovieOpening_Update,
        GameState_LoadScreen_Update,
        GameState_InGame_Update,
        GameState_MapEvent_Update,
        GameState_ExitMovie_Update,
        nullptr,//GameState_ItemScreens_Update,
        nullptr,//GameState_PaperMapScreen_Update,
        GameState_LoadSavegameScreen_Update,
        GameState_DebugMoviePlayer_Update,
        GameState_Options_Update,
        GameState_LoadStatusScreen_Update,
        GameState_LoadMapScreen_Update,
        nullptr,//GameState_Credits_Update
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
                Fs_QueueStartReadTim(FILE_1ST_KONAMI_TIM, FS_BUFFER_1, &g_KonamiLogoImg);

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

                    g_SysWork.counters_1C[0] = 0;
                    g_SysWork.counters_1C[1] = 0;

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

        Screen_BackgroundImgDraw("1ST/2ZANKO_E.TIM", true);

        // Update luma fade.
        renderer.SetLumaFade(Q8_TO_FLT(g_ScreenFadeProgress), false);
    }
}
