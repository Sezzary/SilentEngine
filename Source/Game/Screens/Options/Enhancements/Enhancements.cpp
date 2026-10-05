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
            .EntryStringKey = KEY_ENHANCEMENTS_MENU_SKIP_LOGOS,
            .Binding        = BindMenuEntryBool(&Options::SkipLogos)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_ENHANCEMENTS_MENU_PAPER_MAP_QUALITY,
            .Binding        = BindMenuEntryEnum(&Options::PaperMapQuality,
            {
                KEY_ENHANCEMENTS_MENU_PAPER_MAP_QUALITY_RETRO,
                KEY_ENHANCEMENTS_MENU_PAPER_MAP_QUALITY_MODERN
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_ENHANCEMENTS_MENU_DIALOG_PAUSE,
            .Binding        = BindMenuEntryEnum(&Options::DialogPause,
            {
                KEY_ENHANCEMENTS_MENU_DIALOG_PAUSE_RETRO,
                KEY_ENHANCEMENTS_MENU_DIALOG_PAUSE_REFINED
            })
        }
    };

    void Options_EnhancementsMenu_Control()
    {
        const auto& input = g_App.GetInput();

        // Draw graphics.
        OptionsMenu_DrawEntries(KEY_ENHANCEMENTS_MENU_HEADING, ENTRIES);
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_Enhancements)
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
                    OptionsMenu_ResetSelection(MainOptionsMenuEntry_Enhancements);

                    ScreenFade_Start(true, true, false);
                    Game_StateStepSet(0, OptionsMenuState_LeaveEnhancements);
                }
                break;
            }
        }
    }
}
