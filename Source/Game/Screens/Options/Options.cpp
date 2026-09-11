#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Options/Options.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Main/FsQueue.h"
#include "Game/Screens/Options/MenuGraphics.h"
#include "Input/Input.h"

using namespace Silent::Input;

namespace Silent::Game
{
    constexpr int LINE_CURSOR_TIMER_MAX = 8;

    int g_Options_SelectionHighlightTimer    = 0;
    int g_MainOptionsMenu_SelectedEntry      = 0;
    int g_MainOptionsMenu_PrevSelectedEntry  = 0;
    int g_ExtraOptionsMenu_SelectedEntry     = 0;
    int g_ExtraOptionsMenu_PrevSelectedEntry = 0;

    static int g_ExtraOptionsMenu_EntryCount              = 0;
    static int g_ExtraOptionsMenu_SelectedBloodColorEntry = 0;
    static int g_ExtraOptionsMenu_BulletMultMax           = 0;

    void GameState_Options_Update()
    {
        if (g_GameWork.gameStatePrev == GameState_InGame)
        {
            //Bgm_MenuUpdate();
        }

        if (g_GameWork.gameStatePrev != GameState_MainMenu)
        {
            //Game_TimerUpdate();
        }

        // Handle options menu state.
        int unlockedOptFlags = 0;
        switch (g_GameWork.gameStateSteps[0])
        {
            case OptionsMenuState_EnterMainOptions:
                //DrawSync(SyncMode_Wait);

                if (g_GameWork.gameStatePrev != GameState_InGame)
                {
                    //VSync(SyncMode_Wait8);
                }

                g_GameWork.background2dColor.r = 0;
                g_GameWork.background2dColor.g = 0;
                g_GameWork.background2dColor.b = 0;

                ScreenFade_Start(false, true, false);
                //g_IntervalVBlanks = 1;

                if (g_GameWork.gameStatePrev == GameState_InGame)
                {
                    //Game_RadioSoundStop();
                }

                g_MainOptionsMenu_SelectedEntry      = MainOptionsMenuEntry_Exit;
                g_MainOptionsMenu_PrevSelectedEntry  = 0;
                g_ExtraOptionsMenu_SelectedEntry     = 0;
                g_ExtraOptionsMenu_PrevSelectedEntry = 0;
                g_Options_SelectionHighlightTimer    = 0;
                g_ExtraOptionsMenu_BulletMultMax     = 1;
                unlockedOptFlags                     = g_GameWork.config.extraOptionsEnabled;
                
                // Set available bullet multiplier.
                for (int i = 0; i < 5; i++)
                {
                    if (unlockedOptFlags & (1 << i))
                    {
                        g_ExtraOptionsMenu_BulletMultMax++;
                    }
                }

                // Set selected blood color.
                switch (g_GameWork.config.extraBloodColor)
                {
                    case BloodColor_Normal:
                        g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Normal;
                        break;

                    case BloodColor_Green:
                        g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Green;
                        break;

                    case BloodColor_Violet:
                        g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Violet;
                        break;

                    case BloodColor_Black:
                        g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Black;
                        break;
                }

                g_ExtraOptionsMenu_EntryCount = (g_GameWork.config.extraOptionsEnabled) ? ExtraOptionsMenuEntry_Count :
                                                                                          (ExtraOptionsMenuEntry_Count - 2);
                Game_StateStepSet(0, OptionsMenuState_MainOptions);
                break;

            case OptionsMenuState_LeaveBrightness:
            case OptionsMenuState_LeaveController:
                Game_StateStepSet(0, OptionsMenuState_MainOptions);
                break;

            case OptionsMenuState_EnterBrightness:
                if (ScreenFade_IsFinished())
                {
                    Fs_QueueWaitForEmpty();
                    Game_StateStepSet(0, OptionsMenuState_Brightness);
                }
                break;

            case OptionsMenuState_Brightness:
                Options_BrightnessMenu_Control();
                break;

            case OptionsMenuState_EnterController:
                // Switch to controller menu.
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_Controller);
                }
                break;

            case OptionsMenuState_Controller:
                //Options_ControllerMenu_Control();
                break;

            case OptionsMenuState_Leave:
                ScreenFade_Start(true, false, false);
                Game_StateStepSet(0, OptionsMenuState_LeaveMainOptions);
                break;

            case OptionsMenuState_LeaveMainOptions:
                if (ScreenFade_IsFinished())
                {
                    // TODO: `Game_StateSetPrevious` won't work here? (also tried macro version of it)
                    auto prevGameState = g_GameWork.gameStatePrev;
                    Game_StateSetNext(prevGameState);
                }
                break;

            case OptionsMenuState_EnterExtraOptions:
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_ExtraOptions);
                }
                break;

            case OptionsMenuState_LeaveExtraOptions:
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_EnterMainOptions);
                    ScreenFade_Start(false, true, false);
                }
                break;
        }

        // Handle menu state.
        switch (g_GameWork.gameStateSteps[0])
        {
            case OptionsMenuState_MainOptions:
            case OptionsMenuState_Leave:
            case OptionsMenuState_LeaveMainOptions:
            case OptionsMenuState_EnterBrightness:
            case OptionsMenuState_EnterController:
            case OptionsMenuState_EnterExtraOptions:
                Options_MainOptionsMenu_Control();
                break;

            case OptionsMenuState_ExtraOptions:
            case OptionsMenuState_LeaveExtraOptions:
                Options_ExtraOptionsMenu_Control();
                break;
        }
    }

    void Options_MainOptionsMenu_Control()
    {
        constexpr int SOUND_VOL_STEP = 8;

        const auto& input = g_App.GetInput();

        // Draw graphics.
        Options_MainOptionsMenu_EntryStringsDraw();
        Options_MainOptionsMenu_ConfigDraw();
        Options_MainOptionsMenu_SelectionHighlightDraw();
        Options_Menu_VignetteDraw();
        //Screen_BackgroundImgDraw(&g_ItemInspectionImg);
        Options_MainOptionsMenu_BgmVolumeBarDraw();
        Options_MainOptionsMenu_SfxVolumeBarDraw();

        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_MainOptions)
        {
            return;
        }

        // Increment line move timer.
        if ((LINE_CURSOR_TIMER_MAX - 1) < g_Options_SelectionHighlightTimer)
        {
            g_Options_SelectionHighlightTimer = LINE_CURSOR_TIMER_MAX;
        }
        else
        {
            g_Options_SelectionHighlightTimer++;
        }

        if (g_Options_SelectionHighlightTimer != LINE_CURSOR_TIMER_MAX)
        {
            return;
        }

        g_MainOptionsMenu_PrevSelectedEntry = g_MainOptionsMenu_SelectedEntry;

        // Leave to gameplay (if options menu was accessed with `Option` input action).
        if (g_GameWork.gameStatePrev == GameState_InGame &&
            !input.GetAction(In::Enter).IsClicked() && input.GetAction(In::Option).IsClicked())
        {
            //Sd_SfxPlay(Sfx_MenuCancel, 0, 64);
            Game_StateStepSet(0, OptionsMenuState_Leave);
            return;
        }

        // @todo Accurate pulse delay.
        // Move selection cursor up/down.
        if (input.GetAction(In::Up).IsPulsed(0.2f, 0.4f))
        {
            //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

            g_Options_SelectionHighlightTimer = 0;
            g_MainOptionsMenu_SelectedEntry   = (g_MainOptionsMenu_SelectedEntry + (MainOptionsMenuEntry_Count - 1)) % MainOptionsMenuEntry_Count;
        }
        if (input.GetAction(In::Down).IsPulsed(0.2f, 0.4f))
        {
            //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

            g_Options_SelectionHighlightTimer = 0;
            g_MainOptionsMenu_SelectedEntry   = (g_MainOptionsMenu_SelectedEntry + 1) % MainOptionsMenuEntry_Count;
        }

        // Handle config change.
        int vol = 0;
        switch (g_MainOptionsMenu_SelectedEntry)
        {
            case MainOptionsMenuEntry_Exit:
                // Exit menu to gameplay.
                if (input.GetAction(In::Enter).IsClicked() || input.GetAction(In::Cancel).IsClicked())
                {
                    //Sd_SfxPlay(Sfx_MenuCancel, 0, 64);
                    Game_StateStepSet(0, OptionsMenuState_Leave);
                }
                break;

            case MainOptionsMenuEntry_Controller:
                // Enter controller screen.
                if (input.GetAction(In::Enter).IsClicked())
                {
                    //Sd_SfxPlay(Sfx_MenuConfirm, 0, 64);
                    Fs_QueueStartReadTim(FILE_TIM_OPTION2_TIM, IMAGE_BUFFER_3, &g_ControllerButtonAtlasImg);
                    ScreenFade_Start(true, false, false);
                    Game_StateStepSet(0, OptionsMenuState_EnterController);
                }
                break;


            case MainOptionsMenuEntry_Brightness:
                if (input.GetAction(In::Enter).IsClicked())
                {
                    //Sd_SfxPlay(Sfx_MenuConfirm, 0, 64);
                    if (g_GameWork.gameStatePrev == GameState_MainMenu)
                    {
                        Fs_QueueStartReadTim(FILE_TIM_OP_BRT_E_TIM, IMAGE_BUFFER_3, &g_BrightnessScreenImg0);
                    }
                    else
                    {
                        Fs_QueueStartReadTim(FILE_TIM_OP_BRT_E_TIM, IMAGE_BUFFER_3, &g_BrightnessScreenImg1);
                    }

                    ScreenFade_Start(true, false, false);
                    Game_StateStepSet(0, OptionsMenuState_EnterBrightness);
                }
                break;

            case MainOptionsMenuEntry_Vibration:
                if (input.GetAction(In::Left).IsClicked(0.5f) || input.GetAction(In::Right).IsClicked(0.5f))
                {
                    //Sd_SfxPlay(Sfx_MenuMove, 0, 64);
                    g_GameWork.config.vibrationEnabled = !g_GameWork.config.vibrationEnabled << 7;
                }
                break;

            case MainOptionsMenuEntry_AutoLoad:
                if (input.GetAction(In::Left).IsClicked(0.5f) || input.GetAction(In::Right).IsClicked(0.5f))
                {
                    //Sd_SfxPlay(Sfx_MenuMove, 0, 64);
                    g_GameWork.config.autoLoad = (s8)g_GameWork.config.autoLoad == 0;
                }
                break;

            case MainOptionsMenuEntry_Sound:
                if (input.GetAction(In::Left).IsClicked(0.5f) || input.GetAction(In::Right).IsClicked(0.5f))
                {
                    //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                    // Set config.
                    /*int audioType                           = AudioMode_Stereo;
                    g_GameWork.config.soundType = !g_GameWork.config.soundType;
                    if (g_GameWork.config.soundType)
                    {
                        audioType = AudioMode_Mono;
                    }
                    Sd_EngineCmd(audioType);*/
                }
                break;

            case MainOptionsMenuEntry_BgmVolume:
                vol = g_GameWork.config.volumeBgm;

                /*if ((vol < OPT_SOUND_VOLUME_MAX && input.GetAction(In::Right).IsPulsed(0.2f, 0.4f)) ||
                    (vol > 0                    && input.GetAction(In::Left).IsPulsed(0.2f, 0.4f)))
                {
                    Sd_EngineCmd(Sfx_MenuMove);
                }
                if ((vol == OPT_SOUND_VOLUME_MAX && input.GetAction(In::Right).IsClicked()) ||
                    (vol == 0                    && input.GetAction(In::Left).IsClicked()))
                {
                    Sd_EngineCmd(Sfx_MenuError);
                }*/

                // Scroll left/right.
                if (input.GetAction(In::Left).IsPulsed(0.2f, 0.4f))
                {
                    vol = vol - SOUND_VOL_STEP;
                }
                if (input.GetAction(In::Right).IsPulsed(0.2f, 0.4f))
                {
                    vol = vol + SOUND_VOL_STEP;
                }

                // Set config.
                //vol = CLAMP(vol, 0, OPT_SOUND_VOLUME_MAX);
                //Sd_SetVolume(OPT_SOUND_VOLUME_MAX, vol, g_GameWork.config.volumeSe);
                g_GameWork.config.volumeBgm = vol;
                break;

            case MainOptionsMenuEntry_SfxVolume:
                vol = g_GameWork.config.volumeSe;

                /*if ((vol < OPT_SOUND_VOLUME_MAX && input.GetAction(In::Right).IsPulsed(0.2f, 0.4f)) ||
                    (vol > 0                    && input.GetAction(In::Left).IsPulsed(0.2f, 0.4f)))
                {
                    Sd_EngineCmd(Sfx_MenuMove);
                }
                if ((vol == OPT_SOUND_VOLUME_MAX && input.GetAction(In::Right).IsClicked()) ||
                    (vol == 0                    && input.GetAction(In::Left).IsClicked()))
                {
                    Sd_EngineCmd(Sfx_MenuError);
                }*/

                if (input.GetAction(In::Left).IsPulsed(0.2f, 0.4f))
                {
                    vol = vol - SOUND_VOL_STEP;
                }
                if (input.GetAction(In::Right).IsPulsed(0.2f, 0.4f))
                {
                    vol = vol + SOUND_VOL_STEP;
                }

                /*vol = CLAMP(vol, 0, OPT_SOUND_VOLUME_MAX);

                Sd_SetVolume(OPT_SOUND_VOLUME_MAX, vol, g_GameWork.config.volumeSe);*/
                g_GameWork.config.volumeSe = vol;
                break;

            default:
                break;
        }

        if (input.GetAction(In::StepLeft).IsClicked() || input.GetAction(In::StepRight).IsClicked())
        {
            if (g_GameWork.gameStateSteps[0] == OptionsMenuState_EnterExtraOptions)
            {
                return;
            }

            //Sd_SfxPlay(Sfx_MenuConfirm, 0, 64);

            ScreenFade_Start(true, false, false);
            g_GameWork.gameStateSteps[0] = OptionsMenuState_EnterExtraOptions;
            g_SysWork.counters_1C[1]              = 0;
            g_GameWork.gameStateSteps[1] = 0;
            g_GameWork.gameStateSteps[2] = 0;
        }

        // Reset selection cursor.
        if (((g_GameWork.gameStateSteps[0] != OptionsMenuState_EnterExtraOptions &&
              g_MainOptionsMenu_SelectedEntry != MainOptionsMenuEntry_Exit) &&
            !input.GetAction(In::Enter).IsClicked()) &&
            input.GetAction(In::Cancel).IsClicked())
        {
            //Sd_SfxPlay(Sfx_MenuCancel, 0, 64);

            g_Options_SelectionHighlightTimer = 0;
            g_MainOptionsMenu_SelectedEntry   = MainOptionsMenuEntry_Exit;
        }
    }

    void Options_ExtraOptionsMenu_Control()
    {
/*
        //Options_ExtraOptionsMenu_EntryStringsDraw();
        //Options_ExtraOptionsMenu_ConfigDraw();
        //Options_ExtraOptionsMenu_SelectionHighlightDraw();
        //Options_Menu_VignetteDraw();
        //Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_ExtraOptions)
        {
            return;
        }

        // Increment line move timer.
        if ((LINE_CURSOR_TIMER_MAX - 1) < g_Options_SelectionHighlightTimer)
        {
            g_Options_SelectionHighlightTimer = LINE_CURSOR_TIMER_MAX;
        }
        else
        {
            g_Options_SelectionHighlightTimer++;
        }

        if (g_Options_SelectionHighlightTimer == LINE_CURSOR_TIMER_MAX)
        {
            g_ExtraOptionsMenu_PrevSelectedEntry = g_ExtraOptionsMenu_SelectedEntry;

            // Leave to gameplay (if options menu was accessed with `Option` input action).
            if (g_GameWork.gameStatePrev == GameState_InGame && 
                !(g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter) &&
                (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.option))
            {
                //Sd_SfxPlay(Sfx_Cancel, 0, 64);
                Game_StateStepSet(0, OptionsMenuState_Leave);
                return;
            }

            // Move selection cursor up/down.
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighUp)
            {
                s32 var = 1;
                //Sd_SfxPlay(Sfx_Back, 0, 64);
                g_ExtraOptionsMenu_SelectedEntry  = ((g_ExtraOptionsMenu_SelectedEntry - var) + g_ExtraOptionsMenu_EntryCount) % g_ExtraOptionsMenu_EntryCount;
                g_Options_SelectionHighlightTimer = 0;
            }
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighDown)
            {
                Sd_SfxPlay(Sfx_Back, 0, 64);
                g_ExtraOptionsMenu_SelectedEntry++;
                g_ExtraOptionsMenu_SelectedEntry  = g_ExtraOptionsMenu_SelectedEntry % g_ExtraOptionsMenu_EntryCount;
                g_Options_SelectionHighlightTimer = 0;
            }

            // Handle config change.
            switch (g_ExtraOptionsMenu_SelectedEntry)
            {
                case ExtraOptionsMenuEntry_WeaponCtrl:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);
                        g_GameWork.config.extraWeaponCtrl = !g_GameWork.config.extraWeaponCtrl;
                    }
                    break;

                case ExtraOptionsMenuEntry_Blood:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighRight)
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);
                        g_ExtraOptionsMenu_SelectedBloodColorEntry++;
                    }
                    if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighLeft)
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);
                        g_ExtraOptionsMenu_SelectedBloodColorEntry += 3;
                    }

                    // Set config.
                    g_ExtraOptionsMenu_SelectedBloodColorEntry = g_ExtraOptionsMenu_SelectedBloodColorEntry % BloodColorMenuEntry_Count;
                    switch (g_ExtraOptionsMenu_SelectedBloodColorEntry)
                    {
                        case BloodColorMenuEntry_Normal:
                            g_GameWork.config.extraBloodColor = BloodColor_Normal;
                            break;

                        case BloodColorMenuEntry_Green:
                            g_GameWork.config.extraBloodColor = BloodColor_Green;
                            break;

                        case BloodColorMenuEntry_Violet:
                            g_GameWork.config.extraBloodColor = BloodColor_Violet;
                            break;

                        case BloodColorMenuEntry_Black:
                            g_GameWork.config.extraBloodColor = BloodColor_Black;
                            break;
                    }
                    break;

                case ExtraOptionsMenuEntry_ViewCtrl:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraViewCtrl = !g_GameWork.config.extraViewCtrl;
                    }
                    break;

                case ExtraOptionsMenuEntry_RetreatTurn:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraRetreatTurn = (s8)g_GameWork.config.extraRetreatTurn == 0;
                    }
                    break;

                case ExtraOptionsMenuEntry_MovementCtrl:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraWalkRunCtrl = (s8)g_GameWork.config.extraWalkRunCtrl == 0;
                    }
                    break;

                case ExtraOptionsMenuEntry_AutoAiming:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraAutoAiming = (s8)g_GameWork.config.extraAutoAiming == 0;
                    }
                    break;

                case ExtraOptionsMenuEntry_ViewMode:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraViewMode = !g_GameWork.config.extraViewMode;
                    }
                    break;

                case ExtraOptionsMenuEntry_BulletMult:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighRight)
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraBulletAdjust++;
                    }
                    if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighLeft)
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraBulletAdjust = g_GameWork.config.extraBulletAdjust + (g_ExtraOptionsMenu_BulletMultMax - 1);
                    }
                    g_GameWork.config.extraBulletAdjust = g_GameWork.config.extraBulletAdjust % g_ExtraOptionsMenu_BulletMultMax;
                    break;
            }
        }

        // Leave menu.
        if ((g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.cance |
                                            (ControllerFlag_L2 | ControllerFlag_R2 | ControllerFlag_L1 | ControllerFlag_R1))) &&
            g_GameWork.gameStateSteps[0] != OptionsMenuState_LeaveExtraOptions)
        {
            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
            {
                Sd_SfxPlay(Sfx_Cancel, 0, 64);
            }
            else
            {
                Sd_SfxPlay(Sfx_Confirm, 0, 64);
            }

            ScreenFade_Start(true, false, false);
            Game_StateStepSet(0, OptionsMenuState_LeaveExtraOptions);
        }
*/
    }
}
