#pragma once

#include "Game/Common.h"
#include "Game/Screens/Options/Brightness.h"
#include "Game/Screens/Options/Controller.h"
#include "Game/Screens/Options/MenuGraphics.h"
#include "Game/Screens/Options/SelectionGraphics.h"

namespace Silent::Game
{
    constexpr int DEPTH_24   = 24;
    constexpr int DEPTH_40   = 40;
    constexpr int DEPTH_36   = 36;
    constexpr int DEPTH_8148 = 8148;

    /** @brief Options menu states. Facilitates menu switching via `s_GameWork::gameStateStep[0]`. */
    enum e_OptionsMenuState
    {
        OptionsMenuState_EnterMainOptions,  /** Entering main options menu. */
        OptionsMenuState_MainOptions,       /** In main options menu. */
        OptionsMenuState_Brightness,        /** In brightness menu. */
        OptionsMenuState_Controller,        /** In controller config menu. */
        OptionsMenuState_Leave,             /** Leaving options menu back to gameplay. */
        OptionsMenuState_LeaveMainOptions,  /** Leaving main options menu. */
        OptionsMenuState_EnterBrightness,   /** Entering brightness menu. */
        OptionsMenuState_EnterController,   /** Entering controller config menu. */
        OptionsMenuState_LeaveBrightness,   /** Leaving brightness menu. */
        OptionsMenuState_LeaveController,   /** Leaving controller config menu. */
        OptionsMenuState_EnterExtraOptions, /** Entering extra options menu. */
        OptionsMenuState_ExtraOptions,      /** In extra options menu. */
        OptionsMenuState_LeaveExtraOptions  /** Leaving extra options menu. */
    };

    /** @brief Main options menu entries. */
    enum e_MainOptionsMenuEntry
    {
        MainOptionsMenuEntry_Exit,
        MainOptionsMenuEntry_Brightness,
        MainOptionsMenuEntry_Controller,
        MainOptionsMenuEntry_Vibration,
        MainOptionsMenuEntry_AutoLoad,
        MainOptionsMenuEntry_Sound,
        MainOptionsMenuEntry_BgmVolume,
        MainOptionsMenuEntry_SfxVolume,

        MainOptionsMenuEntry_Count
    };

    /** @brief Extra options menu entries. */
    enum e_ExtraOptionsMenuEntry
    {
        ExtraOptionsMenuEntry_WeaponCtrl,
        ExtraOptionsMenuEntry_Blood,
        ExtraOptionsMenuEntry_ViewCtrl,
        ExtraOptionsMenuEntry_RetreatTurn,
        ExtraOptionsMenuEntry_MovementCtrl,
        ExtraOptionsMenuEntry_AutoAiming,
        ExtraOptionsMenuEntry_ViewMode,
        ExtraOptionsMenuEntry_BulletMult,

        ExtraOptionsMenuEntry_Count
    };

    /** @brief Blood color menu entries. */
    enum e_BloodColorMenuEntry
    {
        BloodColorMenuEntry_Normal,
        BloodColorMenuEntry_Green,
        BloodColorMenuEntry_Violet,
        BloodColorMenuEntry_Black,

        BloodColorMenuEntry_Count
    };

    enum e_BloodColor
    {
        BloodColor_Normal = 0,
        BloodColor_Green  = 2,
        BloodColor_Violet = 5,
        BloodColor_Black  = 11
    };

    extern int g_Options_SelectionHighlightTimer;
    extern int g_MainOptionsMenu_SelectedEntry;
    extern int g_MainOptionsMenu_PrevSelectedEntry;
    extern int g_ExtraOptionsMenu_SelectedEntry;
    extern int g_ExtraOptionsMenu_PrevSelectedEntry;

    /** @brief Options menu game state handler. */
    void GameState_Options_Update();

    /** @brief Controller for the main options menu.
     *
     * Handles menu states, user input, and graphics draw calls.
     */
    void Options_MainOptionsMenu_Control();

    /** @brief Controller for the extra options menu. */
    void Options_ExtraOptionsMenu_Control();
}
