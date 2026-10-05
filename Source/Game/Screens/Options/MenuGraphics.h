#pragma once

#include "Game/Common.h"
#include "Game/Screens/Options/Utils.h"

namespace Silent::Game
{
    /** @brief Submits the heading and all listed entries to draw in the main options menu.
     *
     * @param headingStrKey Heading string translation key.
     * @param entries Entries to draw.
     */
    void OptionsMenu_DrawEntries(const std::string& headingStrKey, const std::vector<MenuEntry>& entries);

    // @deprecated
    /** @brief Draws configuration strings and blue arrows to the right of the listed entries in the main options menu. */
    void OptionsMenu_ConfigDraw();

    // @deprecated
    /** @brief Draws configuration strings and blue arrows to the right of the listed entries in the extra options menu. */
    void Options_ExtraOptionsMenu_ConfigDraw();
}
