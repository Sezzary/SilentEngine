#pragma once

#include "Application.h"
#include "Services/Options.h"

using namespace Silent::Services;

namespace Silent::Game
{
    constexpr q19_12 LINE_CURSOR_TIMER_MAX = Q12(1 / 8.0f);

    struct MenuEntrySubmenuBinding
    {
    };

    struct MenuEntryBoolBinding
    {
        std::function<bool()>     GetState = nullptr;
        std::function<void(bool)> SetState = nullptr;
    };

    struct MenuEntryRangeBinding
    {
        std::function<int()>     GetValue = nullptr;
        std::function<void(int)> SetValue = nullptr;
    };

    struct MenuEntryEnumBinding
    {
        std::function<int()>     GetIdx           = nullptr;
        std::function<void(int)> SetIdx           = nullptr;
        std::vector<std::string> ConfigStringKeys = {};
    };

    struct MenuEntryBarBinding
    {
        std::function<int()>     GetValue = nullptr;
        std::function<void(int)> SetValue = nullptr;
    };

    using MenuEntryBinding = std::variant<MenuEntrySubmenuBinding,
                                          MenuEntryBoolBinding,
                                          MenuEntryRangeBinding,
                                          MenuEntryEnumBinding,
                                          MenuEntryBarBinding>;

    /** @brief Menu entry data. */
    struct MenuEntry
    {
        std::string           EntryStringKey = {};
        MenuEntryBinding      Binding        = {};
        std::function<void()> OnUpdate       = nullptr;

    };

    inline MenuEntryBoolBinding BindMenuEntryBool(bool Options::* field)
    {
        return MenuEntryBoolBinding
        {
            .GetState = [field]()
            {
                const auto& options = g_App.GetOptions().GetBack();
                return options.*field;
            },
            .SetState = [field](bool state)
            {
                auto& options = g_App.GetOptions().GetFront();
                options.*field = state;
            }
        };
    }

    inline MenuEntryRangeBinding BindMenuEntryRange(int Options::* field, int min, int max)
    {
        return MenuEntryRangeBinding
        {
            .GetValue = [field]()
            {
                const auto& options = g_App.GetOptions().GetBack();
                return options.*field;
            },
            .SetValue = [field, min, max](int val)
            {
                auto& options  = g_App.GetOptions().GetFront();
                options.*field = WrapRange(val, min, max);
            }
        };
    }

    template <typename EnumT>
    inline MenuEntryEnumBinding BindMenuEntryEnum(EnumT Options::* field, std::vector<std::string> configStrKeys)
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

    void OptionsMenu_UpdateConfig(const std::vector<MenuEntry>& entries);

    void OptionsMenu_UpdateSelection(int entryCount);

    void OptionsMenu_ResetSelection(int selectedEntryIdx = 0);
}
