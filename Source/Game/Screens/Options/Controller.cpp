#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Options/Controller.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sys/SettingsReset.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Screens/Options/Options.h"
#include "Input/Input.h"

using namespace Silent::Assets;
using namespace Silent::Input;

namespace Silent::Game
{
    static bool g_ControllerMenu_IsOnActionsPane = false;

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

    void Options_ControllerMenu_EntriesDraw(bool isOnRightPane, int presetsEntryIdx, int actionsEntryIdx, int boundActionIdx) // 0x801E6F60
    {
        constexpr int STR_BASE_Y    = 22;
        constexpr int STR_OFFSET_Y  = 20;
        constexpr int ICON_SIZE_Y   = 12;
        constexpr int ICON_OFFSET_X = -12;

        static const char* PRESETS_PANE_ENTRY_STR_KEYS[] =
        {
            KEY_CONT_MENU_EXIT,
            KEY_CONT_MENU_TYPE_1,
            KEY_CONT_MENU_TYPE_2,
            KEY_CONT_MENU_TYPE_3
        };
        static const char* ACTIONS_PANE_ENTRY_STR_KEYS[] =
        {
            KEY_CONT_MENU_UP,
            KEY_CONT_MENU_DOWN,
            KEY_CONT_MENU_LEFT,
            KEY_CONT_MENU_RIGHT,
            KEY_CONT_MENU_ENTER,
            KEY_CONT_MENU_CANCEL,
            KEY_CONT_MENU_ACTION,
            KEY_CONT_MENU_AIM,
            KEY_CONT_MENU_LIGHT,
            KEY_CONT_MENU_RUN,
            KEY_CONT_MENU_VIEW,
            KEY_CONT_MENU_STEP_L,
            KEY_CONT_MENU_STEP_R,
            KEY_CONT_MENU_PAUSE,
            KEY_CONT_MENU_ITEM,
            KEY_CONT_MENU_MAP,
            KEY_CONT_MENU_OPTION
        };

        /** @brief Quads for textured entry selection highlights in the controller config menu.
         * 0: Left presets pane.
         * 1: Right actions pane.
         */
        static POLY_G4 SELECTION_HIGHLIGHT_QUADS[2] =
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
        };

        const auto& translator = g_App.GetTranslator();
        auto&       input      = g_App.GetInput();
        auto&       bindings   = input.GetBindings();

        s16      highlightY0;
        s16      highlightY1;
        int      strYPos;
        int      i;
        u16*     contConfig;
        POLY_G4* poly;

        // Draw entry strings.
        for (i = 0; i < ControllerMenuState_Count; i++)
        {
            Gfx_StringPositionSet(24, STR_BASE_Y + (i * STR_OFFSET_Y));
            Gfx_StringDraw(PRESETS_PANE_ENTRY_STR_KEYS[i], 20);
        }

        if (!isOnRightPane)
        {
            highlightY1 = presetsEntryIdx * STR_OFFSET_Y;
            highlightY0 = highlightY1 - 91;
            setXY4(poly,
                -137, highlightY0,
                -76,  highlightY0,
                -137, highlightY1 - 76,
                -76,  highlightY1 - 76);
        }

        strYPos     = STR_BASE_Y;
        highlightY0 = -300;

        // Draw controller config.
        for (auto actionGroupId : USER_ACTION_GROUP_IDS)
        {
            const auto& actionIds = ACTION_ID_GROUPS[(int)actionGroupId];
            for (auto actionId : actionIds)
            {
                const auto& profile = bindings.GetProfile(BindingProfileId::CustomKeyboardMouse);

                // Draw action string.
                Text_Debug_PositionSet(96, strYPos);
                Text_Debug_Draw(translator(ACTIONS_PANE_ENTRY_STR_KEYS[(int)actionId]));

                // Draw button icon.
                if (i != boundActionIdx)
                {
                    //Options_ControllerMenu_ButtonIconsDraw(ICON_OFFSET_X, strYPos - 114, *contConfig);
                }

                if (i == actionsEntryIdx)
                {
                    highlightY0 = strYPos - 113;
                }

                strYPos = (strYPos + ICON_SIZE_Y) + ((i == 2) ? ICON_SIZE_Y : 0);
                }
        }

        if (isOnRightPane)
        {
            setXY4(poly,
                -65, highlightY0,
                -15, highlightY0,
                -65, highlightY0 + 10,
                -15, highlightY0 + 10);
        }
    }
}
