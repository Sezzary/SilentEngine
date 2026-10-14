#include "Framework.h"
#include "Game/Screens/Options/Utils.h"

#include "Application.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Game.h"
#include "Game/Screens/Options/Options.h"
#include "Gui/Button.h"
#include "Gui/Cursor.h"
#include "Input/Input.h"
#include "Services/Options.h"

using namespace Silent::Gui;
using namespace Silent::Input;
using namespace Silent::Services;

namespace Silent::Game
{
    Cursor g_Options_Cursor = Cursor();

    MenuEntryBoolBinding MenuEntryBoolBinding::Bind(bool Options::* field)
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

    MenuEntryRangeBinding MenuEntryRangeBinding::Bind(int Options::* field, int min, int max, const std::string& prefix)
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
            .Min    = min,
            .Max    = max,
            .Prefix = prefix
        };
    }

    MenuEntryBarBinding MenuEntryBarBinding::Bind(int Options::* field, int max)
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

    MenuEntryLanguageBinding MenuEntryLanguageBinding::Bind(std::string Options::* field)
    {
        return MenuEntryLanguageBinding
        {
            .GetLocaleIdx = []()
            {
                const auto& translator = g_App.GetTranslator();

                const auto& locales = translator.GetLocales();
                for (int i = 0; i < locales.size(); i++)
                {
                    if (locales[i].Name == translator.GetActiveLocaleName())
                    {
                        return i;
                    }
                }

                return 0;
            },
            .SetLocale = [field](int localeIdx)
            {
                auto& options    = g_App.GetOptions().GetFront();
                auto& translator = g_App.GetTranslator();

                const auto& locales = translator.GetLocales();

                int wrappedLocaleIdx = WrapRange(localeIdx, locales.size() - 1);
                options.*field       = locales[wrappedLocaleIdx].Name;
                translator.SetActiveLocale(locales[wrappedLocaleIdx].Name);
            }
        };
    }

    void Options_UpdateConfig(const std::vector<MenuEntry>& entries)
    {
        const auto& input   = g_App.GetInput();
        auto&       options = g_App.GetOptions();

        const auto& entry = entries[g_OptionsMenu_SelectedEntry];

        // @todo Tries to execute even when not set??
        //if (entry.CanUpdate && !entry.CanUpdate())
        //{
        //    return;
        //}

        // Set config.
        bool isOptChanged = false;
        if (std::holds_alternative<MenuEntryBoolBinding>(entry.Binding))
        {
            if (input.GetAction(In::Left).IsClicked(ACTION_HALF_STATE) ||
                input.GetAction(In::Right).IsClicked(ACTION_HALF_STATE))
            {
                const auto& binding = std::get<MenuEntryBoolBinding>(entry.Binding);

                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetState(!binding.GetState());
                isOptChanged = true;
            }
        }
        else if (std::holds_alternative<MenuEntryRangeBinding>(entry.Binding))
        {
            const auto& binding = std::get<MenuEntryRangeBinding>(entry.Binding);

            if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) &&
                binding.GetValue() > binding.Min)
            {
                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetValue(binding.GetValue() - 1);
                isOptChanged = true;
            }
            else if (input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) &&
                     binding.GetValue() < binding.Max)
            {
                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetValue(binding.GetValue() + 1);
                isOptChanged = true;
            }
        }
        else if (std::holds_alternative<MenuEntryEnumBinding>(entry.Binding))
        {
            const auto& binding = std::get<MenuEntryEnumBinding>(entry.Binding);

            if (input.GetAction(In::Left).IsClicked(ACTION_HALF_STATE))
            {
                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetIdx(binding.GetIdx() - 1);
                isOptChanged = true;
            }
            else if (input.GetAction(In::Right).IsClicked(ACTION_HALF_STATE))
            {
                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetIdx(binding.GetIdx() + 1);
                isOptChanged = true;
            }
        }
        else if (std::holds_alternative<MenuEntryBarBinding>(entry.Binding))
        {
            const auto& binding = std::get<MenuEntryBarBinding>(entry.Binding);

            if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC * 0.5f, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) &&
                binding.GetValue() > 0)
            {
                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetValue(binding.GetValue() - (binding.Max / BAR_NOTCH_COUNT));
                isOptChanged = true;
            }
            else if (input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC * 0.5f, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) &&
                     binding.GetValue() < binding.Max)
            {
                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetValue(binding.GetValue() + (binding.Max / BAR_NOTCH_COUNT));
                isOptChanged = true;
            }
        }
        else if (std::holds_alternative<MenuEntryLanguageBinding>(entry.Binding))
        {
            const auto& binding = std::get<MenuEntryLanguageBinding>(entry.Binding);

            const auto& translator = g_App.GetTranslator();
            const auto& locales    = translator.GetLocales();

            if (input.GetAction(In::Left).IsClicked(ACTION_HALF_STATE))
            {
                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetLocale(binding.GetLocaleIdx() - 1);
                isOptChanged = true;
            }
            else if (input.GetAction(In::Right).IsClicked(ACTION_HALF_STATE))
            {
                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetLocale(binding.GetLocaleIdx() + 1);
                isOptChanged = true;
            }
        }

        if (isOptChanged)
        {
            if (entry.OnUpdate)
            {
                entry.OnUpdate();
            }

            options.Save();
        }
    }

    void Options_UpdateSelection(const std::vector<MenuEntry>& entries)
    {
        const auto& input = g_App.GetInput();

        g_Options_Cursor.Update();

        // Increment line move timer. @todo Selection highlight doesn't update after page switch until cursor is moved.
        if (g_OptionsMenu_SelectionHighlightTimer == NO_VALUE)
        {
            g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
        }
        else
        {
            g_OptionsMenu_SelectionHighlightTimer += g_DeltaTime;
            if (g_OptionsMenu_SelectionHighlightTimer >= LINE_CURSOR_TIMER_MAX)
            {
                g_OptionsMenu_SelectionHighlightTimer = LINE_CURSOR_TIMER_MAX;
            }
        }

        if (g_OptionsMenu_SelectionHighlightTimer != LINE_CURSOR_TIMER_MAX)
        {
            return;
        }

        g_OptionsMenu_PrevSelectedEntry          = g_OptionsMenu_SelectedEntry;
        g_OptionsMenu_PrevVisibleEntriesStartIdx = g_OptionsMenu_VisibleEntriesStartIdx;

        // Enter submenu.
        if (input.GetAction(In::Enter).IsClicked(ACTION_HALF_STATE))
        {
            const auto& entry = entries[g_OptionsMenu_SelectedEntry];

            if (std::holds_alternative<MenuEntrySubmenuBinding>(entry.Binding))
            {
                const auto& binding = std::get<MenuEntrySubmenuBinding>(entry.Binding);

                ScreenFade_Start(true, false, false);
                Game_StateStepSet(0, binding.MenuState);
                return;
            }
        }

        // Move selection cursor up/down.
        if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) &&
            !input.GetAction(In::Down).IsHeld(0.0f, GUI_PULSE_STATE_MIN))
        {
            //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

            g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
            g_OptionsMenu_SelectedEntry           = (g_OptionsMenu_SelectedEntry + (entries.size() - 1)) % entries.size();
        }
        else if (input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) &&
                 !input.GetAction(In::Up).IsHeld(0.0f, GUI_PULSE_STATE_MIN))
        {
            //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

            g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
            g_OptionsMenu_SelectedEntry           = (g_OptionsMenu_SelectedEntry + 1) % entries.size();
        }

        // Update visible entries region.
        if (g_OptionsMenu_SelectedEntry < (g_OptionsMenu_VisibleEntriesStartIdx + VISIBLE_ENTRY_SCROLL_BUFFER))
        {
            g_OptionsMenu_PrevVisibleEntriesStartIdx = g_OptionsMenu_VisibleEntriesStartIdx;
            g_OptionsMenu_VisibleEntriesStartIdx     = std::max(g_OptionsMenu_SelectedEntry - VISIBLE_ENTRY_SCROLL_BUFFER,
                                                                0);
        }
        else if (g_OptionsMenu_SelectedEntry > (((g_OptionsMenu_VisibleEntriesStartIdx + VISIBLE_ENTRY_COUNT_MAX) -
                                                 VISIBLE_ENTRY_SCROLL_BUFFER) - 1))
        {
            g_OptionsMenu_PrevVisibleEntriesStartIdx = g_OptionsMenu_VisibleEntriesStartIdx;
            g_OptionsMenu_VisibleEntriesStartIdx     = std::min(((g_OptionsMenu_SelectedEntry - VISIBLE_ENTRY_COUNT_MAX) +
                                                                 VISIBLE_ENTRY_SCROLL_BUFFER) + 1, 
                                                                std::max(0, (int)entries.size() - VISIBLE_ENTRY_COUNT_MAX));
        }
    }

    void Options_ResetSelection(int selectedEntryIdx)
    {
        g_OptionsMenu_SelectedEntry              = 
        g_OptionsMenu_PrevSelectedEntry          = selectedEntryIdx;
        g_OptionsMenu_VisibleEntriesStartIdx     =
        g_OptionsMenu_PrevVisibleEntriesStartIdx = 0; // @todo What if selected entry is outside default visible region?
        g_OptionsMenu_SelectionHighlightTimer    = NO_VALUE;
    }

    void Options_ControlSubmenu(const std::string& headingKey, const std::vector<MenuEntry>& entries, int menuStateStep,
                                int menuState, int leaveMenuState, int prevMenuEntryIdx)
    {
        const auto& input = g_App.GetInput();

        // Draw graphics.
        Options_DrawEntries(headingKey, entries);
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        if (g_GameWork.gameStateSteps[0] != menuState)
        {
            return;
        }

        Options_UpdateSelection(entries);
        Options_UpdateConfig(entries);

        // Handle menu state.
        switch (g_GameWork.gameStateSteps[menuStateStep])
        {
            case 0:
            {
                Options_ResetSelection();

                ScreenFade_Start(true, true, false);
                Game_StateStepIncrement(menuStateStep);
                break;
            }
            case 1:
            {
                if (input.GetAction(In::Cancel).IsClicked())
                {
                    //Sd_SfxPlay(Sfx_Cancel, 0, Q8(0.25f));

                    ScreenFade_Start(true, false, false);
                    Game_StateStepIncrement(menuStateStep);
                }
                break;
            }
            case 2:
            {
                // Switch to previous menu.
                if (ScreenFade_IsFinished())
                {
                    Options_ResetSelection(prevMenuEntryIdx);

                    ScreenFade_Start(true, true, false);
                    Game_StateStepSet(menuStateStep - 1, leaveMenuState);
                }
                break;
            }
        }
    }
}
