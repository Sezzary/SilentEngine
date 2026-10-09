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
     * Called by `Options_DrawSelectionHighlight`.
     *
     * @param line 2D line for the highlight underline and shadow.
     * @param hasShadow `true` for a highlight with a shadow and a line, `false` for a line only. Always passed as `true`.
     */
    void Options_DrawHighlight(const s_Line2d& line);

    /** @brief Draws a blue flashing arrow used for configs of certain listed entries in options menus.
     *
     * @param pos Screen position in retro pixels (320x240 resolution).
     * @param type Arrow type.
     * @param hasOutline `true` if the arrow has an outline, `false` otherwise.
     */
    void Options_DrawArrow(const Vector2i& pos, SelectionArrowType type, bool hasOutline);

    /** @brief Draws a gold bullet point used next to listed entries in options menus.
     *
     * @param pos Position in retro pixels (320x240 resolution).
     * @param isActive `true` if the associated entry is selected, `false` otherwise.
     */
    void Options_DrawBulletPoint(const Vector2i& pos, bool isActive);
}
