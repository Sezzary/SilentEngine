#pragma once

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
        MenuEntryType            Type             = MenuEntryType::Submenu;
        std::string              EntryStringKey   = {};
        std::vector<std::string> ConfigStringKeys = {};
    };

    void UpdateOptionsSelection(int entryCount);

    void ResetOptionsSelection(int selectedEntryIdx = 0);
}
