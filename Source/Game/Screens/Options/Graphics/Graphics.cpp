#include "Framework.h"
#include "Game/Screens/Options/Graphics/Graphics.h"

#include "Game/Screens/Options/Utils.h"

#include "Application.h"
#include "Input/Input.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Game.h"
#include "Game/Screens/Options/Options.h"
#include "Game/Screens/Options/MenuGraphics.h"
#include "Game/Screens/Options/Utils.h"
#include "Utils/Translator.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Utils;

namespace Silent::Game
{
    static const auto ENTRIES = std::vector<MenuEntry>
    {
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_FULLSCREEN,
            .ConfigStringKeys =
            {
                KEY_OPTIONS_MENU_ON,
                KEY_OPTIONS_MENU_OFF
            }
        },
        MenuEntry
        {
            .Type           = MenuEntryType::Submenu,
            .EntryStringKey = KEY_GRAPHICS_MENU_BRIGHTNESS_LEVEL
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_FRAME_RATE,
            .ConfigStringKeys =
            {
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_ASPECT_RATIO,
            .ConfigStringKeys =
            {
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_RENDER_SCALE,
            .ConfigStringKeys =
            {
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_TEXTURE_FILTER,
            .ConfigStringKeys =
            {
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_TEXT_QUALITY,
            .ConfigStringKeys =
            {
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_LIGHTING,
            .ConfigStringKeys =
            {
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_ANTIALIASING,
            .ConfigStringKeys =
            {
                KEY_OPTIONS_MENU_OFF
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_DITHERING_SCALE,
            .ConfigStringKeys =
            {
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_AMBIENT_OCCLUSION,
            .ConfigStringKeys =
            {
                KEY_OPTIONS_MENU_ON,
                KEY_OPTIONS_MENU_OFF
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_VERTEX_JITTER,
            .ConfigStringKeys =
            {
                KEY_OPTIONS_MENU_ON,
                KEY_OPTIONS_MENU_OFF
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_FILM_FRAIN,
            .ConfigStringKeys =
            {
                KEY_OPTIONS_MENU_ON,
                KEY_OPTIONS_MENU_OFF
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_VIGNETTE,
            .ConfigStringKeys =
            {
                KEY_OPTIONS_MENU_ON,
                KEY_OPTIONS_MENU_OFF
            }
        },
        MenuEntry
        {
            .Type             = MenuEntryType::ArrowConfig,
            .EntryStringKey   = KEY_GRAPHICS_MENU_CRT_FILTER,
            .ConfigStringKeys =
            {
                KEY_OPTIONS_MENU_ON,
                KEY_OPTIONS_MENU_OFF
            }
        }
    };

    void ControlGraphicsOptionsMenu()
    {
        const auto& input      = g_App.GetInput();
        const auto& translator = g_App.GetTranslator();

        // Draw graphics.
        auto widths = OptionsMenu_EntriesDraw(KEY_GRAPHICS_MENU_HEADING, ENTRIES);
        OptionsMenu_SelectionHighlightDraw(widths);
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_Graphics)
        {
            return;
        }

        UpdateOptionsSelection((int)GraphicsMenuEntry::Count);

        // Handle menu state.
        switch (g_GameWork.gameStateSteps[1])
        {
            case 0:
            {
                ResetOptionsSelection();

                ScreenFade_Start(true, true, false);
                Game_StateStepIncrement(1);
                break;
            }
            case 1:
            {
                if (input.GetAction(In::Enter).IsClicked())
                {
                    //Sd_SfxPlay(Sfx_Confirm, 0, Q8(0.25f));

                    Game_StateStepSet(0, OptionsMenuState_EnterBrightness);
                }
                else if (input.GetAction(In::Cancel).IsClicked())
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
                    ResetOptionsSelection(MainOptionsMenuEntry_Graphics);

                    ScreenFade_Start(true, true, false);
                    Game_StateStepSet(0, OptionsMenuState_LeaveGraphics);
                }
                break;
            }
        }
    }
}
