#pragma once

#include "Game/include/common.h"

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
    InputAction_Enter  = 0,
    InputAction_Cancel = 1,
    InputAction_Skip   = 2,
    InputAction_Action = 3,
    InputAction_Aim    = 4,
    InputAction_Light  = 5,
    InputAction_Run    = 6,
    InputAction_View   = 7,
    InputAction_StepL  = 8,
    InputAction_StepR  = 9,
    InputAction_Pause  = 10,
    InputAction_Item   = 11,
    InputAction_Map    = 12,
    InputAction_Option = 13,

    InputAction_Count  = 14
};

struct s_ControllerMenu_SelectedEntries
{
    e_ControllerMenuState preset_0;
    e_InputAction         action_4;
};

extern bool g_ControllerMenu_IsOnActionsPane;

//extern DR_MODE g_ControllerMenu_SelectionHighlightDrawModes[MEMORY_CARD_SLOT_COUNT];
//extern POLY_G4 g_ControllerMenu_SelectionHighlightQuad[MEMORY_CARD_SLOT_COUNT];

extern s_ControllerMenu_SelectedEntries g_ControllerMenu_SelectedEntries;
extern s_ControllerMenu_SelectedEntries g_ControllerMenu_SelectedEntries;

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
 * @param isOnRightPane `true` if on the right pane, `false` if on the left pane.
 * @param presetsEntryIdx Selected presets pane entry index.
 * @param actionsEntryIdx Selected actions pane entry index.
 * @param boundActionIdx Index of the newly bound input action.
 */
void Options_ControllerMenu_EntriesDraw(bool isOnRightPane, int presetsEntryIdx, int actionsEntryIdx, int boundActionIdx);

/** @brief Draws button mapping icons in the controller configuration options menu.
 *
 * Called by `Options_ControllerMenu_EntriesDraw`.
 *
 * @param baseX Base X position at which to begin drawing.
 * @param baseY Base Y position at which to begin drawing.
 * @param config Controller configuration flags.
 */
void Options_ControllerMenu_ButtonIconsDraw(int baseX, int baseY, ushort config);
