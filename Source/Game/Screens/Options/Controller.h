#pragma once

#include "Game/Common.h"

namespace Silent::Game
{
    /** @brief Controller config menu states. Also used as entries for the left presets pane.
     *
     * When the user navigates the controller config menu, this facilitates menu switching
     * via `s_GameWork::gameStateStep[1]`.
     */
    enum e_ControllerMenuState
    {
        ControllerMenuState_Leave   = -2,
        ControllerMenuState_Actions = -1,
        ControllerMenuState_Exit    = 0,
        ControllerMenuState_Type1   = 1,
        ControllerMenuState_Type2   = 2,
        ControllerMenuState_Type3   = 3,

        ControllerMenuState_Count   = 4
    };

    /** @brief Input actions. Also used as controller config menu entries for the right pane. */
    enum e_InputAction
    {
        InputAction_Enter,
        InputAction_Cancel,
        InputAction_Action,
        InputAction_Aim,
        InputAction_Light,
        InputAction_Run,
        InputAction_View,
        InputAction_StepL,
        InputAction_StepR,
        InputAction_Pause,
        InputAction_Item,
        InputAction_Map,
        InputAction_Option,

        InputAction_Count
    };

    /** @brief Selected controller options menu column entries. */
    struct s_ControllerMenu_SelectedEntries
    {
        e_ControllerMenuState preset;
        e_InputAction         action;
    };

    /** @brief Controller for the controller configuration options menu.
     *
     * Handles the menu state, user input, SFX, and graphics drawing.
    */
    void Options_ControllerMenu_Control();

    /** @brief Changes the button mapping based on the input and updates the configuration.
     *
     * @param actionIdx Index of the input action for which to update the controller configuration.
     */
    int Options_ControllerMenu_ConfigUpdate(int actionIdx);

    /** @brief Draws entry strings and icons in the controller configuration options menu.
     *
     * @param isOnRightPane `true` if on the right pane, `false` if on the left.
     * @param entryIdx0 Selected entry index in left pane.
     * @param entryIdx1 Selected entry index in right pane.
     * @param boundActionIdx Index of the newly bound input action.
     */
    void Options_ControllerMenu_EntriesDraw(bool isOnRightPane, int entryIdx0, int entryIdx1, int boundActionIdx);

    /** @brief Draws button mapping icons in the controller configuration options menu.
     *
     * Called by `Options_ControllerMenu_EntriesDraw`.
     *
     * @param baseX Base X position at which to begin drawing.
     * @param baseY Base Y position at which to begin drawing.
     * @param config Controller configuration flags.
     */
    void Options_ControllerMenu_ButtonIconsDraw(int baseX, int baseY, ushort config);
}
