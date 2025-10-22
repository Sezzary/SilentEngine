#include "Framework.h"
#include "Game/include/screens/options/menu_graphics.h"

#include "Game/include/screens/options/options.h"

namespace Silent::Game
{
    void Options_MainOptionsMenu_SelectionHighlightDraw(void)
    {
        constexpr uint BULLET_QUAD_COUNT  = 2;
        constexpr int  LINE_OFFSET_Y      = 16;
        constexpr int  HIGHLIGHT_OFFSET_X = 39;
        constexpr int  HIGHLIGHT_OFFSET_Y = 58;

        static const auto SELECTION_HIGHLIGHT_WIDTHS = std::array<int, MainOptionsMenuEntry_Count>
        {
            59, 169, 174, 156, 104, 112, 75, 129, 112
        };

        // 12x12 quad.
        static const auto BULLET_QUAD_FRONT = s_Quad2d
        {
            .vertex0 = Vector2i(40, 65),
            .vertex1 = Vector2i(40, 77),
            .vertex2 = Vector2i(52, 65),
            .vertex3 = Vector2i(52, 77)
        };

        // 14x14 quad.
        static const auto BULLET_QUAD_BACK = s_Quad2d
        {
            .vertex0 = Vector2i(39, 64),
            .vertex1 = Vector2i(39, 76),
            .vertex2 = Vector2i(51, 64),
            .vertex3 = Vector2i(51, 76)
        };

        // Set active selection highlight position references.
        if (g_Options_SelectionHighlightTimer == 0)
        {
            g_MainOptions_SelectionHighlightFrom.x = SELECTION_HIGHLIGHT_WIDTHS[g_MainOptionsMenu_PrevSelectedEntry] + HIGHLIGHT_OFFSET_X;
            g_MainOptions_SelectionHighlightFrom.y = (g_MainOptionsMenu_PrevSelectedEntry * LINE_OFFSET_Y)           - HIGHLIGHT_OFFSET_Y;
            g_MainOptions_SelectionHighlightTo.x   = SELECTION_HIGHLIGHT_WIDTHS[g_MainOptionsMenu_SelectedEntry]     + HIGHLIGHT_OFFSET_X;
            g_MainOptions_SelectionHighlightTo.y   = (g_MainOptionsMenu_SelectedEntry * LINE_OFFSET_Y)               - HIGHLIGHT_OFFSET_Y;
        }

        // Compute sine-based interpolation alpha.
        short interpAlpha = 0;//Math_Sin(g_Options_SelectionHighlightTimer << 7);

        // Draw active selection highlight.
        auto highlightLine = s_Line2d{};
        //highlightLine.vertex0.x = HIGHLIGHT_OFFSET_X;
        //highlightLine.vertex1.x = g_MainOptions_SelectionHighlightFrom.x +
        //                          FP_FROM((g_MainOptions_SelectionHighlightTo.x - g_MainOptions_SelectionHighlightFrom.x) * interpAlpha, Q12_SHIFT);
        //highlightLine.vertex1.y = g_MainOptions_SelectionHighlightFrom.y +
        //                          FP_FROM((g_MainOptions_SelectionHighlightTo.y - g_MainOptions_SelectionHighlightFrom.y) * interpAlpha, Q12_SHIFT) +
        //                          LINE_OFFSET_Y;
        //highlightLine.vertex0.y = highlightLine.vertex1.y;
        //Options_Selection_HighlightDraw(&highlightLine, true, false);

        // Draw selection bullet points.
        for (int i = 0; i < MainOptionsMenuEntry_Count; i++)
        {
            // Set bullet quads.
            auto bulletQuads = std::array<s_Quad2d, BULLET_QUAD_COUNT>
            {
                BULLET_QUAD_FRONT,
                BULLET_QUAD_BACK
            };
            for (auto& quad : bulletQuads)
            {
                quad.vertex0.y += i * LINE_OFFSET_Y;
                quad.vertex1.y += i * LINE_OFFSET_Y;
                quad.vertex2.y += i * LINE_OFFSET_Y;
                quad.vertex3.y += i * LINE_OFFSET_Y;
            }

            // Active selection bullet point.
            if (i == g_MainOptionsMenu_SelectedEntry)
            {
                Options_Selection_BulletPointDraw(&bulletQuads[0], false, false);
                Options_Selection_BulletPointDraw(&bulletQuads[1], true,  false);
            }
            // Inactive selection bullet point.
            else
            {
                Options_Selection_BulletPointDraw(&bulletQuads[0], false, true);
                Options_Selection_BulletPointDraw(&bulletQuads[1], true,  true);
            }
        }
    }

    void Options_Selection_ArrowDraw(const s_Triangle2d* arrow, bool isFlashing, bool resetColor)
    {
        // Fade start color.
        int colorFade = 0;//g_SysWork.timer_1C & 0x7F;

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

        // Draw blue arrow.
        //arrowPoly = (POLY_G3*)GsOUT_PACKET_P;
        //setPolyG3(arrowPoly);

        // Flash color from blue to cyan.
        if (isFlashing)
        {
            // 48, 255, colorEnd
            // 48, 255, colorStart
            // 48, 255, colorStart
        }
        // Set solid cyan color.
        else
        {
            //setRGBC0(arrowPoly, 0, 240, 240);
            //setRGBC1(arrowPoly, 0, 240, 240);
            //setRGBC2(arrowPoly, 0, 240, 240);
        }

        //setXY0Fast(arrowPoly, arrow->vertex0.x, arrow->vertex0.y);
        //setXY1Fast(arrowPoly, arrow->vertex1.x, arrow->vertex1.y);
        //setXY2Fast(arrowPoly, arrow->vertex2.x, arrow->vertex2.y);
        //addPrim((u8*)ot->org + LAYER_40, arrowPoly);
        //GsOUT_PACKET_P = (u8*)arrowPoly + sizeof(POLY_G3);
    }

    void Options_Selection_BulletPointDraw(const s_Quad2d* quad, bool isCenter, bool isInactive)
    {
        constexpr uint QUAD_COUNT = 2;

        // Draw quads as triangles with diagonal gradient.
        for (int i = 0; i < QUAD_COUNT; i++)
        {
            //poly = (POLY_G3*)GsOUT_PACKET_P;
            //setPolyG3(poly);

            // Center quad.
            if (isCenter)
            {
                // Set color.
                switch (isInactive)
                {
                    case false:
                        //setRGBC0(poly, 255, 255, 255, 0x30);
                        //setRGBC1(poly, 160, 128, 64, 0x30);
                        //setRGBC2(poly, 64, 64, 64, 0x30);
                        break;

                    case true:
                        //setRGBC0(poly, 128, 128, 128, 0x30);
                        //setRGBC1(poly, 40, 32, 16, 0x30);
                        //setRGBC2(poly, 16, 16, 16, 0x30);
                        break;
                }

                // Draw triangle.
                if (i != 0)
                {
                    //setXY0Fast(poly, quad->vertex0.x, quad->vertex0.y);
                    //setXY1Fast(poly, quad->vertex1.x, quad->vertex1.y);
                    //setXY2Fast(poly, quad->vertex3.x, quad->vertex3.y);
                }
                else
                {
                    //setXY0Fast(poly, quad->vertex0.x, quad->vertex0.y);
                    //setXY1Fast(poly, quad->vertex2.x, quad->vertex2.y);
                    //setXY2Fast(poly, quad->vertex3.x, quad->vertex3.y);
                }
            }
            // Border quad.
            else
            {
                // Set color.
                switch (isInactive)
                {
                    case false:
                        //setRGBC0(poly, 160, 128, 64, 0x30);
                        //setRGBC1(poly, 255, 255, 255, 0x30);
                        //setRGBC2(poly, 255, 255, 255, 0x30);
                        break;

                    case true:
                        //setRGBC0(poly, 80, 64, 32, 0x30);
                        //setRGBC1(poly, 160, 160, 160, 0x30);
                        //setRGBC2(poly, 160, 160, 160, 0x30);
                        break;
                }

                // Draw triangle.
                if (i != 0)
                {
                    //setXY0Fast(poly, quad->vertex3_C.x, quad->vertex3_C.y);
                    //setXY1Fast(poly, quad->vertex1_4.x, quad->vertex1_4.y);
                    //setXY2Fast(poly, quad->vertex2_8.x, quad->vertex2_8.y);
                }
                else
                {
                    //setXY0Fast(poly, quad->vertex0_0.x, quad->vertex0_0.y);
                    //setXY1Fast(poly, quad->vertex1_4.x, quad->vertex1_4.y);
                    //setXY2Fast(poly, quad->vertex2_8.x, quad->vertex2_8.y);
                }
            }

            //addPrim((u8*)ot->org + LAYER_24, poly);
            //GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_G3);
        }
    }
}