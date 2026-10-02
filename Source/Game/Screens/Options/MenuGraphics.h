#pragma once

#include "Game/Common.h"
#include "Game/Screens/Options/Utils.h"

namespace Silent::Game
{
    /** @brief Draws a BGM volume bar in the main options menu. */
    void OptionsMenu_BgmVolumeBarDraw();

    /** @brief Draws an SFX volume bar in the main options menu. */
    void OptionsMenu_SfxVolumeBarDraw();

    /** @brief Draws a volume bar.
     *
     * Called by `OptionsMenu_BgmVolumeBarDraw` and `OptionsMenu_SfxVolumeBarDraw`.
     */
    void OptionsMenu_VolumeBarDraw(bool isSfx, int vol);

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
