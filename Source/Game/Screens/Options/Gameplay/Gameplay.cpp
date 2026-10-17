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
            .EntryStringKey = KEY_GAMEPLAY_MENU_AUTO_LOAD,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableAutoLoad)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_SUBTITLES,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableSubtitles)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_LANGUAGE,
            .Binding        = MenuEntryLanguageBinding::Bind(&Options::Language)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_SOUND,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::Sound,
            {
                KEY_GAMEPLAY_MENU_SOUND_STEREO,
                KEY_GAMEPLAY_MENU_SOUND_MONAURAL
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_BGM_VOLUME,
            .Binding        = MenuEntryBarBinding::Bind(&Options::BgmVolume, SOUND_VOLUME_MAX)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_SE_VOLUME,
            .Binding        = MenuEntryBarBinding::Bind(&Options::SeVolume, SOUND_VOLUME_MAX)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_BLOOD_COLOR,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::BloodColor,
            {
                KEY_GAMEPLAY_MENU_BLOOD_COLOR_NORMAL,
                KEY_GAMEPLAY_MENU_BLOOD_COLOR_GREEN,
                KEY_GAMEPLAY_MENU_BLOOD_COLOR_VIOLET,
                KEY_GAMEPLAY_MENU_BLOOD_COLOR_BLACK
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_BULLET_ADJUST,
            .Binding        = MenuEntryRangeBinding::Bind(&Options::BulletAdjust, 1, 6, "x")
        }
    };

    void Options_ControlGameplayMenu()
    {
        Options_ControlSubmenu(KEY_GAMEPLAY_MENU_HEADING, ENTRIES, 1,
                               OptionsMenuState_Gameplay, OptionsMenuState_LeaveGameplay,
                               MainOptionsMenuEntry_Gameplay);
    }
}
