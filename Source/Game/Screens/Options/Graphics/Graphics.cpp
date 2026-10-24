#include "Framework.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Screens/Options/Graphics/Graphics.h"

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
            .EntryStringKey = KEY_GRAPHICS_MENU_FULLSCREEN,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableFullscreen),
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
            .Binding        = MenuEntrySubmenuBinding
            {
                .MenuState = OptionsMenuState_EnterBrightness
            }
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_FRAME_RATE,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::FrameRate,
            {
                KEY_GRAPHICS_MENU_FRAME_RATE_30_FPS,
                KEY_GRAPHICS_MENU_FRAME_RATE_60_FPS,
                KEY_GRAPHICS_MENU_FRAME_RATE_UNCAPPED
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_ASPECT_RATIO,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::AspectRatio,
            {
                KEY_GRAPHICS_MENU_ASPECT_RATIO_RETRO,
                KEY_GRAPHICS_MENU_ASPECT_RATIO_WIDE,
                KEY_GRAPHICS_MENU_ASPECT_RATIO_NATIVE
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_RENDER_SCALE,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::RenderScale,
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
            .Binding        = MenuEntryEnumBinding::Bind(&Options::TextureFilter,
            {
                KEY_GRAPHICS_MENU_TEXTURE_FILTER_NEAREST,
                KEY_GRAPHICS_MENU_TEXTURE_FILTER_LINEAR
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_TEXT_QUALITY,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::TextQuality,
            {
                KEY_GRAPHICS_MENU_TEXT_QUALITY_RETRO,
                KEY_GRAPHICS_MENU_TEXT_QUALITY_MODERN
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_LIGHTING,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::Lighting,
            {
                KEY_GRAPHICS_MENU_LIGHTING_RETRO,
                KEY_GRAPHICS_MENU_LIGHTING_MODERN
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_ANTIALIASING,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::Antialiasing,
            {
                KEY_GRAPHICS_MENU_ANTIALIASING_OFF,
                KEY_GRAPHICS_MENU_ANTIALIASING_LOW,
                KEY_GRAPHICS_MENU_ANTIALIASING_HIGH
            })
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_DITHERING_SCALE,
            .Binding        = MenuEntryEnumBinding::Bind(&Options::DitheringScale,
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
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableAmbientOcclusion)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_VERTEX_JITTER,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableVertexJitter)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_FILM_GRAIN,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableFilmGrain)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_VIGNETTE,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableVignette)
        },
        MenuEntry
        {
            .EntryStringKey = KEY_GRAPHICS_MENU_CRT_FILTER,
            .Binding        = MenuEntryBoolBinding::Bind(&Options::EnableCrtFilter)
        }
    };

    void Options_ControlGraphicsMenu()
    {
        Options_ControlSubmenu(KEY_GRAPHICS_MENU_HEADING, ENTRIES, 1,
                               OptionsMenuState_Graphics, OptionsMenuState_LeaveGraphics,
                               MainOptionsMenuEntry_Graphics);
    }
}
