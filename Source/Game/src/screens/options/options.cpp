#include "Framework.h"
#include "Game/include/screens/options/options.h"

namespace Silent::Game
{
    /** @brief State IDs used by the main game loop. The values are used as indices into the 0x800A977C function array. */
    enum e_GameState
    {
        GameState_Init                = 0,
        GameState_KonamiLogo          = 1,
        GameState_KcetLogo            = 2,
        GameState_StartMovieIntro     = 3,
        GameState_DeathLoadScreen     = 4,
        GameState_MovieIntroAlternate = 5,
        GameState_MovieIntro          = 6,
        GameState_MainMenu            = 7,
        GameState_SaveScreen          = 8,
        GameState_MovieOpening        = 9,
        GameState_MainLoadScreen      = 10,
        GameState_InGame              = 11,
        GameState_MapEvent            = 12,
        GameState_ExitMovie           = 13,
        GameState_InventoryScreen     = 14,
        GameState_MapScreen           = 15,
        GameState_Unk10               = 16,
        GameState_DebugMoviePlayer    = 17,
        GameState_OptionScreen        = 18,
        GameState_LoadStatusScreen    = 19,
        GameState_LoadMapScreen       = 20,
        GameState_Unk15               = 21,
        GameState_Unk16               = 22, /** Removed debug menu? Doesn't exist in function array, but `DebugMoviePlayer` state tries to switch to it. */
    };

    /** @brief Controller key bindings for input actions. Bontains bitfield of button presses assigned to each action.
     *
     * Bitfields only contain buttons. Analog directions and D-Pad aren't included.
     */
    struct s_ControllerConfig
    {
        ushort enter_0;
        ushort cancel_2;
        ushort skip_4;
        ushort action_6;
        ushort aim_8;
        ushort light_A;
        ushort run_C;
        ushort view_E;
        ushort stepLeft_10;
        ushort stepRight_12;
        ushort pause_14;
        ushort item_16;
        ushort map_18;
        ushort option_1A;
    };

    struct s_SaveUserConfig
    {
        s_ControllerConfig controllerConfig_0;
        int                optScreenPosX_1C;          /** Range: [-11, 11], default: 0. */
        int                optScreenPosY_1D;          /** Range: [-8, 8], default: 0. */
        bool               optSoundType_1E;           /** Stereo: `false`, Monaural: `true`, default: Stereo. */
        int                optVolumeBgm_1F;           /** Range: [0, 128] with steps of 8, default: 16. */
        int                optVolumeSe_20;            /** Range: [0, 128] with steps of 8, default: 16. */
        bool               optVibrationEnabled_21;    /** Off: 0, On: 128, default: On. */
        int                optBrightness_22;          /** Range: [0, 7], default: 3. */
        bool               optExtraWeaponCtrl_23;     /** Switch: `false`, Press: `true`, default: Press. */
        int                optExtraBloodColor_24;     /** `e_BloodColor` | Default: Normal. */
        bool               optAutoLoad_25;            /** Off: `false`, On: `true`, default: Off. */
        int                optExtraOptionsEnabled_27; /** Holds unlocked option flags. */
        bool               optExtraViewCtrl_28;       /** Normal: `false`, Reverse: `true`, default: Normal. */
        bool               optExtraViewMode_29;       /** Normal: `false`, Self View: `true`, default: Normal. */
        bool               optExtraRetreatTurn_2A;    /** Normal: `false`, Reverse: `true`, default: Normal. */
        bool               optExtraWalkRunCtrl_2B;    /** Normal: `false`, Reverse: `true`, default: Normal. */
        bool               optExtraAutoAiming_2C;     /** On: `false`, Off: `true`, default: On. */
        bool               optExtraBulletAdjust_2D;   /** x1-x6: Range [0, 5], default: x1. */
        u16                seenGameOverTips_2E[1];    /** Bitfield tracking seen game-over tips. Each bit corresponds to a tip index (0–14), set bits indicate seen tips. Resets after picking all 15. */
        s8                 unk_30[4];
        u32                palLanguageId_34;
    };

    struct DummyGameWork
    {
        s_SaveUserConfig config_0;
        uchar            background2dColor_R_58C;
        uchar            background2dColor_G_58D;
        uchar            background2dColor_B_58E;
        e_GameState      gameStatePrev_590;
        e_GameState      gameState_594;
        int              gameStateStep_598[3];
    };

    struct DummySysWork
    {
        int timer_1C;
        int timer_20;
    };

    auto g_GameWork = DummyGameWork{};
    auto g_SysWork  = DummySysWork{};

    void GameState_Options_Update(void) // 0x801E2D44
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
                int unlockedOptFlags                 = g_GameWork.config_0.optExtraOptionsEnabled_27;
                
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
}
