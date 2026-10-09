#pragma once

namespace Silent::Game
{
    constexpr int  INPUT_ACTION_COUNT   = 14;
    constexpr int  CONTROLLER_COUNT_MAX = 2;
    constexpr q0_7 STICK_DEADZONE       = FP_STICK(0.5f);

    /** @brief PSX controller input flags. */
    enum e_ControllerFlags
    {
        ControllerFlag_None            = 0,
        ControllerFlag_Select          = 1 << 0,
        ControllerFlag_L3              = 1 << 1,
        ControllerFlag_R3              = 1 << 2,
        ControllerFlag_Start           = 1 << 3,
        ControllerFlag_DpadUp          = 1 << 4,
        ControllerFlag_DpadRight       = 1 << 5,
        ControllerFlag_DpadDown        = 1 << 6,
        ControllerFlag_DpadLeft        = 1 << 7,
        ControllerFlag_L2              = 1 << 8,
        ControllerFlag_R2              = 1 << 9,
        ControllerFlag_L1              = 1 << 10,
        ControllerFlag_R1              = 1 << 11,
        ControllerFlag_Triangle        = 1 << 12,
        ControllerFlag_Circle          = 1 << 13,
        ControllerFlag_Cross           = 1 << 14,
        ControllerFlag_Square          = 1 << 15,
        ControllerFlag_LStickLowUp     = 1 << 16,
        ControllerFlag_LStickLowRight  = 1 << 17,
        ControllerFlag_LStickLowDown   = 1 << 18,
        ControllerFlag_LStickLowLeft   = 1 << 19,
        ControllerFlag_RStickLowUp     = 1 << 20,
        ControllerFlag_RStickLowRight  = 1 << 21,
        ControllerFlag_RStickLowDown   = 1 << 22,
        ControllerFlag_RStickLowLeft   = 1 << 23,
        ControllerFlag_LStickHighUp    = 1 << 24,
        ControllerFlag_LStickHighRight = 1 << 25,
        ControllerFlag_LStickHighDown  = 1 << 26,
        ControllerFlag_LStickHighLeft  = 1 << 27,
        ControllerFlag_RStickHighUp    = 1 << 28,
        ControllerFlag_RStickHighRight = 1 << 29,
        ControllerFlag_RStickHighDown  = 1 << 30,
        ControllerFlag_RStickHighLeft  = 1 << 31,

        ControllerFlag_FaceButtons = ControllerFlag_Select | ControllerFlag_L3 | ControllerFlag_R3 | ControllerFlag_Start |
                                     ControllerFlag_DpadUp | ControllerFlag_DpadRight | ControllerFlag_DpadDown | ControllerFlag_DpadLeft |
                                     ControllerFlag_L2 | ControllerFlag_R2 | ControllerFlag_L1 | ControllerFlag_R1 |
                                     ControllerFlag_Triangle | ControllerFlag_Circle | ControllerFlag_Cross | ControllerFlag_Square,
        ControllerFlag_LowSticks   = ControllerFlag_LStickLowUp | ControllerFlag_LStickLowRight | ControllerFlag_LStickLowDown | ControllerFlag_LStickLowLeft |
                                     ControllerFlag_RStickLowUp | ControllerFlag_RStickLowRight | ControllerFlag_RStickLowDown | ControllerFlag_RStickLowLeft,
        ControllerFlag_HighSticks  = ControllerFlag_LStickHighUp | ControllerFlag_LStickHighRight | ControllerFlag_LStickHighDown | ControllerFlag_LStickHighLeft |
                                     ControllerFlag_RStickHighUp | ControllerFlag_RStickHighRight | ControllerFlag_RStickHighDown | ControllerFlag_RStickHighLeft,
    };

    typedef union
    {
        u32 rawData_0;
        struct
        {
            s8 rightX;
            s8 rightY;
            s8 leftX;
            s8 leftY;
        } sticks_0; // Normalized range: `[-128, 127]`.
    } s_AnalogSticks;

    /** @brief Analog PSX controller state. */
    struct s_AnalogController
    {
        u8  status;
        u8  receivedBytes : 4; /** Number of bytes received / 2. */
        u8  terminalType  : 4; /** `e_PadTerminalType` */
        u16 digitalButtons;    /** `e_ControllerFlags` */
        u8  rightX;
        u8  rightY;
        u8  leftX;
        u8  leftY;
    };

    /** @brief PSX controller button state flags. */
    struct s_ButtonFlags
    {
        int held;      /* `e_ControllerFlags` */
        int clicked;   /* `e_ControllerFlags` */
        int released;  /* `e_ControllerFlags` */
        int pulsed;    /* `e_ControllerFlags` */
        int pulsedGui; /* `e_ControllerFlags` */
    };

    /** @brief PSX controller input data. */
    struct s_ControllerData
    {
        s_AnalogController analogController;
        s32                pulseTicks;
        s_ButtonFlags      buttonFlags;
        s_AnalogSticks     rawSticks;        /** Raw analog stick values, signed range `[-128, 127]`. */
        s_AnalogSticks     normalizedSticks; /** Normalized analog stick values with deadzone, signed range `[-112, 112]`. */
        s32                field_28;         // Processed input flags.
    };

    /** @brief Controller key bindings for input actions. Contains bitfield of button presses assigned to each action.
     *
     * Bitfields only contain buttons. Analog directions and D-Pad aren't included.
     */
    struct s_ControllerConfig
    {
        u16 enter;
        u16 cancel;
        u16 skip;
        u16 action;
        u16 aim;
        u16 light;
        u16 run;
        u16 view;
        u16 stepLeft;
        u16 stepRight;
        u16 pause;
        u16 item;
        u16 map;
        u16 option;
    };

    extern s_ControllerData* const g_Controller0;
    extern s_ControllerData* const g_Controller1;

    /** @brief Updates digital data for all controllers, additionally handling special directional cases. */
    void Joy_ControllerDataUpdate();

    // TODO: Finish demagicking hex values. Does special handling for player movement.
    void ControllerData_AnalogToDigital(s_ControllerData* cont, bool arg1);
}
