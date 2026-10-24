#include "Framework.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Screens/Options/Enhancements/Enhancements.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Screens/Options/Options.h"
#include "Game/Screens/Options/Utils.h"
#include "Services/Options.h"

using namespace Silent::Assets;
using namespace Silent::Services;

namespace Silent::Game
{
    static const auto ENTRIES = std::vector<MenuEntry>
    {
        MenuEntry
        {
            .EntryStringKey = KEY_SYSTEM_MENU_TOASTS,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableToasts)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_SYSTEM_MENU_PARALLELISM,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableParallelism)
        }
    };

    void Options_ControlSystemMenu()
    {
        Options_ControlSubmenu(KEY_SYSTEM_MENU_HEADING, ENTRIES, 1,
                               OptionsMenuState_System, OptionsMenuState_LeaveSystem,
                               MainOptionsMenuEntry_System);
    }
}
