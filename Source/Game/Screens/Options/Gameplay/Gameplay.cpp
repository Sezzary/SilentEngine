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
            .Binding        = MenuEntryLangugeBinding::Bind(&Options::Language)
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
        //MenuEntry
        //{
        //    .EntryStringKey = KEY_GAMEPLAY_MENU_BULLET_ADJUST,
        //    .Binding        = MenuEntryEnumBinding::Bind(&Options::BulletAdjust,
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
                    Options_ResetSelection(MainOptionsMenuEntry_Gameplay);

                    ScreenFade_Start(true, true, false);
                    Game_StateStepSet(0, OptionsMenuState_LeaveGameplay);
                }
                break;
            }
        }
    }
}
