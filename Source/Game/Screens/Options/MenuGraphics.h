#pragma once

#include "Game/Common.h"
#include "Game/Screens/Options/Utils.h"

namespace Silent::Game
{
    /** @brief Draws a notched bar.
     *
     * @param activeCount Active notch count.
     */
    void OptionsMenu_BarDraw(int activeCount);

    /** @brief Draws the heading and all listed entries in the main options menu.
     *
     * @param headingStrKey Heading string translation key.
     * @param entries Entries to draw.
     * @return Previous and current entry string widths in retro pixels.
     */
    std::pair<int, int> OptionsMenu_EntriesDraw(const std::string& headingStrKey,
                                                const std::vector<MenuEntry>& entries);

    /** @brief Draws gold bullet points next to the listed entries and a highlight indicating the
     * selected entry in options menus.
     *
     * @param widths Previous and current entry string widths in retro pixels.
     */
    void OptionsMenu_SelectionHighlightDraw(const std::pair<int, int>& widths);

    /** @brief Draws configuration strings and blue arrows to the right of the listed entries in the main options menu. */
    void OptionsMenu_ConfigDraw();

    /** @brief Draws configuration strings and blue arrows to the right of the listed entries in the extra options menu. */
    void Options_ExtraOptionsMenu_ConfigDraw();
}
