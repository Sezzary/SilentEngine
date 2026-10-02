#pragma once

#include "Game/Common.h"

namespace Silent::Game
{
    /** @brief Selection arrow types. */
    enum class SelectionArrowType
    {
        Up,
        Down,
        Left,
        Right,

        Count
    };

    /** @brief Draws a scaling entry selection highlight in the main and extra options menus.
     *
     * Called by `OptionsMenu_SelectionHighlightDraw`.
     *
     * @param line 2D line for the highlight underline and shadow.
     * @param hasShadow `true` for a highlight with a shadow and a line, `false` for a line only. Always passed as `true`.
     */
    void Options_Selection_HighlightDraw(const s_Line2d& line);

    // @deprecated
    /** @brief Draws a blue arrow element used for certain listed entries in the main and extra options menus.
     *
     * @note Called twice if the arrow requires a border, with `isFlashing` passed as `true` and `false` on consecutive
     * calls.
     *
     * @param tri 2D triangle of the arrow element.
     * @param isFlashing `true` for a flashing element with a gradient, `false` for a border.
     */
    void Options_Selection_ArrowDraw(const s_Triangle2d& tri, bool isFlashing);

    /** @brief Draws a blue flashing arrow used for configs of certain listed entries in options menus.
     *
     * @param pos Screen position in retro pixels (320x240 resolution).
     * @param type Arrow type.
     * @param hasOutline `true` if the arrow has an outline, `false` otherwise.
     */
    void Options_Selection_ArrowDraw(const Vector2i& pos, SelectionArrowType type, bool hasOutline);

    /** @brief Draws a gold bullet point used next to listed entries in options menus.
     *
     * @param pos Position in retro pixels (320x240 resolution).
     * @param isActive `true` if the associated entry is selected, `false` otherwise.
     */
    void Options_Selection_BulletPointDraw(const Vector2i& pos, bool isActive);
}
