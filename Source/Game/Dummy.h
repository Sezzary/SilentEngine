#pragma once

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

    typedef enum e_ControllerFlags
    {
        ControllerFlag_None         = 0,
        ControllerFlag_Select       = 1 << 0,
        ControllerFlag_L3           = 1 << 1,
        ControllerFlag_R3           = 1 << 2,
        ControllerFlag_Start        = 1 << 3,
        ControllerFlag_DpadUp       = 1 << 4,
        ControllerFlag_DpadRight    = 1 << 5,
        ControllerFlag_DpadDown     = 1 << 6,
        ControllerFlag_DpadLeft     = 1 << 7,
        ControllerFlag_L2           = 1 << 8,
        ControllerFlag_R2           = 1 << 9,
        ControllerFlag_L1           = 1 << 10,
        ControllerFlag_R1           = 1 << 11,
        ControllerFlag_Triangle     = 1 << 12,
        ControllerFlag_Circle       = 1 << 13,
        ControllerFlag_Cross        = 1 << 14,
        ControllerFlag_Square       = 1 << 15,
        ControllerFlag_LStickUp2    = 1 << 16,
        ControllerFlag_LStickRight2 = 1 << 17,
        ControllerFlag_LStickDown2  = 1 << 18,
        ControllerFlag_LStickLeft2  = 1 << 19,
        ControllerFlag_RStickUp     = 1 << 20,
        ControllerFlag_RStickRight  = 1 << 21,
        ControllerFlag_RStickDown   = 1 << 22,
        ControllerFlag_RStickLeft   = 1 << 23,
        ControllerFlag_LStickUp     = 1 << 24,
        ControllerFlag_LStickRight  = 1 << 25,
        ControllerFlag_LStickDown   = 1 << 26,
        ControllerFlag_LStickLeft   = 1 << 27
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

    union s_AnalogSticks
    {
        u32 rawData_0;
        struct
        {
            s8 rightX;
            s8 rightY;
            s8 leftX;
            s8 leftY;
        } sticks_0; // Range is `[-128, 127]`?
    };

    struct s_AnalogController
    {
        u8  status;
        u8  received_bytes : 4; /** Number of bytes received / 2. */
        u8  terminal_type  : 4; /** `e_PadTerminalType` */
        u16 digitalButtons;
        u8  rightX;
        u8  rightY;
        u8  leftX;
        u8  leftY;
    };

    struct s_ControllerData
    {
        s_AnalogController analogController_0;
        s32                pulseTicks_8;
        e_ControllerFlags  btnsHeld_C;
        e_ControllerFlags  btnsClicked_10;
        e_ControllerFlags  btnsReleased_14;
        e_ControllerFlags  btnsPulsed_18;
        e_ControllerFlags  btnsPulsedGui_1C;
        s_AnalogSticks     sticks_20;
        s_AnalogSticks     sticks_24;
        s32                field_28;
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

    extern DummyGameWork    g_GameWork;
    extern DummySysWork     g_SysWork;
    extern s_ControllerData g_Controller0;

    extern int g_Screen_FadeStatus;
}
