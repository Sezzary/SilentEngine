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
#include "Services/Clock.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Renderer;
using namespace Silent::Services;

namespace Silent::Game
{
    // Used by both `GameState_KcetLogo_MemCardCheck` and `GameState_KcetLogo_Update`.
    enum e_KcetLogoStateStep
    {
        KcetLogoStateStep_Init,
        KcetLogoStateStep_CheckMemCards,
        KcetLogoStateStep_NoSavegame,
        KcetLogoStateStep_HasSavegame,
        KcetLogoStateStep_LogoDelay,
        KcetLogoStateStep_FinishAfterFade
    };

    void GameState_KonamiLogo_Update() // 0x800C95AC
    {
        enum e_KonamiLogoStateStep
        {
            KonamiLogoStateStep_Init,
            KonamiLogoStateStep_WaitForFade,
            KonamiLogoStateStep_LogoDelay,
            KonamiLogoStateStep_FinishAfterFade
        };

        const auto& assets = g_App.GetAssets();
        const auto& input  = g_App.GetInput();

        switch (g_GameWork.gameStateSteps[0])
        {
            case KonamiLogoStateStep_Init:
                ScreenFade_Start(true, true, false, Q12(1.0f));

                // Load `Psx/1ST/KONAMI2.TIM` (Konami logo).
                Fs_QueueStartReadTim(FILE_1ST_KONAMI2_TIM);

                // @todo
                //WorldGfx_HarryCharaLoad();
                //GameFs_BgItemLoad();
                //Map_EffectTexturesLoad(NO_VALUE);

                // Start loading `Psx/ANIM/HB_BASE.ANM` (base Harry animations).
                Fs_QueueStartRead(FILE_ANIM_HB_BASE_ANM, FS_BUFFER_0);

                Game_StateStepIncrement(0);
                break;

            case KonamiLogoStateStep_WaitForFade:
                if (ScreenFade_IsNone())
                {
                    Game_StateStepIncrement(0);
                }
                break;

            case KonamiLogoStateStep_LogoDelay:
                if (input.GetAction(In::Enter).IsHeld()  ||
                    input.GetAction(In::Cancel).IsHeld() ||
                    g_SysWork.gameStateCounter >= SEC_TO_TICK(3.0f))
                {
                    ScreenFade_Start(false, false, false, Q12(1.0f));

                    Game_StateStepIncrement(0);
                }
                break;

            case KonamiLogoStateStep_FinishAfterFade:
                if (ScreenFade_IsFinished() && !assets.IsBusy())
                {
                    Game_StateSetNext(GameState_KcetLogo);
                }
                break;
        }

        // @todo Original code depended on blocking logic, but the new way continues the game loop.
        // Need to do some light refactoring here for logos to show properly.
        //if (g_GameWork.gameState != GameState_KonamiLogo)
        {
            BootScreen_KonamiScreenDraw();
            //MemCard_Update();
            //func_80033548();
        }
    }

    /** @brief Checks memory cards for free space and existing savegames.
     *
     * @return `e_KcetLogoStateStep`.
     */
    static s32 GameState_KcetLogo_MemCardCheck()
    {
        // @todo
        //return KcetLogoStateStep_HasSavegame;
        return KcetLogoStateStep_NoSavegame;

        /*s32 saveEntryTypeSlot1;
        s32 saveEntryTypeSlot_2;

        // Memory cards not ready yet, rerun this on next frame.
        if (func_80033548() == false)
        {
            return KcetLogoStateStep_CheckMemCards;
        }

        g_MemCard_ActiveMemCardSlotSaves = (s_SaveScreenElement*)SAVEGAME_ENTRY_BUFFER_0;
        saveEntryTypeSlot1                = g_MemCard_ActiveMemCardSlotSaves->type;

        g_MemCard_ActiveMemCardSlotSaves = (s_SaveScreenElement*)SAVEGAME_ENTRY_BUFFER_1;
        saveEntryTypeSlot_2                = g_MemCard_ActiveMemCardSlotSaves->type;

        if (saveEntryTypeSlot1 == SavegameEntryType_Save || saveEntryTypeSlot_2 == SavegameEntryType_Save)
        {
            g_MemCard_ActiveMemCardSlotSaves = MemCard_ActiveMemCardSlotGet(g_SelectedSaveSlotIdx);
            g_MemCard_ActiveMemCardSlotSaves = &g_MemCard_ActiveMemCardSlotSaves[g_SlotElementSelectedIdx[g_SelectedSaveSlotIdx]];

            g_SelectedDeviceId            = g_MemCard_ActiveMemCardSlotSaves->deviceId;
            g_SelectedFileIdx             = g_MemCard_ActiveMemCardSlotSaves->fileIdx;
            g_Savegame_SelectedElementIdx = g_MemCard_ActiveMemCardSlotSaves->elementIdx;

            return KcetLogoStateStep_HasSavegame;
        }

        return KcetLogoStateStep_NoSavegame;*/
    }

    void GameState_KcetLogo_Update()
    {
        static auto nextGameState = GameState_Init;

        const auto& input  = g_App.GetInput();
        auto&       assets = g_App.GetAssets();

        switch (g_GameWork.gameStateSteps[0])
        {
            case KcetLogoStateStep_Init:
                ScreenFade_Start(true, true, false, Q12(1.0f));
                //Settings_RestoreDefaults();
                //GameFs_BgEtcGfxLoad();

                Game_StateStepIncrement(0);
                break;

            case KcetLogoStateStep_CheckMemCards:
                if (ScreenFade_IsNone() && !assets.IsBusy())
                {
                    g_GameWork.gameStateSteps[0] = GameState_KcetLogo_MemCardCheck();
                    //MemCard_Update();
                }
                break;

            case KcetLogoStateStep_NoSavegame:
                GameFs_TitleGfxLoad();
                nextGameState = GameState_MovieIntro;

                Game_StateStepSet(0, KcetLogoStateStep_LogoDelay);
                break;

            case KcetLogoStateStep_HasSavegame:
                while (g_GameWork.gameStateSteps[1] <= 2)
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
                                assets.Load("Psx/TIM/SAVELOAD.TIM");
                                nextGameState = GameState_AutoLoadSavegame;
                            }
                            else
                            {
                                GameFs_TitleGfxLoad();
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
                if (input.GetAction(In::Enter).IsHeld()  ||
                    input.GetAction(In::Cancel).IsHeld() ||
                    g_SysWork.gameStateCounter >= SEC_TO_TICK(3.0f))
                {
                    ScreenFade_Start(false, false, false, Q12(1.0f));

                    Game_StateStepIncrement(0);
                }
                break;

            case KcetLogoStateStep_FinishAfterFade:
                if (ScreenFade_IsFinished() && !assets.IsBusy())
                {
                    //Settings_ScreenAndVolUpdate();

                    Demo_SequenceAdvance(0);
                    Demo_DemoDataRead();
                    Game_StateSetNext(nextGameState);
                }
                break;
        }

        BootScreen_KcetScreenDraw();
        //Screen_FadeUpdate();
        //MemCard_Update();
        //func_80033548();
    }

    void BootScreen_KonamiScreenDraw()
    {
        Screen_BackgroundImgDraw("Psx/1ST/KONAMI.TIM", true, 0, Color::From8Bit(248, 248, 248));
    }

    void BootScreen_KcetScreenDraw()
    {
        Screen_BackgroundImgDraw("Psx/1ST/KONAMI2.TIM", true, 0, Color::From8Bit(248, 248, 248));
    }
}
