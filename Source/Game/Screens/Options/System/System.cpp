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
            .EntryStringKey = KEY_SYSTEM_MENU_TOASTS,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableToasts)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_SYSTEM_MENU_PARALLELISM,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableParallelism)
        }
    };

    void Options_SystemMenu_Control()
    {
        const auto& input = g_App.GetInput();

        // Draw graphics.
        OptionsMenu_DrawEntries(KEY_SYSTEM_MENU_HEADING, ENTRIES);
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_System)
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
                    Options_ResetSelection(MainOptionsMenuEntry_System);

                    ScreenFade_Start(true, true, false);
                    Game_StateStepSet(0, OptionsMenuState_LeaveSystem);
                }
                break;
            }
        }
    }
}
