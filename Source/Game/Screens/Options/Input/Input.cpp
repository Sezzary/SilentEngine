#include "Framework.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Screens/Options/Input/Input.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Game.h"
#include "Game/Screens/Options/Options.h"
#include "Game/Screens/Options/Input/Bindings.h"
#include "Game/Screens/Options/MenuGraphics.h"
#include "Game/Screens/Options/Utils.h"
#include "Input/Input.h"
#include "Services/Options.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Services;

namespace Silent::Game
{
    static const auto ENTRIES = std::vector<MenuEntry>
    {
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_BINDINGS_CONFIG,
            .Binding        = MenuEntrySubmenuBinding{}
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_VIBRATION,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableVibration)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_MOUSE_SENSITIVITY,
            .Binding        = BindMenuEntryRange(&Options::MouseSensitivity,
                                                 MOUSE_SENSITIVITY_MIN, MOUSE_SENSITIVITY_MAX)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_WEAPON_CONTROL,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::WeaponControl,
            {
                KEY_INPUT_MENU_WEAPON_CONTROL_SWITCH,
                KEY_INPUT_MENU_WEAPON_CONTROL_PRESS
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_VIEW_CONTROL,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::ViewControl,
            {
                KEY_INPUT_MENU_VIEW_CONTROL_NORMAL,
                KEY_INPUT_MENU_VIEW_CONTROL_REVERSE
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_RETREAT_TURN_CONTROL,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::RetreatTurnControl,
            {
                KEY_INPUT_MENU_RETREAT_TURN_CONTROL_NORMAL,
                KEY_INPUT_MENU_RETREAT_TURN_CONTROL_REVERSE
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_WALK_RUN_CONTROL,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::WalkRunControl,
            {
                KEY_INPUT_MENU_WALK_RUN_CONTROL_NORMAL,
                KEY_INPUT_MENU_WALK_RUN_CONTROL_REVERSE
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_AUTO_AIMING,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::DisableAutoAiming)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_VIEW_MODE,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::ViewMode,
            {
                KEY_INPUT_MENU_VIEW_MODE_NORMAL,
                KEY_INPUT_MENU_VIEW_MODE_SELF_VIEW
            })
        }
    };

    void OptionsMenu_ControlInputMenu()
    {
        const auto& input = g_App.GetInput();

        // Draw graphics.
        OptionsMenu_DrawEntries(KEY_INPUT_MENU_HEADING, ENTRIES);
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_Input)
        {
            return;
        }

        Options_UpdateConfig(ENTRIES);
        Options_UpdateSelection(ENTRIES.size());

        // Handle menu state.
        switch (g_GameWork.gameStateSteps[1])
        {
            case 0:
            {
                Options_ResetSelection();

                ScreenFade_Start(true, true, false);
                Game_StateStepIncrement(1);
                break;
            }
            case 1:
            {
                if (input.GetAction(In::Cancel).IsClicked())
                {
                    //Sd_SfxPlay(Sfx_Cancel, 0, Q8(0.25f));

                    ScreenFade_Start(true, false, false);
                    Game_StateStepIncrement(1);
                }
                break;
            }
            case 2:
            {
                // Switch to previous menu.
                if (ScreenFade_IsFinished())
                {
                    Options_ResetSelection(MainOptionsMenuEntry_Input);

                    ScreenFade_Start(true, true, false);
                    Game_StateStepSet(0, OptionsMenuState_LeaveInput);
                }
                break;
            }
        }
    }
}
