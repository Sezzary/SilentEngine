#include "Framework.h"
#include "Game/screens/options/menu_graphics.h"

#include "Game/screens/options/options.h"
#include "Game/screens/options/selection_graphics.h"

namespace Silent::Game
{
    void Options_MainOptionsMenu_SelectionHighlightDraw()
    {
        constexpr uint BULLET_QUAD_COUNT  = 2;
        constexpr int  LINE_OFFSET_Y      = 16;
        constexpr int  HIGHLIGHT_OFFSET_X = 39;
        constexpr int  HIGHLIGHT_OFFSET_Y = 58;

        // 12x12 quad.
        constexpr auto BULLET_QUAD_FRONT = s_Quad2d
        {
            .vertex0 = Vector2i(40, 65),
            .vertex1 = Vector2i(40, 77),
            .vertex2 = Vector2i(52, 65),
            .vertex3 = Vector2i(52, 77)
        };

        // 14x14 quad.
        constexpr auto BULLET_QUAD_BACK = s_Quad2d
        {
            .vertex0 = Vector2i(39, 64),
            .vertex1 = Vector2i(39, 76),
            .vertex2 = Vector2i(51, 64),
            .vertex3 = Vector2i(51, 76)
        };

        // @todo Dynamically retrieve string pixel width instead, allowing for automatic translation support.
        constexpr auto SELECTION_HIGHLIGHT_WIDTHS = std::array<int, MainOptionsMenuEntry_Count>
        {
            59, 169, 174, 156, 104, 112, 75, 129, 112
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
        q3_12 interpAlpha = Math_Sin(g_Options_SelectionHighlightTimer << 7);

        // Draw active selection highlight.
        auto highlightLine      = s_Line2d{};
        highlightLine.vertex0.x = HIGHLIGHT_OFFSET_X;
        highlightLine.vertex1.x = g_MainOptions_SelectionHighlightFrom.x +
                                  FP_FROM((g_MainOptions_SelectionHighlightTo.x - g_MainOptions_SelectionHighlightFrom.x) * interpAlpha, Q12_SHIFT);
        highlightLine.vertex1.y = g_MainOptions_SelectionHighlightFrom.y +
                                  FP_FROM((g_MainOptions_SelectionHighlightTo.y - g_MainOptions_SelectionHighlightFrom.y) * interpAlpha, Q12_SHIFT) +
                                  LINE_OFFSET_Y;
        highlightLine.vertex0.y = highlightLine.vertex1.y;
        Options_Selection_HighlightDraw(highlightLine);

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
                Options_Selection_BulletPointDraw(bulletQuads[0], false, false);
                Options_Selection_BulletPointDraw(bulletQuads[1], true,  false);
            }
            // Inactive selection bullet point.
            else
            {
                Options_Selection_BulletPointDraw(bulletQuads[0], false, true);
                Options_Selection_BulletPointDraw(bulletQuads[1], true,  true);
            }
        }
    }
}
