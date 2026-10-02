#include "Framework.h"
#include "Game/Screens/Options/Input/Bindings.h"
#include "Game/Screens/Options/Utils.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"

using namespace Silent::Assets;

namespace Silent::Game
{
    static const auto ENTRIES = std::vector<MenuEntry>
    {
        MenuEntry
        {
            .Type           = MenuEntryType::Submenu,
            .EntryStringKey = KEY_INPUT_MENU_BINDINGS
        },
        MenuEntry
        {
            .Type             = MenuEntryType::List,
            .EntryStringKey   = KEY_INPUT_MENU_ENABLE_VIBRATION,
            .ConfigStringKeys =
            {
                KEY_OPTIONS_MENU_ON,
                KEY_OPTIONS_MENU_OFF
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::List,
            .EntryStringKey   = KEY_INPUT_MENU_MOUSE_SENSITIVITY,
            .ConfigStringKeys =
            {
                "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
                "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::List,
            .EntryStringKey   = KEY_INPUT_MENU_WEAPON_CONTROL,
            .ConfigStringKeys =
            {
                KEY_INPUT_MENU_WEAPON_CONTROL_SWITCH,
                KEY_INPUT_MENU_WEAPON_CONTROL_PRESS
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::List,
            .EntryStringKey   = KEY_INPUT_MENU_VIEW_CONTROL,
            .ConfigStringKeys =
            {
                KEY_INPUT_MENU_VIEW_CONTROL_NORMAL,
                KEY_INPUT_MENU_VIEW_CONTROL_REVERSE
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::List,
            .EntryStringKey   = KEY_INPUT_MENU_RETREAT_TURN_CONTROL,
            .ConfigStringKeys =
            {
                KEY_INPUT_MENU_RETREAT_TURN_CONTROL_NORMAL,
                KEY_INPUT_MENU_RETREAT_TURN_CONTROL_REVERSE
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::List,
            .EntryStringKey   = KEY_INPUT_MENU_WALK_RUN_CONTROL,
            .ConfigStringKeys =
            {
                KEY_INPUT_MENU_WALK_RUN_CONTROL_NORMAL,
                KEY_INPUT_MENU_WALK_RUN_CONTROL_REVERSE
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::List,
            .EntryStringKey   = KEY_INPUT_MENU_VIEW_MODE,
            .ConfigStringKeys =
            {
                KEY_INPUT_MENU_VIEW_MODE_NORMAL,
                KEY_INPUT_MENU_VIEW_MODE_SELF_VIEW
            }
        }
    };

    void Options_InputMenu_Control()
    {

    }
}
