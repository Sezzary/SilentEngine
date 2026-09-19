#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/GameBoot/GameLoad.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Game/Bodyprog/Demo.h"
#include "Game/Bodyprog/Events/GameSysStates.h"
#include "Game/Bodyprog/Events/Radio.h"
#include "Game/Bodyprog/GameBoot/GameBoot.h"
//#include "Game/Bodyprog/item_screens.h"
//#include "Game/Bodyprog/player.h"
//#include "Game/Bodyprog/ranking.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
//#include "Game/Bodyprog/sound_background.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"
#include "Game/Main/Mem.h"
#include "Game/Main/Rng.h"
#include "Game/Screens/Stream/Stream.h"

namespace Silent::Game
{
    void Anim_CharaTypeAnimInfoClear(void) // 0x800348C0
    {
        //bzero(&g_CharaModelAnimsData[1], 72);
    }

    /** @brief Initalizes drawing of a loading screen. */
    static void GameBoot_LoadingScreen(void) // 0x80034E58
    {
        if (g_SysWork.loadingScreenIdx != LoadingScreenId_None && g_GameWork.gameStateSteps[0] < 10)
        {
            ScreenFade_Start(false, true, false);
            g_ScreenFadeTimestep = Q12(0.8f);
            g_MapOverlayHdr.loadingScreenFuncs[g_SysWork.loadingScreenIdx]();
        }

        Screen_BackgroundMotionBlur(SyncMode_Wait2);
    }

    void GameState_LoadScreen_Update(void) // 0x800348E8
    {
        GameBoot_LoadingScreen();
        GameBoot_WorldStartup();

        if (g_SysWork.sysState & SysFlag_LoadActive)
        {
            D_800BCDD4++;

            if (D_800BCDD4 >= 21)
            {
                g_SysWork.sysState &= ~SysFlag_LoadActive;

                SD_Call(Sfx_Unk1502);
                SD_Call(Sfx_Unk1501);
            }
        }
    }

    static inline void Game_StateStepIncrement(void) // TODO: Move to header?
    {
        int gameStateStep0 = g_GameWork.gameStateSteps[0];

        g_SysWork.gameStateStepCounter = 0;
        g_GameWork.gameStateSteps[1]   = 0;
        g_GameWork.gameStateSteps[2]   = 0;
        g_GameWork.gameStateSteps[0]   = gameStateStep0 + 1;
    }

    void GameBoot_WorldStartup(void) // 0x80034964
    {
        // It makes up to 5 attemps. If the load fails, it restarts
        // the entire process by restarting the timer used to check if a demo
        // should be triggered.
        static int demoLoadAttempCount = 0;

        switch (g_GameWork.gameStateSteps[0])
        {
            case 0:
                g_IntervalVBlanks              = 1;
                g_GameWork.background2dColor.r = 0;
                g_GameWork.background2dColor.g = 0;
                g_GameWork.background2dColor.b = 0;

                if (g_SysWork.processFlags == ProcessFlag_RoomTransition)
                {
                    //AreaLoad_UpdatePlayerPosition();
                    g_GameWork.gameStateSteps[0] = 7;
                }
                else if (g_SysWork.processFlags == ProcessFlag_BootDemo)
                {
                    demoLoadAttempCount            = 0;
                    g_GameWork.gameStateSteps[0]   = 1;
                    g_SysWork.gameStateStepCounter = 1;
                }
                else
                {
                    g_GameWork.gameStateSteps[0] = 3;
                }

                SD_Call(19);
                break;

            case 1:
                if (g_SysWork.gameStateStepCounter > SECONDS_60_FPS(20) &&
                    Fs_QueueGetLength() == 0/* && !Sd_AudioStreamingCheck()*/)
                {
                    Demo_DemoFileSavegameUpdate();
                    GameBoot_PlayerInit();

                    if (Demo_PlayFileBufferSetup() != 0)
                    {
                        GameBoot_MapLoad(g_SavegamePtr->mapIdx);
                        Game_StateStepSet(0, 2);
                        break;
                    }

                    Demo_SequenceAdvance(1);
                    Demo_DemoDataRead();

                    demoLoadAttempCount++;
                    if (demoLoadAttempCount >= 5)
                    {
                        demoLoadAttempCount            = 0;
                        g_SysWork.gameStateStepCounter = 0;
                        break;
                    }
                }
                break;

            case 2:
                if (Fs_QueueGetLength() == 0/* && !Sd_AudioStreamingCheck()*/)
                {
                    Demo_PlayDataRead();
                    Game_StateStepSet(0, 3);
                }
                break;

            case 3:
                if (Fs_QueueGetLength() == 0)
                {
                    g_GameWork.gameStateSteps[0] = 4;
                }
                break;

            case 4:
                if (g_SysWork.processFlags == ProcessFlag_OverlayTransition)
                {
                    //AreaLoad_UpdatePlayerPosition();
                }
                else if (g_SysWork.processFlags == ProcessFlag_LoadSave ||
                        g_SysWork.processFlags == ProcessFlag_Continue)
                {
                    g_SysWork.loadingScreenIdx = LoadingScreenId_PlayerRun;
                }

                g_GameWork.gameStateSteps[0]++;
                break;

            case 5:
                //Fs_CharaAnimDataAlloc(1, g_MapOverlayHdr.charaGroupIds[0], nullptr, 0);
                //Fs_CharaAnimDataAlloc(2, g_MapOverlayHdr.charaGroupIds[1], nullptr, 0);
                //Fs_CharaAnimDataAlloc(3, g_MapOverlayHdr.charaGroupIds[2], nullptr, 0);
                //WorldGfx_MapInitCharaLoad(&g_MapOverlayHdr);

                g_GameWork.gameStateSteps[0]++;

            case 6:
                if (Fs_QueueGetLength() == 0)
                {
                    g_GameWork.gameStateSteps[0]++;
                }
                break;

            case 7:
                //if (AreaLoad_TransitionFlags() & AreaTransitionFlag_UnfreezeWorld)
                {
                    //Map_WorldClear();
                }

                //Ipd_PlayerChunkInit(&g_MapOverlayHdr, g_SysWork.playerWork.player.position.vx, g_SysWork.playerWork.player.position.vz);
                if (g_SysWork.processFlags == ProcessFlag_OverlayTransition)
                {
                    Game_RadioSoundStop();
                }

                g_GameWork.gameStateSteps[0]++;

            case 8:
                //if (Ipd_ChunkInitCheck() != false)
                {
                    Game_StateStepIncrement(0);
                }
                break;

            case 9:
                //if (Bgm_Init() == 0)
                {
                    g_GameWork.gameState = GameState_MainLoadScreen;
                    Game_StateStepIncrement(0);
                }
                break;

            case 10:
                if (g_SysWork.processFlags == ProcessFlag_BootDemo && !(g_SysWork.sysState & SysFlag_DemoActive))
                {
                    Demo_Start();
                    g_SysWork.sysState |= SysFlag_DemoActive;
                }

                //if (AreaLoad_TransitionFlags() & EventParamUnkState_2 || Sd_AmbientSfxInit() == 0)
                {
                    Game_StateStepIncrement(0);
                }
                break;

            case 11:
                if (g_SysWork.gameStateCounter >= 60)
                {
                    if (g_SysWork.processFlags == ProcessFlag_RoomTransition)
                    {
                        //GameBoot_NpcInit();
                    }
                    else
                    {
                        //GameBoot_InGameInit();
                    }

                    if (g_SysWork.processFlags <= (u32)ProcessFlag_OverlayTransition)
                    {
                        //AreaLoad_TransitionSound();
                    }

                    //MemCard_SysDisable();
                    g_GameWork.gameStateSteps[0]++;
                }
                break;

            case 12:
                //if (!Sd_AudioStreamingCheck())
                {
                    Game_StateSetNext(GameState_InGame);

                    //if (AreaLoad_TransitionFlags() & AreaTransitionFlag_SkipFadeIn)
                    {
                        g_GameWork.gameStateSteps[0] = 1;
                        g_Screen_FadeStatus          = SCREEN_FADE_STATUS(ScreenFadeState_ResetTimestep, IS_SCREEN_FADE_WHITE(g_Screen_FadeStatus));
                    }
                }
                break;

            default:
                break;
        }
    }
}
