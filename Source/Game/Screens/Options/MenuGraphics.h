#pragma once

#include "Game/Common.h"
#include "Game/Screens/Options/Utils.h"

namespace Silent::Game
{
    /** @brief Submits a notched bar to draw.
     *
     * @param activeCount Active notch count.
     */
    void OptionsMenu_DrawConfigBar(int activeCount);

    /** @brief Submits the heading and all listed entries to draw in the main options menu.
     *
     * @param headingStrKey Heading string translation key.
     * @param entries Entries to draw.
     * @return Previous and current entry string widths in retro pixels.
     */
    std::pair<int, int> OptionsMenu_DrawEntries(const std::string& headingStrKey, const std::vector<MenuEntry>& entries);

    /** @brief Submits gold bullet points next to the listed entries and a highlight indicating the selected entry to
     * draw in options menus.
     *
     * @param widths Previous and current entry string widths in retro pixels.
     */
    void OptionsMenu_DrawSelectionHighlight(const std::pair<int, int>& widths);

    // @deprecated
    /** @brief Draws configuration strings and blue arrows to the right of the listed entries in the main options menu. */
    void OptionsMenu_ConfigDraw();

    // @deprecated
    /** @brief Draws configuration strings and blue arrows to the right of the listed entries in the extra options menu. */
    void Options_ExtraOptionsMenu_ConfigDraw();
}
