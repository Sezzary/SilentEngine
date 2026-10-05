#include "Framework.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Screens/Options/Graphics/Graphics.h"

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
            .EntryStringKey = KEY_GRAPHICS_MENU_FULLSCREEN,
            .Binding        = BindMenuEntryBool(&Options::EnableFullscreen),
            .OnUpdate       = []()
            {
                auto& options = g_App.GetOptions().GetFront();

                options.EnableFullscreen = !options.EnableFullscreen; // @todo Hack, do this properly.
                g_App.ToggleFullscreen();
            }
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_BRIGHTNESS_LEVEL,
            .Binding        = MenuEntrySubmenuBinding{}
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_FRAME_RATE,
            .Binding        = BindMenuEntryEnum(&Options::FrameRate,
            {
                KEY_GRAPHICS_MENU_FRAME_RATE_30_FPS,
                KEY_GRAPHICS_MENU_FRAME_RATE_60_FPS,
                KEY_GRAPHICS_MENU_FRAME_RATE_UNCAPPED
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_ASPECT_RATIO,
            .Binding        = BindMenuEntryEnum(&Options::AspectRatio,
            {
                KEY_GRAPHICS_MENU_ASPECT_RATIO_RETRO,
                KEY_GRAPHICS_MENU_ASPECT_RATIO_WIDE,
                KEY_GRAPHICS_MENU_ASPECT_RATIO_NATIVE
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_RENDER_SCALE,
            .Binding        = BindMenuEntryEnum(&Options::RenderScale,
            {
                KEY_GRAPHICS_MENU_RENDER_SCALE_RETRO,
                KEY_GRAPHICS_MENU_RENDER_SCALE_RETRO_2X,
                KEY_GRAPHICS_MENU_RENDER_SCALE_NATIVE
            }),
            .OnUpdate = []()
            {
                auto& renderer = g_App.GetRenderer();

                renderer.SignalResize();
            }
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_TEXTURE_FILTER,
            .Binding        = BindMenuEntryEnum(&Options::TextureFilter,
            {
                KEY_GRAPHICS_MENU_TEXTURE_FILTER_NEAREST,
                KEY_GRAPHICS_MENU_TEXTURE_FILTER_LINEAR
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_TEXT_QUALITY,
            .Binding        = BindMenuEntryEnum(&Options::TextQuality,
            {
                KEY_GRAPHICS_MENU_TEXT_QUALITY_RETRO,
                KEY_GRAPHICS_MENU_TEXT_QUALITY_MODERN
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_LIGHTING,
            .Binding        = BindMenuEntryEnum(&Options::Lighting,
            {
                KEY_GRAPHICS_MENU_LIGHTING_RETRO,
                KEY_GRAPHICS_MENU_LIGHTING_MODERN
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_ANTIALIASING,
            .Binding        = BindMenuEntryEnum(&Options::Antialiasing,
            {
                KEY_GRAPHICS_MENU_ANTIALIASING_OFF,
                KEY_GRAPHICS_MENU_ANTIALIASING_LOW,
                KEY_GRAPHICS_MENU_ANTIALIASING_HIGH
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_DITHERING_SCALE,
            .Binding        = BindMenuEntryEnum(&Options::DitheringScale,
            {
                KEY_GRAPHICS_MENU_DITHERING_SCALE_OFF,
                KEY_GRAPHICS_MENU_DITHERING_SCALE_RETRO,
                KEY_GRAPHICS_MENU_DITHERING_SCALE_RETRO_2X,
                KEY_GRAPHICS_MENU_DITHERING_SCALE_NATIVE
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_AMBIENT_OCCLUSION,
            .Binding        = BindMenuEntryBool(&Options::EnableAmbientOcclusion)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_VERTEX_JITTER,
            .Binding        = BindMenuEntryBool(&Options::EnableVertexJitter)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_FILM_FRAIN,
            .Binding        = BindMenuEntryBool(&Options::EnableFilmGrain)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_VIGNETTE,
            .Binding        = BindMenuEntryBool(&Options::EnableVignette)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_CRT_FILTER,
            .Binding        = BindMenuEntryBool(&Options::EnableCrtFilter)
        }
    };

    void OptionsMenu_ControlGraphicsMenu()
    {
        const auto& input = g_App.GetInput();

        // Draw graphics.
        OptionsMenu_DrawEntries(KEY_GRAPHICS_MENU_HEADING, ENTRIES);
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_Graphics)
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
                    OptionsMenu_ResetSelection(MainOptionsMenuEntry_Graphics);

                    ScreenFade_Start(true, true, false);
                    Game_StateStepSet(0, OptionsMenuState_LeaveGraphics);
                }
                break;
            }
        }
    }
}
