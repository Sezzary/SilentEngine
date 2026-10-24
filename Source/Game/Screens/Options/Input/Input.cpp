#include "Framework.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Screens/Options/Input/Input.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Screens/Options/Options.h"
#include "Services/Options.h"

using namespace Silent::Assets;
using namespace Silent::Services;

namespace Silent::Game
{
    static const auto ENTRIES = std::vector<MenuEntry>
    {
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_BINDINGS_CONFIG,
            .Binding        = MenuEntrySubmenuBinding
            {
                OptionsMenuState_EnterBindings
            }
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_VIBRATION,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableVibration)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_MOUSE_SENSITIVITY,
            .Binding        = MenuEntryRangeBinding::Bind(&Options::MouseSensitivity,
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

    void Options_ControlInputMenu()
    {
        Options_ControlSubmenu(KEY_INPUT_MENU_HEADING, ENTRIES, 1,
                               OptionsMenuState_Input, OptionsMenuState_LeaveInput,
                               MainOptionsMenuEntry_Input);
    }
}
