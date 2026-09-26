#pragma once

#include "Game/Common.h"

namespace Silent::Game
{
    /** @brief Menu entry types. */
    enum class MenuEntryType
    {
        Submenu,
        ArrowConfig,
        BarConfig
    };

    /** @brief Menu entry data. */
    struct MenuEntry
    {
        MenuEntryType Type            = MenuEntryType::Submenu;
        std::string   EntryStringKey  = {};
        std::string   ConfigStringKey = {};
        int           ConfigPosition  = 0;
    };

    /** @brief Draws a BGM volume bar in the main options menu. */
    void OptionsMenu_BgmVolumeBarDraw();

    /** @brief Draws an SFX volume bar in the main options menu. */
    void OptionsMenu_SfxVolumeBarDraw();

    /** @brief Draws a volume bar.
     *
     * Called by `OptionsMenu_BgmVolumeBarDraw` and `OptionsMenu_SfxVolumeBarDraw`.
     */
    void OptionsMenu_VolumeBarDraw(bool isSfx, uchar vol);

    /** @brief Draws the heading and all listed entry strings in the main options menu. */
    std::pair<float, float> OptionsMenu_EntryStringsDraw();

    /** @brief Draws gold bullet points next to the listed entries and a highlight indicating the
     * selected entry in the main options menu.
     */
    void OptionsMenu_SelectionHighlightDraw(const std::pair<float, float>& widths);

    /** @brief Draws configuration strings and blue arrows to the right of the listed entries in the main options menu. */
    void OptionsMenu_ConfigDraw();

    /** @brief Draws configuration strings and blue arrows to the right of the listed entries in the extra options menu. */
    void Options_ExtraOptionsMenu_ConfigDraw();
}
