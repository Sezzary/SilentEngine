#include "Framework.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Screens/Options/Enhancements/Enhancements.h"

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
            .EntryStringKey = KEY_ENHANCEMENTS_MENU_SKIP_LOGOS,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::SkipLogos)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_ENHANCEMENTS_MENU_PAPER_MAP_QUALITY,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::PaperMapQuality,
            {
                KEY_ENHANCEMENTS_MENU_PAPER_MAP_QUALITY_RETRO,
                KEY_ENHANCEMENTS_MENU_PAPER_MAP_QUALITY_MODERN
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_ENHANCEMENTS_MENU_DIALOG_PAUSE,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::DialogPause,
            {
                KEY_ENHANCEMENTS_MENU_DIALOG_PAUSE_RETRO,
                KEY_ENHANCEMENTS_MENU_DIALOG_PAUSE_REFINED
            })
        }
    };

    void Options_ControlEnhancementsMenu()
    {
        Options_ControlSubmenu(KEY_ENHANCEMENTS_MENU_HEADING, ENTRIES, 1,
                               OptionsMenuState_Enhancements, OptionsMenuState_LeaveEnhancements,
                               MainOptionsMenuEntry_Enhancements);
    }
}
