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

    constexpr int VISIBLE_ENTRY_COUNT_MAX = 8;

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
        OptionsMenuState_LeaveController    /** Leaving controller config menu. */
    };

    /** @brief Options menu entries. */
    enum e_OptionsMenuEntry
    {
        OptionsMenuEntry_Exit,
        OptionsMenuEntry_Brightness,
        OptionsMenuEntry_Controller,
        OptionsMenuEntry_Vibration,
        OptionsMenuEntry_AutoLoad,
        OptionsMenuEntry_Sound,
        OptionsMenuEntry_BgmVolume,
        OptionsMenuEntry_SfxVolume,
        OptionsMenuEntry_Language,
        OptionsMenuEntry_WeaponCtrl,
        OptionsMenuEntry_Blood,
        OptionsMenuEntry_ViewCtrl,
        OptionsMenuEntry_RetreatTurn,
        OptionsMenuEntry_WalkRunCtrl,
        OptionsMenuEntry_AutoAiming,
        OptionsMenuEntry_ViewMode,
        OptionsMenuEntry_BulletAdjust,

        OptionsMenuEntry_Count
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

    extern int g_OptionsMenu_SelectionHighlightTimer;
    extern int g_OptionsMenu_SelectedEntry;
    extern int g_OptionsMenu_PrevSelectedEntry;
    extern int g_OptionsMenu_VisibleEntriesStartIdx;

    /** @brief Options menu game state handler. */
    void GameState_Options_Update();

    /** @brief Controller for the options menu. */
    void OptionsMenu_Control();

    /** @brief Controller for the extra options menu. */
    void Options_ExtraOptionsMenu_Control();
}
