#include "Framework.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Screens/Options/Enhancements/Enhancements.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Game.h"
#include "Game/Screens/Options/Options.h"
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
            .EntryStringKey = KEY_GAMEPLAY_MENU_AUTO_LOAD,
            .Binding        = BindMenuEntryBool(&Options::EnableAutoLoad)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_SUBTITLES,
            .Binding        = BindMenuEntryBool(&Options::EnableSubtitles)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_LANGUAGE,
            .Binding        = MenuEntryStringsBinding
            {
                .GetIdx = []()
                {
                    const auto& translator = g_App.GetTranslator();

                    const auto& locales = translator.GetLocales();
                    for (int i = 0; i < locales.size(); i++)
                    {
                        if (locales[i].Name == translator.GetActiveLocaleName())
                        {
                            return i;
                        }
                    }

                    return 0;
                },
                .SetIdx = [](int idx)
                {
                    auto& options    = g_App.GetOptions().GetFront();
                    auto& translator = g_App.GetTranslator();

                    const auto& locales = translator.GetLocales();

                    options.Language = locales[idx].Name;
                    translator.SetActiveLocale(locales[idx].Name);
                },
                .GetStrings = []()
                {
                    const auto& translator = g_App.GetTranslator();

                    auto names = std::vector<std::string>{};
                    for (const auto& locale : translator.GetLocales())
                    {
                        names.push_back(locale.Name);
                    }

                    return names;
                }
            }
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_SOUND,
            .Binding        = BindMenuEntryEnum(&Options::Sound,
            {
                KEY_GAMEPLAY_MENU_SOUND_STEREO,
                KEY_GAMEPLAY_MENU_SOUND_MONAURAL
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_BGM_VOLUME,
            .Binding        = BindMenuEntryBar(&Options::BgmVolume, SOUND_VOLUME_MAX)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_SE_VOLUME,
            .Binding        = BindMenuEntryBar(&Options::SeVolume, SOUND_VOLUME_MAX)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GAMEPLAY_MENU_BLOOD_COLOR,
            .Binding        = BindMenuEntryEnum(&Options::BloodColor,
            {
                KEY_GAMEPLAY_MENU_BLOOD_COLOR_NORMAL,
                KEY_GAMEPLAY_MENU_BLOOD_COLOR_GREEN,
                KEY_GAMEPLAY_MENU_BLOOD_COLOR_VIOLET,
                KEY_GAMEPLAY_MENU_BLOOD_COLOR_BLACK
            })
        },
        //MenuEntry
        //{
        //    .EntryStringKey = KEY_GAMEPLAY_MENU_BULLET_ADJUST,
        //    .Binding        = BindMenuEntryEnum(&Options::BulletAdjust,
        //    {
        //        // @todo
        //    })
        //}
    };

    void Options_GameplayMenu_Control()
    {
        const auto& input = g_App.GetInput();

        // Draw graphics.
        OptionsMenu_DrawEntries(KEY_GAMEPLAY_MENU_HEADING, ENTRIES);
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_Gameplay)
        {
            return;
        }

        OptionsMenu_UpdateConfig(ENTRIES);
        OptionsMenu_UpdateSelection(ENTRIES.size());

        // Handle menu state.
        switch (g_GameWork.gameStateSteps[1])
        {
            case 0:
            {
                OptionsMenu_ResetSelection();

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
                    OptionsMenu_ResetSelection(MainOptionsMenuEntry_Gameplay);

                    ScreenFade_Start(true, true, false);
                    Game_StateStepSet(0, OptionsMenuState_LeaveGameplay);
                }
                break;
            }
        }
    }
}
