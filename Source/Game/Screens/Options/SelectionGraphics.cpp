#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Options/SelectionGraphics.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Game/Screens/Options/Options.h"
#include "Renderer/Renderer.h"

using namespace Silent::Renderer;

namespace Silent::Game
{
    void Options_Selection_HighlightDraw(const s_Line2d& line)
    {
        constexpr auto COLOR_LINE_START   = Color::From8Bit(176, 176, 176);
        constexpr auto COLOR_LINE_END     = Color::From8Bit(160, 128, 64);
        constexpr auto COLOR_SHADOW_START = Color::From8Bit(96,  96,  96);
        constexpr auto COLOR_SHADOW_END   = Color::From8Bit(0,   0,   0);
        constexpr int  SHADOW_WIDTH       = 16;

        auto& renderer = g_App.GetRenderer();

        // Submit line primitive for underline.
        auto underlinePrim = Shape2d::CreateLine(line.vertex0, line.vertex1,
                                                 COLOR_LINE_START, COLOR_LINE_END,
                                                 DEPTH_36, ScaleMode::VerticalEdge, BlendMode::Opaque);
        renderer.SubmitShape2d(underlinePrim);

        // Submit quad primitive for shadow.
        auto shadowPrim = Shape2d::CreateQuad(Vector2i(line.vertex0.x, line.vertex0.y - SHADOW_WIDTH), line.vertex0,
                                              Vector2i(line.vertex1.x, line.vertex1.y - SHADOW_WIDTH), line.vertex1,
                                              COLOR_SHADOW_END, COLOR_SHADOW_START, COLOR_SHADOW_END, COLOR_SHADOW_START,
                                              DEPTH_36, ScaleMode::VerticalEdge, BlendMode::Subtract);
        renderer.SubmitShape2d(shadowPrim);
    }

    void Options_Selection_ArrowDraw(const s_Triangle2d& tri, bool isFlashing)
    {
        constexpr auto COLOR_FLASH_BASE = Color::From8Bit(0, 112, 255);
        constexpr auto COLOR_CYAN       = Color::From8Bit(0, 240, 240);

        auto& renderer = g_App.GetRenderer();

        // Compute color fade as component in range `[0.0f, 0.5f]`.
        int colorFade = g_SysWork.gameStateCounter & (Q8_COLOR(0.5f) - 1);

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
            color0 = Color::From8Bit(COLOR_FLASH_BASE.R8(), COLOR_FLASH_BASE.G8() + colorEnd, colorEnd, COLOR_FLASH_BASE.B8());
            color1 =
            color2 = Color::From8Bit(COLOR_FLASH_BASE.R8(), COLOR_FLASH_BASE.G8() + colorStart, COLOR_FLASH_BASE.B8());
        }
        // Set solid cyan color.
        else
        {
            color0 =
            color1 =
            color2 = COLOR_CYAN;
        }

        // Submit triangle primitive for arrow.
        auto arrowPrim = Shape2d::CreateTriangle(tri.vertex0, tri.vertex1, tri.vertex2,
                                                     color0, color1, color2,
                                                     DEPTH_40, ScaleMode::VerticalEdge, BlendMode::Opaque);
        renderer.SubmitShape2d(arrowPrim);
    }

    void Options_Selection_BulletPointDraw(const s_Quad2d& quad, bool isBorder, bool isInactive)
    {
        constexpr auto COLOR_ACTIVE_CENTER_HIGHLIGHT = Color::From8Bit(255, 255, 255);
        constexpr auto COLOR_ACTIVE_CENTER_MIDTONE   = Color::From8Bit(160, 128, 64);
        constexpr auto COLOR_ACTIVE_BORDER_HIGHLIGHT = Color::From8Bit(255, 255, 255);
        constexpr auto COLOR_ACTIVE_BORDER_MIDTONE   = Color::From8Bit(160, 128, 64);
        constexpr auto COLOR_ACTIVE_BORDER_SHADOW    = Color::From8Bit(64,  64,  64);

        constexpr auto COLOR_INACTIVE_CENTER_HIGHLIGHT = Color::From8Bit(160, 160, 160);
        constexpr auto COLOR_INACTIVE_CENTER_MIDTONE   = Color::From8Bit(80,  64,  32);
        constexpr auto COLOR_INACTIVE_BORDER_HIGHLIGHT = Color::From8Bit(128, 128, 128);
        constexpr auto COLOR_INACTIVE_BORDER_MIDTONE   = Color::From8Bit(40,  32,  16);
        constexpr auto COLOR_INACTIVE_BORDER_SHADOW    = Color::From8Bit(16,  16,  16);

        auto& renderer = g_App.GetRenderer();

        // Set colors.
        auto colors = std::array<Color, QUAD_VERTEX_COUNT>{};
        if (isBorder)
        {
            colors[0] = isInactive ? COLOR_INACTIVE_BORDER_HIGHLIGHT : COLOR_ACTIVE_BORDER_HIGHLIGHT;
            colors[1] = isInactive ? COLOR_INACTIVE_BORDER_MIDTONE   : COLOR_ACTIVE_BORDER_MIDTONE;
            colors[2] = isInactive ? COLOR_INACTIVE_BORDER_MIDTONE   : COLOR_ACTIVE_BORDER_MIDTONE;
            colors[3] = isInactive ? COLOR_INACTIVE_BORDER_SHADOW    : COLOR_ACTIVE_BORDER_SHADOW;
        }
        else
        {
            colors[0] = isInactive ? COLOR_INACTIVE_CENTER_MIDTONE   : COLOR_ACTIVE_CENTER_MIDTONE;
            colors[1] = isInactive ? COLOR_INACTIVE_CENTER_HIGHLIGHT : COLOR_ACTIVE_CENTER_HIGHLIGHT;
            colors[2] = isInactive ? COLOR_INACTIVE_CENTER_HIGHLIGHT : COLOR_ACTIVE_CENTER_HIGHLIGHT;
            colors[3] = isInactive ? COLOR_INACTIVE_CENTER_MIDTONE   : COLOR_ACTIVE_CENTER_MIDTONE;
        }

        // Submit quad primitive for bullet point element.
        auto elementPrim = Shape2d::CreateQuad(quad.vertex0, quad.vertex1, quad.vertex2, quad.vertex3,
                                               colors[0], colors[1], colors[2], colors[3],
                                               DEPTH_24, ScaleMode::VerticalEdge, BlendMode::Opaque);
        renderer.SubmitShape2d(elementPrim);
    }
}
