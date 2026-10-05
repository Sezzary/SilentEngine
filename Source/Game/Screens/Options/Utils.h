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
    };

    struct MenuEntryRangeBinding
    {
        std::function<int()>     GetValue = nullptr;
        std::function<void(int)> SetValue = nullptr;
        int                      Min      = 0;
        int                      Max      = 0;
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
        int                      Max      = 0;
    };

    struct MenuEntryStringsBinding
    {
        std::function<int()>                      GetIdx     = nullptr;
        std::function<void(int)>                  SetIdx     = nullptr;
        std::function<std::vector<std::string>()> GetStrings = nullptr;
    };

    using MenuEntryBinding = std::variant<MenuEntrySubmenuBinding,
                                          MenuEntryBoolBinding,
                                          MenuEntryRangeBinding,
                                          MenuEntryEnumBinding,
                                          MenuEntryBarBinding,
                                          MenuEntryStringsBinding>;

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
            },
            .Max = max
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

    inline MenuEntryBarBinding BindMenuEntryBar(int Options::* field, int max)
    {
        return MenuEntryBarBinding
        {
            .GetValue = [field]()
            {
                const auto& options = g_App.GetOptions().GetBack();
                return options.*field;
            },
            .SetValue = [field, max](int val)
            {
                auto& options  = g_App.GetOptions().GetFront();
                options.*field = std::clamp(val, 0, max);
            },
            .Max = max
        };
    }

    void OptionsMenu_UpdateConfig(const std::vector<MenuEntry>& entries);

    void OptionsMenu_UpdateSelection(int entryCount);

    void OptionsMenu_ResetSelection(int selectedEntryIdx = 0);
}
