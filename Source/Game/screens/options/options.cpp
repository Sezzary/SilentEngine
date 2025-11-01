#include "Framework.h"
#include "Game/screens/options/options.h"

#include "Game/dummy.h"

namespace Silent::Game
{
    constexpr uint LINE_CURSOR_TIMER_MAX = 8;

    s32 g_Options_SelectionHighlightTimer = 0;

    int  g_MainOptionsMenu_SelectedEntry      = 0;
    int  g_ExtraOptionsMenu_SelectedEntry     = 0;
    int  g_MainOptionsMenu_PrevSelectedEntry  = 0;
    int  g_ExtraOptionsMenu_PrevSelectedEntry = 0;
    bool g_ScreenPosMenu_InvertBackgroundFade = false;

    Vector2i g_MainOptions_SelectionHighlightFrom  = Vector2i::Zero;
    Vector2i g_MainOptions_SelectionHighlightTo    = Vector2i::Zero;
    Vector2i g_ExtraOptions_SelectionHighlightFrom = Vector2i::Zero;
    Vector2i g_ExtraOptions_SelectionHighlightTo   = Vector2i::Zero;

    void GameState_Options_Update()
    {
        if (g_GameWork.gameStatePrev_590 == GameState_InGame)
        {
            //func_800363D0();
        }

        if (g_GameWork.gameStatePrev_590 != GameState_MainMenu)
        {
            //Game_TimerUpdate();
        }

        // Handle options menu state.
        int unlockedOptFlags = 0;
        switch (g_GameWork.gameStateStep_598[0])
        {
            case OptionsMenuState_EnterMainOptions:
                //DrawSync(SyncMode_Wait);

                if (g_GameWork.gameStatePrev_590 != GameState_InGame)
                {
                    //VSync(SyncMode_Wait8);
                }

                g_GameWork.background2dColor_R_58C = 0;
                g_GameWork.background2dColor_G_58D = 0;
                g_GameWork.background2dColor_B_58E = 0;

                //ScreenFade_Start(false, true, false);
                //g_IntervalVBlanks   = 1;

                if (g_GameWork.gameStatePrev_590 == GameState_InGame)
                {
                    //func_80037188();
                }

                //g_MainOptionsMenu_SelectedEntry      = MainOptionsMenuEntry_Exit;
                //g_MainOptionsMenu_PrevSelectedEntry  = 0;
                //g_ExtraOptionsMenu_SelectedEntry     = 0;
                //g_ExtraOptionsMenu_PrevSelectedEntry = 0;
                //g_Options_SelectionHighlightTimer    = 0;
                //g_ExtraOptionsMenu_BulletMultMax     = 1;
                unlockedOptFlags                     = g_GameWork.config_0.optExtraOptionsEnabled_27;
                
                // Set available bullet multiplier.
                for (int i = 0; i < 5; i++)
                {
                    if (unlockedOptFlags & (1 << i))
                    {
                        //g_ExtraOptionsMenu_BulletMultMax++;
                    }
                }

                // Set selected blood color.
                switch (g_GameWork.config_0.optExtraBloodColor_24)
                {
                    case BloodColor_Normal:
                        //g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Normal;
                        break;

                    case BloodColor_Green:
                        //g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Green;
                        break;

                    case BloodColor_Violet:
                        //g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Violet;
                        break;

                    case BloodColor_Black:
                        //g_ExtraOptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Black;
                        break;
                }

                //g_ExtraOptionsMenu_EntryCount   = (g_GameWork.config_0.optExtraOptionsEnabled_27) ? 8 : 6;
                g_GameWork.gameStateStep_598[0] = OptionsMenuState_MainOptions;
                g_SysWork.timer_20              = 0;
                g_GameWork.gameStateStep_598[1] = 0;
                g_GameWork.gameStateStep_598[2] = 0;
                break;

            case OptionsMenuState_LeaveScreenPos:
            case OptionsMenuState_LeaveBrightness:
            case OptionsMenuState_LeaveController:
                g_GameWork.gameStateStep_598[0] = OptionsMenuState_MainOptions;
                g_SysWork.timer_20              = 0;
                g_GameWork.gameStateStep_598[1] = 0;
                g_GameWork.gameStateStep_598[2] = 0;
                break;

            case OptionsMenuState_EnterScreenPos:
                if (false)//(ScreenFade_IsFinished())
                {
                    g_GameWork.gameStateStep_598[0] = OptionsMenuState_ScreenPos;
                    g_SysWork.timer_20              = 0;
                    g_GameWork.gameStateStep_598[1] = 0;
                    g_GameWork.gameStateStep_598[2] = 0;
                }
                break;

            case OptionsMenuState_ScreenPos:
                //Options_ScreenPosMenu_Control();
                break;

            case OptionsMenuState_EnterBrightness:
                if (false)//(ScreenFade_IsFinished())
                {
                    //Fs_QueueWaitForEmpty();

                    g_GameWork.gameStateStep_598[0] = OptionsMenuState_Brightness;
                    g_GameWork.gameStateStep_598[0] = OptionsMenuState_Brightness;
                    g_SysWork.timer_20              = 0;
                    g_GameWork.gameStateStep_598[1] = 0;
                    g_GameWork.gameStateStep_598[2] = 0;
                }
                break;

            case OptionsMenuState_Brightness:
                //Options_BrightnessMenu_Control();
                break;

            case OptionsMenuState_EnterController:
                // Switch to controller menu.
                if (false)//(ScreenFade_IsFinished())
                {
                    g_GameWork.gameStateStep_598[0] = OptionsMenuState_Controller;
                    g_GameWork.gameStateStep_598[0] = OptionsMenuState_Controller;
                    g_SysWork.timer_20              = 0;
                    g_GameWork.gameStateStep_598[1] = 0;
                    g_GameWork.gameStateStep_598[2] = 0;
                }
                break;

            case OptionsMenuState_Controller:
                //Options_ControllerMenu_Control();
                break;

            case OptionsMenuState_Leave:
                //ScreenFade_Start(true, false, false);

                g_GameWork.gameStateStep_598[0] = OptionsMenuState_LeaveMainOptions;
                g_SysWork.timer_20              = 0;
                g_GameWork.gameStateStep_598[1] = 0;
                g_GameWork.gameStateStep_598[2] = 0;
                break;

            case OptionsMenuState_LeaveMainOptions:
                if (false)//(ScreenFade_IsFinished())
                {
                    // TODO: Likely `Game_StateSetPrevious` inline, but `gameState_594`/`gameStatePrev_590` loads inside are switched?

                    auto prevGameState = g_GameWork.gameStatePrev_590;
                    auto gameState     = g_GameWork.gameState_594;

                    g_SysWork.timer_1C              = 0;
                    g_SysWork.timer_20              = 0;
                    g_GameWork.gameStateStep_598[1] = 0;
                    g_GameWork.gameStateStep_598[2] = 0;

                    //SysWork_StateSetNext(SysState_Gameplay);

                    g_GameWork.gameStateStep_598[0] = gameState;
                    g_GameWork.gameState_594        = prevGameState;
                    g_GameWork.gameStatePrev_590    = gameState;
                    g_GameWork.gameStateStep_598[0] = OptionsMenuState_EnterMainOptions;
                }
                break;

            case OptionsMenuState_EnterExtraOptions:
                if (false)//(ScreenFade_IsFinished())
                {
                    g_GameWork.gameStateStep_598[0]   = OptionsMenuState_ExtraOptions;
                    g_SysWork.timer_20                = 0;
                    //ScreenFade_Start(false, true, false);
                    g_GameWork.gameStateStep_598[1]   = 0;
                    g_GameWork.gameStateStep_598[2]   = 0;
                    //g_Options_SelectionHighlightTimer = 0;
                }
                break;

            case OptionsMenuState_LeaveExtraOptions:
                if (false)//(ScreenFade_IsFinished())
                {
                    g_GameWork.gameStateStep_598[0] = OptionsMenuState_EnterMainOptions;
                    g_SysWork.timer_20              = 0;
                    g_GameWork.gameStateStep_598[1] = 0;
                    g_GameWork.gameStateStep_598[2] = 0;
                    //ScreenFade_Start(false, true, false);
                }
                break;
        }

        // Handle menu state.
        switch (g_GameWork.gameStateStep_598[0])
        {
            case OptionsMenuState_MainOptions:
            case OptionsMenuState_Leave:
            case OptionsMenuState_LeaveMainOptions:
            case OptionsMenuState_EnterScreenPos:
            case OptionsMenuState_EnterBrightness:
            case OptionsMenuState_EnterController:
            case OptionsMenuState_EnterExtraOptions:
                //Options_MainOptionsMenu_Control();
                break;

            case OptionsMenuState_ExtraOptions:
            case OptionsMenuState_LeaveExtraOptions:
                //Options_ExtraOptionsMenu_Control();
                break;
        }
    }
/*
    void Options_ExtraOptionsMenu_Control()
    {
        //Options_ExtraOptionsMenu_EntryStringsDraw();
        //Options_ExtraOptionsMenu_ConfigDraw();
        //Options_ExtraOptionsMenu_SelectionHighlightDraw();
        //Options_Menu_VignetteDraw();
        //Gfx_BackgroundSpriteDraw(&g_ItemInspectionImg);

        if (g_GameWork.gameStateStep_598[0] != OptionsMenuState_ExtraOptions)
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
            if (g_GameWork.gameStatePrev_590 == GameState_InGame && 
                !(g_Controller0->btnsClicked_10 & g_GameWorkPtr->config_0.controllerConfig_0.enter_0) &&
                (g_Controller0->btnsClicked_10 & g_GameWorkPtr->config_0.controllerConfig_0.option_1A))
            {
                //Sd_PlaySfx(Sfx_Cancel, 0, 64);

                g_GameWork.gameStateStep_598[0] = OptionsMenuState_Leave;
                g_SysWork.timer_20              = 0;
                g_GameWork.gameStateStep_598[1] = 0;
                g_GameWork.gameStateStep_598[2] = 0;
                return;
            }

            // Move selection cursor up/down.
            if (g_Controller0->btnsPulsed_18 & ControllerFlag_LStickUp)
            {
                s32 var = 1;
                //Sd_PlaySfx(Sfx_Back, 0, 64);
                g_ExtraOptionsMenu_SelectedEntry  = ((g_ExtraOptionsMenu_SelectedEntry - var) + g_ExtraOptionsMenu_EntryCount) % g_ExtraOptionsMenu_EntryCount;
                g_Options_SelectionHighlightTimer = 0;
            }
            if (g_Controller0->btnsPulsed_18 & ControllerFlag_LStickDown)
            {
                Sd_PlaySfx(Sfx_Back, 0, 64);
                g_ExtraOptionsMenu_SelectedEntry++;
                g_ExtraOptionsMenu_SelectedEntry  = g_ExtraOptionsMenu_SelectedEntry % g_ExtraOptionsMenu_EntryCount;
                g_Options_SelectionHighlightTimer = 0;
            }

            // Handle config change.
            switch (g_ExtraOptionsMenu_SelectedEntry)
            {
                case ExtraOptionsMenuEntry_WeaponCtrl:
                    // Scroll left/right.
                    if (g_Controller0->btnsClicked_10 & (ControllerFlag_LStickRight | ControllerFlag_LStickLeft))
                    {
                        Sd_PlaySfx(Sfx_Back, 0, 64);
                        g_GameWork.config_0.optExtraWeaponCtrl_23 = !g_GameWork.config_0.optExtraWeaponCtrl_23;
                    }
                    break;

                case ExtraOptionsMenuEntry_Blood:
                    // Scroll left/right.
                    if (g_Controller0->btnsClicked_10 & ControllerFlag_LStickRight)
                    {
                        Sd_PlaySfx(Sfx_Back, 0, 64);
                        g_ExtraOptionsMenu_SelectedBloodColorEntry++;
                    }
                    if (g_Controller0->btnsClicked_10 & ControllerFlag_LStickLeft)
                    {
                        Sd_PlaySfx(Sfx_Back, 0, 64);
                        g_ExtraOptionsMenu_SelectedBloodColorEntry += 3;
                    }

                    // Set config.
                    g_ExtraOptionsMenu_SelectedBloodColorEntry = g_ExtraOptionsMenu_SelectedBloodColorEntry % BloodColorMenuEntry_Count;
                    switch (g_ExtraOptionsMenu_SelectedBloodColorEntry)
                    {
                        case BloodColorMenuEntry_Normal:
                            g_GameWork.config_0.optExtraBloodColor_24 = BloodColor_Normal;
                            break;

                        case BloodColorMenuEntry_Green:
                            g_GameWork.config_0.optExtraBloodColor_24 = BloodColor_Green;
                            break;

                        case BloodColorMenuEntry_Violet:
                            g_GameWork.config_0.optExtraBloodColor_24 = BloodColor_Violet;
                            break;

                        case BloodColorMenuEntry_Black:
                            g_GameWork.config_0.optExtraBloodColor_24 = BloodColor_Black;
                            break;
                    }
                    break;

                case ExtraOptionsMenuEntry_ViewCtrl:
                    // Scroll left/right.
                    if (g_Controller0->btnsClicked_10 & (ControllerFlag_LStickRight | ControllerFlag_LStickLeft))
                    {
                        Sd_PlaySfx(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config_0.optExtraViewCtrl_28 = !g_GameWork.config_0.optExtraViewCtrl_28;
                    }
                    break;

                case ExtraOptionsMenuEntry_RetreatTurn:
                    // Scroll left/right.
                    if (g_Controller0->btnsClicked_10 & (ControllerFlag_LStickRight | ControllerFlag_LStickLeft))
                    {
                        Sd_PlaySfx(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config_0.optExtraRetreatTurn_2A = (s8)g_GameWork.config_0.optExtraRetreatTurn_2A == 0;
                    }
                    break;

                case ExtraOptionsMenuEntry_MovementCtrl:
                    // Scroll left/right.
                    if (g_Controller0->btnsClicked_10 & (ControllerFlag_LStickRight | ControllerFlag_LStickLeft))
                    {
                        Sd_PlaySfx(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config_0.optExtraWalkRunCtrl_2B = (s8)g_GameWork.config_0.optExtraWalkRunCtrl_2B == 0;
                    }
                    break;

                case ExtraOptionsMenuEntry_AutoAiming:
                    // Scroll left/right.
                    if (g_Controller0->btnsClicked_10 & (ControllerFlag_LStickRight | ControllerFlag_LStickLeft))
                    {
                        Sd_PlaySfx(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config_0.optExtraAutoAiming_2C = (s8)g_GameWork.config_0.optExtraAutoAiming_2C == 0;
                    }
                    break;

                case ExtraOptionsMenuEntry_ViewMode:
                    // Scroll left/right.
                    if (g_Controller0->btnsClicked_10 & (ControllerFlag_LStickRight | ControllerFlag_LStickLeft))
                    {
                        Sd_PlaySfx(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config_0.optExtraViewMode_29 = !g_GameWork.config_0.optExtraViewMode_29;
                    }
                    break;

                case ExtraOptionsMenuEntry_BulletMult:
                    // Scroll left/right.
                    if (g_Controller0->btnsClicked_10 & ControllerFlag_LStickRight)
                    {
                        Sd_PlaySfx(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config_0.optExtraBulletAdjust_2D++;
                    }
                    if (g_Controller0->btnsClicked_10 & ControllerFlag_LStickLeft)
                    {
                        Sd_PlaySfx(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config_0.optExtraBulletAdjust_2D = g_GameWork.config_0.optExtraBulletAdjust_2D + (g_ExtraOptionsMenu_BulletMultMax - 1);
                    }
                    g_GameWork.config_0.optExtraBulletAdjust_2D = g_GameWork.config_0.optExtraBulletAdjust_2D % g_ExtraOptionsMenu_BulletMultMax;
                    break;
            }
        }

        // Leave menu.
        if ((g_Controller0->btnsClicked_10 & (g_GameWorkPtr->config_0.controllerConfig_0.cancel_2 |
                                            (ControllerFlag_L2 | ControllerFlag_R2 | ControllerFlag_L1 | ControllerFlag_R1))) &&
            g_GameWork.gameStateStep_598[0] != OptionsMenuState_LeaveExtraOptions)
        {
            if (g_Controller0->btnsClicked_10 & g_GameWorkPtr->config_0.controllerConfig_0.cancel_2)
            {
                Sd_PlaySfx(Sfx_Cancel, 0, 64);
            }
            else
            {
                Sd_PlaySfx(Sfx_Confirm, 0, 64);
            }

            ScreenFade_Start(true, false, false);
            g_GameWork.gameStateStep_598[0] = OptionsMenuState_LeaveExtraOptions;
            g_SysWork.timer_20              = 0;
            g_GameWork.gameStateStep_598[1] = 0;
            g_GameWork.gameStateStep_598[2] = 0;
        }
    }*/
}
