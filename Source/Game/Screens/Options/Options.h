#pragma once

#include "Game/Common.h"
#include "Game/Screens/Options/Brightness.h"
#include "Game/Screens/Options/Controller.h"
#include "Game/Screens/Options/MenuGraphics.h"
#include "Game/Screens/Options/ScreenPosition.h"
#include "Game/Screens/Options/SelectionGraphics.h"

namespace Silent::Game
{
    constexpr uint DEPTH_24   = 24;
    constexpr uint DEPTH_40   = 40;
    constexpr uint DEPTH_36   = 36;
    constexpr uint DEPTH_8148 = 8148;

    /** @brief Options menu states. Facilitates menu switching via `s_GameWork::gameStateStep[0]`. */
    enum e_OptionsMenuState
    {
        OptionsMenuState_EnterMainOptions  = 0,  /** Entering main options menu. */
        OptionsMenuState_MainOptions       = 1,  /** In main options menu. */
        OptionsMenuState_ScreenPos         = 2,  /** In screen position menu. */
        OptionsMenuState_Brightness        = 3,  /** In brightness menu. */
        OptionsMenuState_Controller        = 4,  /** In controller config menu. */
        OptionsMenuState_Leave             = 5,  /** Leaving options menu back to gameplay. */
        OptionsMenuState_LeaveMainOptions  = 6,  /** Leaving main options menu. */
        OptionsMenuState_EnterScreenPos    = 7,  /** Entering screen position menu. */
        OptionsMenuState_EnterBrightness   = 8,  /** Entering brightness menu. */
        OptionsMenuState_EnterController   = 9,  /** Entering controller config menu. */
        OptionsMenuState_LeaveScreenPos    = 10, /** Leaving screen position menu. */
        OptionsMenuState_LeaveBrightness   = 11, /** Leaving brightness menu. */
        OptionsMenuState_LeaveController   = 12, /** Leaving controller config menu. */
        OptionsMenuState_EnterExtraOptions = 13, /** Entering extra options menu. */
        OptionsMenuState_ExtraOptions      = 14, /** In extra options menu. */
        OptionsMenuState_LeaveExtraOptions = 15  /** Leaving extra options menu. */
    };

    /** @brief Main options menu entries. */
    enum e_MainOptionsMenuEntry
    {
        MainOptionsMenuEntry_Exit           = 0,
        MainOptionsMenuEntry_Brightness     = 1,
        MainOptionsMenuEntry_Controller     = 2,
        MainOptionsMenuEntry_ScreenPosition = 3,
        MainOptionsMenuEntry_Vibration      = 4,
        MainOptionsMenuEntry_AutoLoad       = 5,
        MainOptionsMenuEntry_Sound          = 6,
        MainOptionsMenuEntry_BgmVolume      = 7,
        MainOptionsMenuEntry_SfxVolume      = 8,

        MainOptionsMenuEntry_Count          = 9
    };

    /** @brief Extra options menu entries. */
    enum e_ExtraOptionsMenuEntry
    {
        ExtraOptionsMenuEntry_WeaponCtrl   = 0,
        ExtraOptionsMenuEntry_Blood        = 1,
        ExtraOptionsMenuEntry_ViewCtrl     = 2,
        ExtraOptionsMenuEntry_RetreatTurn  = 3,
        ExtraOptionsMenuEntry_MovementCtrl = 4,
        ExtraOptionsMenuEntry_AutoAiming   = 5,
        ExtraOptionsMenuEntry_ViewMode     = 6,
        ExtraOptionsMenuEntry_BulletMult   = 7,

        ExtraOptionsMenuEntry_Count        = 8
    };

    /** @brief Blood color menu entries. */
    enum e_BloodColorMenuEntry
    {
        BloodColorMenuEntry_Normal = 0,
        BloodColorMenuEntry_Green  = 1,
        BloodColorMenuEntry_Violet = 2,
        BloodColorMenuEntry_Black  = 3,

        BloodColorMenuEntry_Count  = 4
    };

    enum e_BloodColor
    {
        BloodColor_Normal = 0,
        BloodColor_Green  = 2,
        BloodColor_Violet = 5,
        BloodColor_Black  = 11
    };

    extern int g_MainOptionsMenu_SelectedEntry;
    extern int g_ExtraOptionsMenu_SelectedEntry;
    extern int g_MainOptionsMenu_PrevSelectedEntry;
    extern int g_ExtraOptionsMenu_PrevSelectedEntry;

    extern Vector2i g_ExtraOptions_SelectionHighlightFrom;
    extern Vector2i g_ExtraOptions_SelectionHighlightTo;
    extern Vector2i g_MainOptions_SelectionHighlightFrom;
    extern Vector2i g_MainOptions_SelectionHighlightTo;

    extern int g_Options_SelectionHighlightTimer;
    extern int g_ExtraOptionsMenu_EntryCount;
    extern int g_ExtraOptionsMenu_SelectedBloodColorEntry;
    extern int g_ExtraOptionsMenu_BulletMultMax;

    /** @brief Options menu game state handler. */
    void GameState_Options_Update();

    /** @brief Controller for the extra options menu. */
    void Options_ExtraOptionsMenu_Control();

    /** @brief Controller for the main options menu.
     *
     * Handles menu states, user input, and graphics draw calls.
     */
    void Options_MainOptionsMenu_Control();
}
