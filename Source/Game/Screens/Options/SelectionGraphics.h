#pragma once

#include "Game/Common.h"

namespace Silent::Game
{
    /** @brief Draws a scaling entry selection highlight in the main and extra options menus.
     *
     * Called by `Options_MainOptionsMenu_SelectionHighlightDraw` and `Options_ExtraOptionsMenu_SelectionHighlightDraw`.
     *
     * @param line 2D line for the highlight underline and shadow.
     * @param hasShadow `true` if drawing the highlight with a shadow and a line, `false` for the line only.
     */
    void Options_Selection_HighlightDraw(const s_Line2d& line);

    /** @brief Draws a blue arrow element used for certain listed entries in the main and extra options menus.
     *
     * @note Called twice if the arrow requires a border with `isFlashing` passed as `false` in the second call.
     *
     * @param tri 2D triangle of the arrow element.
     * @param isFlashing `true` if drawing the flashing element with a gradient, `false` if drawing the border.
     */
    void Options_Selection_ArrowDraw(const s_Triangle2d& tri, bool isFlashing);

    /** @brief Draws a gold bullet point element used next to listed entries in the main and extra options menus.
     *
     * @note Called twice to build a whole bullet point with `isBorder` passed as `true` and `false` on consecutive calls.
     *
     * @param quad 2D quad of the bullet point element.
     * @param isBorder `true` if drawing the border quad, `false` if drawing the quad.
     * @param isInactive `true` if the entry is unselected, `false` if selected.
     */
    void Options_Selection_BulletPointDraw(const s_Quad2d& quad, bool isBorder, bool isInactive);
}
