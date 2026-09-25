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

    void Options_Selection_ArrowDraw(const s_Triangle2d& tri, bool hasOutline)
    {
        constexpr auto COLOR_FLASH = Color::From8Bit(0, 112, 255);
        constexpr auto COLOR_CYAN  = Color::From8Bit(0, 240, 240);

        auto& renderer = g_App.GetRenderer();

        // Compute color fade as component in range `[0, 128]`.
        int colorFade = (g_SysWork.gameStateCounter * 2) % 128;

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

        // Set flashing blue-cyan color. @todo Use constant.
        if (hasOutline)
        {
            color0 = Color::From8Bit(0, 112 + (colorEnd * 4), 255);
            color1 =
            color2 = Color::From8Bit(0, 112 + (colorStart * 4), 255);
        }
        // Set solid cyan color.
        else
        {
            color0 =
            color1 =
            color2 = COLOR_CYAN;
        }

        // Submit triangle primitive for arrow.
        int  depth     = DEPTH_40 + (hasOutline ? 0 : 1);
        auto arrowPrim = Shape2d::CreateTriangle(tri.vertex0, tri.vertex1, tri.vertex2,
                                                 color0, color1, color2,
                                                 depth, ScaleMode::VerticalEdge, BlendMode::Opaque);
        renderer.SubmitShape2d(arrowPrim);
    }

    void Options_Selection_ArrowDraw(const Vector2i& pos, SelectionArrowType type, bool hasOutline)
    {
        constexpr int  DEPTH       = 40;
        constexpr auto COLOR_FLASH = Color::From8Bit(0, 112, 255);
        constexpr auto COLOR_CYAN  = Color::From8Bit(0, 240, 240);
        constexpr auto BASE_ARROWS = std::array<s_Triangle2d, (int)SelectionArrowType::Count>
        {
            s_Triangle2d
            {
                Vector2i( 0, -8),
                Vector2i(-8,  0),
                Vector2i( 8,  0)
            },
            s_Triangle2d
            {
                Vector2i( 0, 8),
                Vector2i(-8, 0),
                Vector2i( 8, 0)
            },
            s_Triangle2d
            {
                Vector2i(-8, -8),
                Vector2i( 0, -16),
                Vector2i( 0,  0)
            },
            s_Triangle2d
            {
                Vector2i(8, -8),
                Vector2i(0, -16),
                Vector2i(0,  0)
            }
        };

        // Compute color fade as component in range `[0, 128]`.
        int colorFade = (g_SysWork.gameStateCounter * 2) % 128;

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

        // Set flashing blue-cyan color. @todo Use constant. Base blue is wrong?
        color0 = Color::From8Bit(0, 112 + (colorEnd * 4), 255);
        color1 =
        color2 = Color::From8Bit(0, 112 + (colorStart * 4), 255);

        const auto& baseArrow  = BASE_ARROWS[(int)type];
        auto        flashArrow = s_Triangle2d
        {
            pos + baseArrow.vertex0,
            pos + baseArrow.vertex1,
            pos + baseArrow.vertex2
        };

        auto SubmitArrow = [&](const s_Triangle2d& tri, const Color& color0, const Color& color1, const Color& color2,
                               bool isOutline)
        {
            auto& renderer = g_App.GetRenderer();

            // Submit triangle primitive for arrow.
            int  depth = DEPTH + (isOutline ? 1 : 0);
            auto prim  = Shape2d::CreateTriangle(tri.vertex0, tri.vertex1, tri.vertex2,
                                                 color0, color1, color2,
                                                 depth, ScaleMode::VerticalEdge, BlendMode::Opaque);
            renderer.SubmitShape2d(prim);
        };

        // Submit flashing arrow.
        SubmitArrow(flashArrow, color0, color1, color2, false);

        // Submit flashing outline.
        if (hasOutline)
        {
            switch (type)
            {
                case SelectionArrowType::Up:
                {
                    auto bgArrow = s_Triangle2d
                    {
                        flashArrow.vertex0 + Vector2i( 0, -1),
                        flashArrow.vertex1 + Vector2i(-2,  1),
                        flashArrow.vertex2 + Vector2i( 2,  1)
                    };
                    SubmitArrow(bgArrow, COLOR_CYAN, COLOR_CYAN, COLOR_CYAN, true);
                    break;
                }
                case SelectionArrowType::Down:
                {
                    auto bgArrow = s_Triangle2d
                    {
                        flashArrow.vertex0 + Vector2i( 0, -1),
                        flashArrow.vertex1 + Vector2i(-2,  1),
                        flashArrow.vertex2 + Vector2i( 2,  1)
                    };
                    SubmitArrow(bgArrow, COLOR_CYAN, COLOR_CYAN, COLOR_CYAN, true);
                    break;
                }
                case SelectionArrowType::Left:
                {
                    auto bgArrow = s_Triangle2d
                    {
                        flashArrow.vertex0 + Vector2i(-1,  0),
                        flashArrow.vertex1 + Vector2i( 1, -2),
                        flashArrow.vertex2 + Vector2i( 1,  2)
                    };
                    SubmitArrow(bgArrow, COLOR_CYAN, COLOR_CYAN, COLOR_CYAN, true);
                    break;
                }
                case SelectionArrowType::Right:
                {
                    auto bgArrow = s_Triangle2d
                    {
                        flashArrow.vertex0 + Vector2i( 1,  0),
                        flashArrow.vertex1 + Vector2i(-1, -2),
                        flashArrow.vertex2 + Vector2i(-1,  2)
                    };
                    SubmitArrow(bgArrow, COLOR_CYAN, COLOR_CYAN, COLOR_CYAN, true);
                    break;
                }
            }
        }
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
