#pragma once

#include "Application.h"
#include "Gui/Button.h"
#include "Gui/Cursor.h"
#include "Services/Options.h"

using namespace Silent::Gui;
using namespace Silent::Services;

namespace Silent::Game
{
    constexpr int    LINE_HEIGHT           = 16;
    constexpr int    BAR_NOTCH_COUNT       = 16;
    constexpr q19_12 LINE_CURSOR_TIMER_MAX = Q12(GUI_PULSE_DELAY_SEC);

    /** @brief Submenu entry options binding. */
    struct MenuEntrySubmenuBinding
    {
        int MenuState = 0;
    };

    /** @brief Boolean entry options binding. */
    struct MenuEntryBoolBinding
    {
        std::function<bool()>     GetState = nullptr;
        std::function<void(bool)> SetState = nullptr;

        static MenuEntryBoolBinding Bind(bool Options::* field);
    };

    /** @brief Value range entry options binding. */
    struct MenuEntryRangeBinding
    {
        std::function<int()>     GetValue = nullptr;
        std::function<void(int)> SetValue = nullptr;
        int                      Min      = 0;
        int                      Max      = 0;
        std::string              Prefix   = {};

        static MenuEntryRangeBinding Bind(int Options::* field, int min, int max, const std::string& prefix = {});
    };

    /** @brief Enum entry options binding. */
    struct MenuEntryEnumBinding
    {
        std::function<int()>     GetIdx           = nullptr;
        std::function<void(int)> SetIdx           = nullptr;
        std::vector<std::string> ConfigStringKeys = {};

        template <typename EnumT>
        static MenuEntryEnumBinding Bind(EnumT Options::* field, std::vector<std::string> configStrKeys);
    };

    /** @brief Notched bar entry options binding. */
    struct MenuEntryBarBinding
    {
        std::function<int()>     GetValue = nullptr;
        std::function<void(int)> SetValue = nullptr;
        int                      Max      = 0;

        static MenuEntryBarBinding Bind(int Options::* field, int max);
    };

    /** @brief Language locale entry options binding. */
    struct MenuEntryLanguageBinding
    {
        std::function<int()>     GetLocaleIdx = nullptr;
        std::function<void(int)> SetLocale    = nullptr;

        static MenuEntryLanguageBinding Bind(std::string Options::* field);
    };

    /** @brief Menu entry options binding. */
    using MenuEntryBinding = std::variant<MenuEntrySubmenuBinding,
                                          MenuEntryBoolBinding,
                                          MenuEntryRangeBinding,
                                          MenuEntryEnumBinding,
                                          MenuEntryBarBinding,
                                          MenuEntryLanguageBinding>;

    /** @brief Menu entry data. */
    struct MenuEntry
    {
        std::string      EntryStringKey = {};
        Button           Button         = {};
        MenuEntryBinding Binding        = {};

        std::function<void()> OnUpdate  = nullptr;
        std::function<bool()> CanUpdate = nullptr;
        std::function<bool()> IsEnabled = nullptr;
    };

    extern Cursor g_Options_Cursor;

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

    /** @brief Creates a new menu entry button.
     *
     * @todo
     * @param entry Button menu entry.
     * @param entryIdx Button entry index.
     */
    Button Options_GetEntryButton(int menuState, int entryIdx);

    /** @brief Updates a set of menu entry buttons according to what's visible onscreen.
     *
     * @param entries Output menu entries to update.
     */
    void Options_UpdateButtons(std::vector<MenuEntry>& entries);

    /** @brief Updates the options configuration according to bound menu entry behaviors.
     *
     * @param entries Menu entries.
     */
    void Options_UpdateConfig(const std::vector<MenuEntry>& entries);

    /** @brief Updates an options submenu selection within the context of a given set of entries.
     *
     * @param entries Menu entries.
     */
    void Options_UpdateSelection(std::vector<MenuEntry>& entries);

    /** @brief Resets the options submenu selection to a default entry.
     *
     * @param selectedEntryIdx Index of the default selected entry.
     */
    void Options_ResetSelection(int selectedEntryIdx = 0);

    /** @brief Controls an options submenu.
     *
     * @param headingKey String key of the submenu heading.
     * @param entries Output menu entries to update.
     * @param menuStateStep Menu state step.
     * @param menuState Menu state.
     * @param leaveMenuState Menu state to set when leaving the submenu.
     * @param prevMenuEntryIdx Index of the selected entry in the parent menu.
     */
    void Options_ControlSubmenu(const std::string& headingKey, std::vector<MenuEntry>& entries, int menuStateStep,
                                int menuState, int leaveMenuState, int prevMenuEntryIdx);
}
