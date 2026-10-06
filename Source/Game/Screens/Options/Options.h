#pragma once

#include "Game/Common.h"
#include "Game/Screens/Options/Graphics/Brightness.h"
#include "Game/Screens/Options/Input/Bindings.h"
#include "Game/Screens/Options/MenuGraphics.h"
#include "Game/Screens/Options/SelectionGraphics.h"

namespace Silent::Game
{
    constexpr int VISIBLE_ENTRY_COUNT_MAX = 9;

    /** @brief Options menu states. Facilitates menu switching via `s_GameWork::gameStateStep[0]`. */
    enum e_OptionsMenuState
    {
        OptionsMenuState_EnterOptions,
        OptionsMenuState_Options,
        OptionsMenuState_LeaveOptions,
        OptionsMenuState_Leave,

        OptionsMenuState_Brightness,
        OptionsMenuState_Bindings,
        OptionsMenuState_EnterBrightness,
        OptionsMenuState_EnterBindings,
        OptionsMenuState_LeaveBrightness,
        OptionsMenuState_LeaveBindings,

        OptionsMenuState_EnterGraphics,
        OptionsMenuState_Graphics,
        OptionsMenuState_LeaveGraphics,
        OptionsMenuState_EnterGameplay,
        OptionsMenuState_Gameplay,
        OptionsMenuState_LeaveGameplay,
        OptionsMenuState_EnterInput,
        OptionsMenuState_Input,
        OptionsMenuState_LeaveInput,
        OptionsMenuState_EnterEnhancements,
        OptionsMenuState_Enhancements,
        OptionsMenuState_LeaveEnhancements,
        OptionsMenuState_EnterSystem,
        OptionsMenuState_System,
        OptionsMenuState_LeaveSystem,
    };

    /** @brief Main options menu entries. */
    enum e_MainOptionsMenuEntry
    {
        MainOptionsMenuEntry_Exit,
        MainOptionsMenuEntry_Graphics,
        MainOptionsMenuEntry_Gameplay,
        MainOptionsMenuEntry_Input,
        MainOptionsMenuEntry_Enhancements,
        MainOptionsMenuEntry_System,

        MainOptionsMenuEntry_Count
    };

    /** @brief Options menu entries. @deprecated */
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

    extern int    g_OptionsMenu_SelectedEntry;
    extern int    g_OptionsMenu_PrevSelectedEntry;
    extern int    g_OptionsMenu_VisibleEntriesStartIdx;
    extern int    g_OptionsMenu_PrevVisibleEntriesStartIdx;
    extern q19_12 g_OptionsMenu_SelectionHighlightTimer;

    /** @brief Options menu game state handler. */
    void GameState_Options_Update();
}
