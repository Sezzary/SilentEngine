#include "Framework.h"
#include "Game/include/screens/options/selection_graphics.h"

#include "Application.h"
#include "Game/include/screens/options/options.h"
#include "Renderer/Renderer.h"

#include "Game/dummy.h"

using namespace Silent::Renderer;

namespace Silent::Game
{
    void Options_Selection_HighlightDraw(const s_Line2d& line, bool hasShadow, bool invertGradient)
    {
        // @todo
    }

    void Options_Selection_ArrowDraw(const s_Triangle2d& arrow, bool isFlashing, bool resetColor)
    {
        constexpr auto COLOR_FLASH_BASE = Color::From8Bit(0, 112, 255);
        constexpr auto COLOR_CYAN       = Color::From8Bit(0, 240, 240);

        auto& renderer = g_App.GetRenderer();

        // Compute color fade as component in range `[0.0f, 0.5f]`.
        int colorFade = g_SysWork.timer_1C & (FP_COLOR(0.5f) - 1);

        // Fade start color.
        int colorStart = 0;
        if (colorFade >= 32)
        {
            colorStart = 32;
            if (colorFade < 64)
            {
                colorStart = 32;
            }
            else if (colorFade < 96)
            {
                colorStart = 96 - colorFade;
            }
            else
            {
                colorStart = 0;
            }
        }
        else
        {
            colorStart = colorFade;
        }

        // Fade end color.
        int colorEnd = 0;
        if (colorFade >= 32)
        {
            if (colorFade < 64)
            {
                colorEnd = colorFade - 32;
            }
            else if (colorFade >= 96)
            {
                colorEnd = 128 - colorFade;
            }
            else
            {
                colorEnd = 32;
            }
        }
        else
        {
            colorEnd = 0;
        }

        auto color0 = Color::Clear;
        auto color1 = Color::Clear;
        auto color2 = Color::Clear;

        // Set flashing blue-cyan color.
        if (isFlashing)
        {
            color0 = Color::From8Bit(COLOR_FLASH_BASE.R8(), COLOR_FLASH_BASE.R8() + colorEnd, colorEnd, COLOR_FLASH_BASE.B8());
            color1 =
            color2 = Color::From8Bit(COLOR_FLASH_BASE.R8(), COLOR_FLASH_BASE.R8() + colorStart, COLOR_FLASH_BASE.B8());
        }
        // Set solid cyan color.
        else
        {
            color0 =
            color1 =
            color2 = COLOR_CYAN;
        }

        // Submit triangle primitive.
        auto prim = Primitive2d::CreateTriangle(arrow.vertex0, arrow.vertex1, arrow.vertex2,
                                                color0, color1, color2,
                                                DEPTH_40);
        renderer.Submit2dPrimitive(prim);
    }

    void Options_Selection_BulletPointDraw(const s_Quad2d& quad, bool isBorder, bool isInactive)
    {
        constexpr auto COLOR_ACTIVE_CENTER_HIGHLIGHT   = Color::From8Bit(255, 255, 255);
        constexpr auto COLOR_ACTIVE_CENTER_MIDTONE     = Color::From8Bit(160, 128, 64);
        constexpr auto COLOR_ACTIVE_BORDER_HIGHLIGHT   = Color::From8Bit(255, 255, 255);
        constexpr auto COLOR_ACTIVE_BORDER_MIDTONE     = Color::From8Bit(160, 128, 64);
        constexpr auto COLOR_ACTIVE_BORDER_SHADOW      = Color::From8Bit(64,  64,  64);

        constexpr auto COLOR_INACTIVE_CENTER_HIGHLIGHT = Color::From8Bit(160, 160, 160);
        constexpr auto COLOR_INACTIVE_CENTER_MIDTONE   = Color::From8Bit(80,  64,  32);
        constexpr auto COLOR_INACTIVE_BORDER_HIGHLIGHT = Color::From8Bit(128, 128, 128);
        constexpr auto COLOR_INACTIVE_BORDER_MIDTONE   = Color::From8Bit(40,  32,  16);
        constexpr auto COLOR_INACTIVE_BORDER_SHADOW    = Color::From8Bit(16,  16,  16);

        auto& renderer = g_App.GetRenderer();

        auto color0 = Color::Clear;
        auto color1 = Color::Clear;
        auto color2 = Color::Clear;
        auto color3 = Color::Clear;

        // Set color.
        if (isBorder)
        {
            color0 = isInactive ? COLOR_INACTIVE_BORDER_HIGHLIGHT : COLOR_ACTIVE_BORDER_HIGHLIGHT;
            color1 = isInactive ? COLOR_INACTIVE_BORDER_MIDTONE   : COLOR_ACTIVE_BORDER_MIDTONE;
            color2 = isInactive ? COLOR_INACTIVE_BORDER_MIDTONE   : COLOR_ACTIVE_BORDER_MIDTONE;
            color3 = isInactive ? COLOR_INACTIVE_BORDER_SHADOW    : COLOR_ACTIVE_BORDER_SHADOW;
        }
        else
        {
            color0 = isInactive ? COLOR_INACTIVE_CENTER_MIDTONE   : COLOR_ACTIVE_CENTER_MIDTONE;
            color1 = isInactive ? COLOR_INACTIVE_CENTER_HIGHLIGHT : COLOR_ACTIVE_CENTER_HIGHLIGHT;
            color2 = isInactive ? COLOR_INACTIVE_CENTER_HIGHLIGHT : COLOR_ACTIVE_CENTER_HIGHLIGHT;
            color3 = isInactive ? COLOR_INACTIVE_CENTER_MIDTONE   : COLOR_ACTIVE_CENTER_MIDTONE;
        }
        
        // Submit quad primitive.
        auto prim = Primitive2d::CreateQuad(quad.vertex0, quad.vertex1, quad.vertex2, quad.vertex3,
                                            color0, color1, color2, color3,
                                            DEPTH_24);
        renderer.Submit2dPrimitive(prim);
    }
}
