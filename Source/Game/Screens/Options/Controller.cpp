#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Options/Controller.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sys/SettingsReset.h"
#include "Game/Screens/Options/Options.h"
#include "Input/Input.h"

using namespace Silent::Input;

namespace Silent::Game
{
    static bool g_ControllerMenu_IsOnActionsPane = false;

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

    void Options_ControllerMenu_Control()
    {
        static auto selectedEntries = s_ControllerMenu_SelectedEntries{};

        const auto& input = g_App.GetInput();

        int boundActionIdx = NO_VALUE;

        // Handle controller config menu state.
        switch (g_GameWork.gameStateSteps[1])
        {
            case ControllerMenuState_Exit:
                ScreenFade_Start(false, true, false);
                selectedEntries.preset = ControllerMenuState_Exit;

                // Leave menu.
                if (input.GetAction(In::Enter).IsClicked() ||
                    input.GetAction(In::Cancel).IsClicked())
                {
                    //Sd_EngineCmd(Sfx_Cancel);

                    ScreenFade_Start(false, false, false);
                    g_GameWork.gameStateSteps[1] = ControllerMenuState_Leave;
                    g_GameWork.gameStateSteps[2] = 0;
                    break;
                }

                // Move selection cursor up/down.
                if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    g_GameWork.gameStateSteps[1] = ControllerMenuState_Type3;
                    g_GameWork.gameStateSteps[2] = 0;
                }
                else if (input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    g_GameWork.gameStateSteps[1] = ControllerMenuState_Type1;
                    g_GameWork.gameStateSteps[2] = 0;
                }
                // Move selection cursor left/right.
                else if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) ||
                         input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    g_GameWork.gameStateSteps[1] = ControllerMenuState_Actions;
                    g_GameWork.gameStateSteps[2] = 0;
                }
                break;

            case ControllerMenuState_Type1:
            case ControllerMenuState_Type2:
            case ControllerMenuState_Type3:
                selectedEntries.preset = (e_ControllerMenuState)g_GameWork.gameStateSteps[1];

                // Set binding preset.
                if (input.GetAction(In::Enter).IsClicked())
                {
                    //Sd_EngineCmd(Sfx_Confirm);
                    Settings_RestoreControlDefaults(g_GameWork.gameStateSteps[1] - 1);
                }
                // Reset selection cursor.
                else if (input.GetAction(In::Cancel).IsClicked())
                {
                    //Sd_EngineCmd(Sfx_Cancel);
                    g_GameWork.gameStateSteps[1] = ControllerMenuState_Exit;
                    g_GameWork.gameStateSteps[2] = 0;
                }
                // Move selection cursor.
                else
                {
                    // Move selection cursor up/down.
                    if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                    {
                        g_GameWork.gameStateSteps[1] = (g_GameWork.gameStateSteps[1] - 1) & 3;
                        g_GameWork.gameStateSteps[2] = 0;
                    }
                    else if (input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                    {
                        g_GameWork.gameStateSteps[1] = (g_GameWork.gameStateSteps[1] + 1) & 3;
                        g_GameWork.gameStateSteps[2] = 0;
                    }
                    // Move selection cursor left/right.
                    else if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) ||
                             input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                    {
                        g_GameWork.gameStateSteps[1] = ControllerMenuState_Actions;
                        g_GameWork.gameStateSteps[2] = 0;
                    }
                }
                break;

            case ControllerMenuState_Actions:
            {
                auto actionIdx = selectedEntries.action;

                // Move selection cursor up/down.
                if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    if (actionIdx != InputAction_Enter)
                    {
                        selectedEntries.action = (e_InputAction)(actionIdx - 1);
                    }
                    else
                    {
                        selectedEntries.action = InputAction_Option;
                    }
                }
                else if (input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    if (actionIdx != InputAction_Option)
                    {
                        selectedEntries.action = (e_InputAction)(actionIdx + 1);
                    }
                    else
                    {
                        selectedEntries.action = InputAction_Enter;
                    }
                }
                // Move selection cursor left/right.
                else if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) ||
                         input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    g_GameWork.gameStateSteps[2] = 0;
                    g_GameWork.gameStateSteps[1] = selectedEntries.preset;
                }
                // Bind button to input action.
                else
                {
                    //boundActionIdx = Options_ControllerMenu_ConfigUpdate(actionIdx);
                }
                break;
            }

            case ControllerMenuState_Leave:
                // Switch to previous menu.
                if (ScreenFade_IsFinished())
                {
                    ScreenFade_Start(true, true, false);
                    g_GameWork.gameStateSteps[0]   = OptionsMenuState_LeaveController;
                    g_SysWork.gameStateStepCounter = 0;
                    g_GameWork.gameStateSteps[1]   = 0;
                    g_GameWork.gameStateSteps[2]   = 0;
                }
                break;
        }

        if (g_GameWork.gameStateSteps[1] == ControllerMenuState_Actions)
        {
            g_ControllerMenu_IsOnActionsPane = true;
        }
        else
        {
            g_ControllerMenu_IsOnActionsPane = false;
        }

        // Play cursor navigation SFX.
        if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN)   ||
            input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) ||
            input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) ||
            input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
        {
            //Sd_EngineCmd(Sfx_Back);
        }

        // Draw menu graphics.
        //Options_ControllerMenu_EntriesDraw(g_ControllerMenu_IsOnActionsPane, selectedEntries.preset, selectedEntries.action, boundActionIdx);
    }
}
