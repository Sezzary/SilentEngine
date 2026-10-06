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
    void Options_DrawEntries(const std::string& headingStrKey, const std::vector<MenuEntry>& entries);
}
