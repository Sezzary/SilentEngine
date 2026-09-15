#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/BKonami/BKonami.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Game/Bodyprog/Demo.h"
#include "Game/Bodyprog/MemCard.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sys/FsScreens.h"
#include "Game/Bodyprog/Sys/Joy.h"
#include "Game/Main/FsQueue.h"
#include "Game/Screens/Stream/Stream.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Renderer;

namespace Silent::Game
{
    void GameState_KonamiLogo_Update() // 0x800C95AC
    {
        const auto& input = g_App.GetInput();

        //while (g_GameWork.gameState == GameState_KonamiLogo)
        switch (g_GameWork.gameStateSteps[0])
        {
            case KonamiLogoStateStep_Init:
                ScreenFade_Start(true, true, false);
                g_ScreenFadeTimestep = Q12(0.2f);

                // Load `Psx/1ST/KONAMI2.TIM` (Konami logo).
                Fs_QueueStartReadTim(FILE_1ST_KONAMI2_TIM);

                //WorldGfx_HarryCharaLoad();
                //GameFs_BgItemLoad();
                //Map_EffectTexturesLoad(NO_VALUE);

                // Start loading `Psx/ANIM/HB_BASE.ANM` (base Harry animations).
                Fs_QueueStartRead(FILE_ANIM_HB_BASE_ANM, FS_BUFFER_0);

                g_GameWork.gameStateSteps[0]++;
                break;

            case KonamiLogoStateStep_WaitForFade:
                if (ScreenFade_IsNone())
                {
                    g_GameWork.gameStateSteps[0] = KonamiLogoStateStep_LogoDelay;
                }
                break;

            case KonamiLogoStateStep_LogoDelay:
                if (g_Controller0->buttonFlags.held != 0 || g_SysWork.gameStateCounter > 180)
                {
                    ScreenFade_Start(false, false, false);
                    g_ScreenFadeTimestep         = Q12(0.2f);
                    g_GameWork.gameStateSteps[0] = KonamiLogoStateStep_FinishAfterFade;
                }
                break;

            case KonamiLogoStateStep_FinishAfterFade:
                if (ScreenFade_IsFinished())
                {
                    Fs_QueueWaitForEmpty();
                    Game_StateSetNext(GameState_KcetLogo);
                }
                break;
        }

        // @todo Original code depended on blocking logic, but the new way continues the game loop.
        // Need to do some light refactoring here for logos to show properly.
        //if (g_GameWork.gameState != GameState_KonamiLogo)
        {
            BootScreen_KonamiScreenDraw();
            //Screen_FadeUpdate();
            //MemCard_Update();
            //func_80033548();
        }

        if (input.GetAction(In::Enter).IsClicked())
        {
            Game_StateSetNext(GameState_KcetLogo);
        }
    }

    s32 GameState_KcetLogo_MemCardCheck() // 0x800C9874
    {
        // @stub
        return KcetLogoStateStep_HasSavegame;

        /*s32 saveEntryTypeSlot1;
        s32 saveEntryTypeSlot_2;

        // Memory cards not ready yet, rerun this on next frame.
        if (func_80033548() == false)
        {
            return KcetLogoStateStep_CheckMemCards;
        }

        g_MemCard_ActiveMemCardSlotSaves = (s_SaveScreenElement*)SAVEGAME_ENTRY_BUFFER_0;
        saveEntryTypeSlot1                = g_MemCard_ActiveMemCardSlotSaves->type_4;

        g_MemCard_ActiveMemCardSlotSaves = (s_SaveScreenElement*)SAVEGAME_ENTRY_BUFFER_1;
        saveEntryTypeSlot_2                = g_MemCard_ActiveMemCardSlotSaves->type_4;

        // No memory cards.
        if (saveEntryTypeSlot1 == SavegameEntryType_NoMemCard && saveEntryTypeSlot_2 == SavegameEntryType_NoMemCard)
        {
            return KcetLogoStateStep_NoMemCard;
        }

        // No free space on any card.
        if ((saveEntryTypeSlot1 == SavegameEntryType_OutOfBlocks && (saveEntryTypeSlot_2 == SavegameEntryType_OutOfBlocks || saveEntryTypeSlot_2 == SavegameEntryType_NoMemCard)) ||
            (saveEntryTypeSlot1 == SavegameEntryType_NoMemCard && saveEntryTypeSlot_2 == SavegameEntryType_OutOfBlocks))
        {
            return KcetLogoStateStep_NoMemCardFreeSpace;
        }

        if (saveEntryTypeSlot1 == SavegameEntryType_Save || saveEntryTypeSlot_2 == SavegameEntryType_Save)
        {
            g_MemCard_ActiveMemCardSlotSaves = MemCard_ActiveMemCardSlotGet(g_SelectedSaveSlotIdx);
            g_MemCard_ActiveMemCardSlotSaves = &g_MemCard_ActiveMemCardSlotSaves[g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx]];

            g_SelectedDeviceId            = g_MemCard_ActiveMemCardSlotSaves->deviceId_5;
            g_SelectedFileIdx             = g_MemCard_ActiveMemCardSlotSaves->fileIdx_6;
            g_Savegame_SelectedElementIdx = g_MemCard_ActiveMemCardSlotSaves->elementIdx_7;

            return KcetLogoStateStep_HasSavegame;
        }

        return KcetLogoStateStep_NoSaveGame;*/
    }

    void GameState_KcetLogo_Update() // 0x800C99A4
    {
        static e_GameState nextGameState = GameState_Init;

        //while (g_GameWork.gameState == GameState_KcetLogo)
        switch (g_GameWork.gameStateSteps[0])
        {
            case KcetLogoStateStep_Init:
                //Settings_RestoreDefaults();

                ScreenFade_Start(true, true, false);
                g_ScreenFadeTimestep = Q12(0.2f);

                //GameFs_BgEtcGfxLoad();
                //Fs_QueueStartRead(FILE_BG_HP_SAFE1_BIN, FS_BUFFER_5);
                //Fs_QueueStartRead(FILE_BG_S__SAFE2_BIN, FS_BUFFER_6);
                g_GameWork.gameStateSteps[0]++;
                break;

            case KcetLogoStateStep_CheckMemCards:
                if (ScreenFade_IsNone())
                {
                    s32 curTime;

                    Fs_QueueWaitForEmpty();

                    //while (g_GameWork.gameStateSteps[0] < KcetLogoStateStep_NoMemCard)
                    {
                        g_GameWork.gameStateSteps[0] = GameState_KcetLogo_MemCardCheck();
                        //MemCard_Update();
                        //VSync(SyncMode_Wait);
                    }
                }
                break;

            // @deprecated
            case KcetLogoStateStep_NoMemCard:
                nextGameState = GameState_MovieIntroFadeIn;
                Game_StateStepSet(0, KcetLogoStateStep_LogoDelay);
                break;

            // @deprecated
            case KcetLogoStateStep_NoMemCardFreeSpace:
                nextGameState = GameState_MovieIntroFadeIn;
                Game_StateStepSet(0, KcetLogoStateStep_LogoDelay);
                break;

            case KcetLogoStateStep_NoSaveGame:
                GameFs_TitleGfxSeek();
                nextGameState = GameState_MovieIntro;

                Game_StateStepSet(0, KcetLogoStateStep_LogoDelay);
                break;

            case KcetLogoStateStep_HasSavegame:
                while (g_GameWork.gameStateSteps[1] < 3)
                {
                    switch (g_GameWork.gameStateSteps[1])
                    {
                        case 0:
                            //MemCard_ProcessSet(MemCardProcess_Load_Game, g_SelectedDeviceId, 0, 0);
                            Game_StateStepIncrement(1);

                        case 1:
                            //if (MemCard_LastMemCardResultGet() != MemCardResult_Success)
                            //{
                                Game_StateStepIncrement(1);
                            //}
                            break;

                        case 2:
                            if (g_GameWorkConst->config.autoLoad)
                            {
                                g_App.GetAssets().Load("Psx/TIM/SAVELOAD.TIM");
                                nextGameState = GameState_AutoLoadSavegame;
                            }
                            else
                            {
                                GameFs_TitleGfxSeek();
                                nextGameState = GameState_MovieIntro;
                            }

                            Game_StateStepIncrement(1);
                            break;
                    }

                    //func_80033548();
                    //MemCard_Update();
                }

                Game_StateStepSet(0, KcetLogoStateStep_LogoDelay);
                break;

            case KcetLogoStateStep_LogoDelay:
                if (g_Controller0->buttonFlags.held != 0 || g_SysWork.gameStateCounter > 180)
                {
                    ScreenFade_Start(false, false, false);
                    g_ScreenFadeTimestep = Q12(0.2f);
                    g_GameWork.gameStateSteps[0]++;
                }
                break;

            case KcetLogoStateStep_FinishAfterFade:
                if (ScreenFade_IsFinished())
                {
                    //Settings_ScreenAndVolUpdate();

                    switch (nextGameState)
                    {
                        case GameState_AutoLoadSavegame:
                            Fs_QueueStartReadTim(FILE_TIM_SAVELOAD_TIM, FS_BUFFER_7, &g_ItemInspectionImg);
                            break;

                        case GameState_MovieIntroFadeIn:
                            break;

                        case GameState_MovieIntroAlternate:
                        default:
                            GameFs_TitleGfxLoad();
                            break;
                    }

                    Demo_SequenceAdvance(0);
                    Demo_DemoDataRead();
                    Fs_QueueWaitForEmpty();
                    Game_StateSetNext(nextGameState);
                }
                break;
        }

        BootScreen_KcetScreenDraw();
        //Screen_FadeUpdate();
        //MemCard_Update();
        //func_80033548();
        //VSync(SyncMode_Wait);
        //GsSwapDispBuff();
        //GsDrawOt(&g_OrderingTable2[g_ActiveBufferIdx]);

        //g_ActiveBufferIdx = GsGetActiveBuff();
        //GsOUT_PACKET_P   = (g_ActiveBufferIdx << 0xF) + (u32)TEMP_MEMORY_ADDR;
        //GsClearOt(0, 0, &g_OrderingTable0[g_ActiveBufferIdx]);
        //GsClearOt(0, 0, &g_OrderingTable2[g_ActiveBufferIdx]);
    }

    void BootScreen_KonamiScreenDraw()
    {
        auto& renderer = g_App.GetRenderer();

        Screen_BackgroundImgDraw("Psx/1ST/KONAMI.TIM", true);
        renderer.SetLumaFade(Q8_TO_FLT(g_ScreenFadeProgress), false);
    }

    void BootScreen_KcetScreenDraw()
    {
        auto& renderer = g_App.GetRenderer();

        Screen_BackgroundImgDraw("Psx/1ST/KONAMI2.TIM", true);

        // Update luma fade.
        renderer.SetLumaFade(Q8_TO_FLT(g_ScreenFadeProgress), false);
    }
}
