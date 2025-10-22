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
            .vertex0_0 = { 40, 65 },
            .vertex1_4 = { 40, 77 },
            .vertex2_8 = { 52, 65 },
            .vertex3_C = { 52, 77 }
        };

        // 14x14 quad.
        static const auto BULLET_QUAD_BACK = s_Quad2d
        {
            .vertex0_0 = { 39, 64 },
            .vertex1_4 = { 39, 76 },
            .vertex2_8 = { 51, 64 },
            .vertex3_C = { 51, 76 }
        };

        // Set active selection highlight position references.
        if (g_Options_SelectionHighlightTimer == 0)
        {
            g_MainOptions_SelectionHighlightFrom.vx = SELECTION_HIGHLIGHT_WIDTHS[g_MainOptionsMenu_PrevSelectedEntry] + HIGHLIGHT_OFFSET_X;
            g_MainOptions_SelectionHighlightFrom.vy = (g_MainOptionsMenu_PrevSelectedEntry * LINE_OFFSET_Y)           - HIGHLIGHT_OFFSET_Y;
            g_MainOptions_SelectionHighlightTo.vx   = SELECTION_HIGHLIGHT_WIDTHS[g_MainOptionsMenu_SelectedEntry]     + HIGHLIGHT_OFFSET_X;
            g_MainOptions_SelectionHighlightTo.vy   = (g_MainOptionsMenu_SelectedEntry * LINE_OFFSET_Y)               - HIGHLIGHT_OFFSET_Y;
        }

        // Compute sine-based interpolation alpha.
        short interpAlpha = 0;//Math_Sin(g_Options_SelectionHighlightTimer << 7);

        // Draw active selection highlight.
        auto highlightLine = s_Line2d{};
        //highlightLine.vertex0_0.vx = HIGHLIGHT_OFFSET_X;
        //highlightLine.vertex1_4.vx = g_MainOptions_SelectionHighlightFrom.vx +
        //                             FP_FROM((g_MainOptions_SelectionHighlightTo.vx - g_MainOptions_SelectionHighlightFrom.vx) * interpAlpha, Q12_SHIFT);
        //highlightLine.vertex1_4.vy = g_MainOptions_SelectionHighlightFrom.vy +
        //                             FP_FROM((g_MainOptions_SelectionHighlightTo.vy - g_MainOptions_SelectionHighlightFrom.vy) * interpAlpha, Q12_SHIFT) +
        //                             LINE_OFFSET_Y;
        //highlightLine.vertex0_0.vy = highlightLine.vertex1_4.vy;
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
                quad.vertex0_0.vy += i * LINE_OFFSET_Y;
                quad.vertex1_4.vy += i * LINE_OFFSET_Y;
                quad.vertex2_8.vy += i * LINE_OFFSET_Y;
                quad.vertex3_C.vy += i * LINE_OFFSET_Y;
            }

            // Active selection bullet point.
            if (i == g_MainOptionsMenu_SelectedEntry)
            {
                //Options_Selection_BulletPointDraw(&bulletQuads[0], false, false);
                //Options_Selection_BulletPointDraw(&bulletQuads[1], true,  false);
            }
            // Inactive selection bullet point.
            else
            {
                //Options_Selection_BulletPointDraw(&bulletQuads[0], false, true);
                //Options_Selection_BulletPointDraw(&bulletQuads[1], true,  true);
            }
        }
    }
}