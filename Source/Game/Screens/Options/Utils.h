#pragma once

namespace Silent::Game
{
    constexpr q19_12 LINE_CURSOR_TIMER_MAX = Q12(1 / 8.0f);

    /** @brief Menu entry types. */
    enum class MenuEntryType
    {
        Submenu,
        Boolean, // @todo Use to simplify some list entries.
        List,
        Bar
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
