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
            .EntryStringKey = KEY_INPUT_MENU_BINDINGS,
            .Binding        = MenuEntrySubmenuBinding{}
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_ENABLE_VIBRATION,
            .Binding        = BindMenuEntryBool(&Options::EnableVibration)
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
            .Binding        = BindMenuEntryEnum(&Options::WeaponControl,
            {
                KEY_INPUT_MENU_WEAPON_CONTROL_SWITCH,
                KEY_INPUT_MENU_WEAPON_CONTROL_PRESS
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_VIEW_CONTROL,
            .Binding        = BindMenuEntryEnum(&Options::ViewControl,
            {
                KEY_INPUT_MENU_VIEW_CONTROL_NORMAL,
                KEY_INPUT_MENU_VIEW_CONTROL_REVERSE
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_RETREAT_TURN_CONTROL,
            .Binding        = BindMenuEntryEnum(&Options::RetreatTurnControl,
            {
                KEY_INPUT_MENU_RETREAT_TURN_CONTROL_NORMAL,
                KEY_INPUT_MENU_RETREAT_TURN_CONTROL_REVERSE
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_WALK_RUN_CONTROL,
            .Binding        = BindMenuEntryEnum(&Options::WalkRunControl,
            {
                KEY_INPUT_MENU_WALK_RUN_CONTROL_NORMAL,
                KEY_INPUT_MENU_WALK_RUN_CONTROL_REVERSE
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_INPUT_MENU_VIEW_MODE,
            .Binding        = BindMenuEntryEnum(&Options::ViewMode,
            {
                KEY_INPUT_MENU_VIEW_MODE_NORMAL,
                KEY_INPUT_MENU_VIEW_MODE_SELF_VIEW
            })
        }
    };

    void Options_InputMenu_Control()
    {

    }
}
