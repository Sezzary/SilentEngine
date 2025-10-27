#include "Framework.h"
#include "Game/include/screens/options/controller.h"

#include "Game/dummy.h"
#include "Game/include/screens/options/options.h"

namespace Silent::Game
{
    bool g_ControllerMenu_IsOnActionsPane = false;
    s_ControllerMenu_SelectedEntries g_ControllerMenu_SelectedEntries;

    /** @brief Draw modes for textured entry selection highlights in the controller config menu.
     * 0 corresponds to the presets pane on the left,
     * 1 corresponds to the actions pane on the right.
     */
    /*DR_MODE g_ControllerMenu_SelectionHighlightDrawModes[2] =
    {
        {
            .tag  = 0x03000000,
            .code = { 0xE1000200, 0 }
        },
        {
            .tag  = 0x03000000,
            .code = { 0xE1000200, 0 }
        }
    };*/

    /** @brief Quads for textured entry selection highlights in the controller config menu.
     * 0 corresponds to the presets pane on the left,
     * 1 corresponds to the actions pane on the right.
     */
    /*POLY_G4 g_ControllerMenu_SelectionHighlightQuads[2] =
    {
        {
            .tag  = 0x08000000,
            .r0   = 255,
            .g0   = 255,
            .b0   = 255,
            .code = 0x3A,
            .r3   = 255,
            .g3   = 255,
            .b3   = 255
        },
        {
            .tag  = 0x08000000,
            .code = 0x3A,
            .r1   = 255,
            .g1   = 255,
            .b1   = 255,
            .r2   = 255,
            .g2   = 255,
            .b2   = 255
        },
    };*/

    /*void Options_ControllerMenu_Control() // 0x801E69BC
    {
        int           boundActionIdx = NO_VALUE;
        e_InputAction actionIdx;

        // Handle controller config menu state.
        switch (g_GameWork.gameStateStep_598[1])
        {
            case ControllerMenuState_Exit:
                //ScreenFade_Start(false, true, false);
                g_ControllerMenu_SelectedEntries.preset_0 = ControllerMenuState_Exit;

                // Leave menu.
                if (g_Controller0.btnsClicked_10 & (g_GameWork.config_0.controllerConfig_0.enter_0 |
                                                    g_GameWork.config_0.controllerConfig_0.cancel_2))
                {
                    //Sd_EngineCmd(Sfx_Cancel);

                    //ScreenFade_Start(false, false, false);
                    g_GameWork.gameStateStep_598[1] = ControllerMenuState_Leave;
                    g_GameWork.gameStateStep_598[2] = 0;
                    break;
                }

                // Move selection cursor up/down.
                if (g_Controller0.btnsPulsedGui_1C & ControllerFlag_LStickUp)
                {
                    g_GameWork.gameStateStep_598[1] = ControllerMenuState_Type3;
                    g_GameWork.gameStateStep_598[2] = 0;
                }
                else if (g_Controller0.btnsPulsedGui_1C & ControllerFlag_LStickDown)
                {
                    g_GameWork.gameStateStep_598[1] = ControllerMenuState_Type1;
                    g_GameWork.gameStateStep_598[2] = 0;
                }
                // Move selection cursor left/right.
                else if (g_Controller0.btnsPulsedGui_1C & (ControllerFlag_LStickLeft | ControllerFlag_LStickRight))
                {
                    g_GameWork.gameStateStep_598[1] = ControllerMenuState_Actions;
                    g_GameWork.gameStateStep_598[2] = 0;
                }
                break;

            case ControllerMenuState_Type1:
            case ControllerMenuState_Type2:
            case ControllerMenuState_Type3:
                g_ControllerMenu_SelectedEntries.preset_0 = g_GameWork.gameStateStep_598[1];

                // Set binding preset.
                if (g_Controller0.btnsClicked_10 & g_GameWork.config_0.controllerConfig_0.enter_0)
                {
                    //Sd_EngineCmd(Sfx_Confirm);
                    //Settings_RestoreControlDefaults(g_GameWork.gameStateStep_598[1] - 1);
                }
                // Reset selection cursor.
                else if (g_Controller0.btnsClicked_10 & g_GameWork.config_0.controllerConfig_0.cancel_2)
                {
                    //Sd_EngineCmd(Sfx_Cancel);
                    g_GameWork.gameStateStep_598[1] = ControllerMenuState_Exit;
                    g_GameWork.gameStateStep_598[2] = 0;
                }
                // Move selection cursor.
                else
                {
                    // Move selection cursor up/down.
                    if (g_Controller0.btnsPulsedGui_1C & ControllerFlag_LStickUp)
                    {
                        g_GameWork.gameStateStep_598[1] = (g_GameWork.gameStateStep_598[1] - 1) & 3;
                        g_GameWork.gameStateStep_598[2] = 0;
                    }
                    else if (g_Controller0.btnsPulsedGui_1C & ControllerFlag_LStickDown)
                    {
                        g_GameWork.gameStateStep_598[1] = (g_GameWork.gameStateStep_598[1] + 1) & 3;
                        g_GameWork.gameStateStep_598[2] = 0;
                    }
                    // Move selection cursor left/right.
                    else if (g_Controller0.btnsPulsedGui_1C & (ControllerFlag_LStickLeft | ControllerFlag_LStickRight))
                    {
                        g_GameWork.gameStateStep_598[1] = ControllerMenuState_Actions;
                        g_GameWork.gameStateStep_598[2] = 0;
                    }
                }
                break;

            case ControllerMenuState_Actions:
                actionIdx = g_ControllerMenu_SelectedEntries.action_4;

                // Move selection cursor up/down.
                if (g_Controller0.btnsPulsedGui_1C & ControllerFlag_LStickUp)
                {
                    if (actionIdx != InputAction_Enter)
                    {
                        g_ControllerMenu_SelectedEntries.action_4 = actionIdx - 1;
                    }
                    else
                    {
                        g_ControllerMenu_SelectedEntries.action_4 = InputAction_Option;
                    }
                }
                else if (g_Controller0.btnsPulsedGui_1C & ControllerFlag_LStickDown)
                {
                    if (actionIdx != InputAction_Option)
                    {
                        g_ControllerMenu_SelectedEntries.action_4 = actionIdx + 1;
                    }
                    else
                    {
                        g_ControllerMenu_SelectedEntries.action_4 = InputAction_Enter;
                    }
                }
                // Move selection cursor left/right.
                else if (g_Controller0.btnsPulsedGui_1C & (ControllerFlag_LStickLeft | ControllerFlag_LStickRight))
                {
                    g_GameWork.gameStateStep_598[2] = 0;
                    g_GameWork.gameStateStep_598[1] = g_ControllerMenu_SelectedEntries.preset_0;
                }
                // Bind button to input action.
                else
                {
                    boundActionIdx = Options_ControllerMenu_ConfigUpdate(actionIdx);
                }
                break;

            case ControllerMenuState_Leave:
                // Switch to previous menu.
                if (false)//(ScreenFade_IsFinished())
                {
                    //ScreenFade_Start(true, true, false);
                    g_GameWork.gameStateStep_598[0] = OptionsMenuState_LeaveController;
                    g_SysWork.timer_20              = 0;
                    g_GameWork.gameStateStep_598[1] = 0;
                    g_GameWork.gameStateStep_598[2] = 0;
                }
                break;
        }

        if (g_GameWork.gameStateStep_598[1] == ControllerMenuState_Actions)
        {
            g_ControllerMenu_IsOnActionsPane = true;
        }
        else
        {
            g_ControllerMenu_IsOnActionsPane = false;
        }

        // Play cursor navigation SFX.
        if (g_Controller0.btnsPulsedGui_1C & (ControllerFlag_LStickUp | ControllerFlag_LStickRight | ControllerFlag_LStickDown | ControllerFlag_LStickLeft))
        {
            //Sd_EngineCmd(Sfx_Back);
        }

        // Draw menu graphics.
        Options_ControllerMenu_EntriesDraw(g_ControllerMenu_IsOnActionsPane, g_ControllerMenu_SelectedEntries.preset_0, g_ControllerMenu_SelectedEntries.action_4, boundActionIdx);
    }*/
}
