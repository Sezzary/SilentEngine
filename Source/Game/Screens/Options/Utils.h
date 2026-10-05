#pragma once

#include "Application.h"
#include "Services/Options.h"

using namespace Silent::Services;

namespace Silent::Game
{
    constexpr q19_12 LINE_CURSOR_TIMER_MAX = Q12(1 / 8.0f);
    constexpr int    BAR_NOTCH_COUNT       = 16;

    struct MenuEntrySubmenuBinding
    {
    };

    struct MenuEntryBoolBinding
    {
        std::function<bool()>     GetState = nullptr;
        std::function<void(bool)> SetState = nullptr;

        static MenuEntryBoolBinding Bind(bool Options::* field);
    };

    struct MenuEntryRangeBinding
    {
        std::function<int()>     GetValue = nullptr;
        std::function<void(int)> SetValue = nullptr;
        int                      Min      = 0;
        int                      Max      = 0;

        static MenuEntryRangeBinding Bind(int Options::* field, int min, int max);
    };

    struct MenuEntryEnumBinding
    {
        std::function<int()>     GetIdx           = nullptr;
        std::function<void(int)> SetIdx           = nullptr;
        std::vector<std::string> ConfigStringKeys = {};

        template <typename EnumT>
        static MenuEntryEnumBinding Bind(EnumT Options::* field, std::vector<std::string> configStrKeys);
    };

    struct MenuEntryBarBinding
    {
        std::function<int()>     GetValue = nullptr;
        std::function<void(int)> SetValue = nullptr;
        int                      Max      = 0;

        static MenuEntryBarBinding Bind(int Options::* field, int max);
    };

    struct MenuEntryLangugeBinding
    {
        std::function<int()>                      GetIdx     = nullptr;
        std::function<void(int)>                  SetIdx     = nullptr;
        std::function<std::vector<std::string>()> GetStrings = nullptr;

        static MenuEntryLangugeBinding Bind(std::string Options::* field);
    };

    using MenuEntryBinding = std::variant<MenuEntrySubmenuBinding,
                                          MenuEntryBoolBinding,
                                          MenuEntryRangeBinding,
                                          MenuEntryEnumBinding,
                                          MenuEntryBarBinding,
                                          MenuEntryLangugeBinding>;

    /** @brief Menu entry data. */
    struct MenuEntry
    {
        std::string           EntryStringKey = {};
        MenuEntryBinding      Binding        = {};
        std::function<void()> OnUpdate       = nullptr;
    };

    template <typename EnumT>
    MenuEntryEnumBinding MenuEntryEnumBinding::Bind(EnumT Options::* field, std::vector<std::string> configStrKeys)
    {
        return MenuEntryEnumBinding
        {
            .GetIdx = [field]()
            {
                const auto& options = g_App.GetOptions().GetBack();
                return (int)(options.*field);
            },
            .SetIdx = [field](int idx)
            {
                auto& options  = g_App.GetOptions().GetFront();
                options.*field = WrapEnum<EnumT>(idx);
            },
            .ConfigStringKeys = std::move(configStrKeys)
        };
    }

    void Options_UpdateConfig(const std::vector<MenuEntry>& entries);

    void Options_UpdateSelection(int entryCount);

    void Options_ResetSelection(int selectedEntryIdx = 0);
}
