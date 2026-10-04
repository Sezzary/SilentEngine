#include "Framework.h"
#include "Game/Screens/Options/Utils.h"

#include "Application.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Screens/Options/Options.h"
#include "Input/Input.h"

using namespace Silent::Input;

namespace Silent::Game
{
    void OptionsMenu_UpdateConfig(const std::vector<MenuEntry>& entries)
    {
        const auto& input   = g_App.GetInput();
        auto&       options = g_App.GetOptions();

        auto& entry = entries[g_OptionsMenu_SelectedEntry];

        // Check if `Left` or `Right` action is clicked.
        bool isLeftClicked  = input.GetAction(In::Left).IsClicked(ACTION_HALF_STATE);
        bool isRightClicked = input.GetAction(In::Right).IsClicked(ACTION_HALF_STATE);
        if (!isLeftClicked && !isRightClicked)
        {
            return;
        }

        // Set config.
        if (std::holds_alternative<MenuEntryBoolBinding>(entry.Binding))
        {
            const auto& binding = std::get<MenuEntryBoolBinding>(entry.Binding);

            binding.SetState(!binding.GetState());
        }
        else if (std::holds_alternative<MenuEntryRangeBinding>(entry.Binding))
        {
            const auto& binding = std::get<MenuEntryRangeBinding>(entry.Binding);

            binding.SetValue(binding.GetValue() + (isLeftClicked ? -1 : 1));
        }
        else if (std::holds_alternative<MenuEntryEnumBinding>(entry.Binding))
        {
            const auto& binding = std::get<MenuEntryEnumBinding>(entry.Binding);

            binding.SetIdx(binding.GetIdx() + (isLeftClicked ? -1 : 1));
        }
        else if (std::holds_alternative<MenuEntryBarBinding>(entry.Binding))
        {
            const auto& binding = std::get<MenuEntryBarBinding>(entry.Binding);

            if (isLeftClicked && binding.GetValue() > 0)
            {
                binding.SetValue(binding.GetValue() - 1);
            }
            else if (isRightClicked && binding.GetValue() < 16) // @todo Demagic.
            {
                binding.SetValue(binding.GetValue() + 1);
            }
        }

        // Execute post-update callback.
        if (entry.OnUpdate)
        {
            entry.OnUpdate();
        }

        options.Save();
    }

    void OptionsMenu_UpdateSelection(int entryCount)
    {
        const auto& input = g_App.GetInput();

        // Increment line move timer.
        g_OptionsMenu_SelectionHighlightTimer += g_DeltaTime;
        if (g_OptionsMenu_SelectionHighlightTimer >= LINE_CURSOR_TIMER_MAX)
        {
            g_OptionsMenu_SelectionHighlightTimer = LINE_CURSOR_TIMER_MAX;
        }

        if (g_OptionsMenu_SelectionHighlightTimer != LINE_CURSOR_TIMER_MAX)
        {
            return;
        }

        g_OptionsMenu_PrevSelectedEntry = g_OptionsMenu_SelectedEntry;

        // Move selection cursor up/down.
        if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
        {
            //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

            g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
            g_OptionsMenu_SelectedEntry           = (g_OptionsMenu_SelectedEntry + (entryCount - 1)) % entryCount;
        }
        if (input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
        {
            //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

            g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
            g_OptionsMenu_SelectedEntry           = (g_OptionsMenu_SelectedEntry + 1) % entryCount;
        }

        // Update visible entries region.
        if (g_OptionsMenu_SelectedEntry < (g_OptionsMenu_VisibleEntriesStartIdx + 1))
        {
            g_OptionsMenu_VisibleEntriesStartIdx = std::max(g_OptionsMenu_SelectedEntry - 1, 0);
        }
        else if (g_OptionsMenu_SelectedEntry > ((g_OptionsMenu_VisibleEntriesStartIdx + VISIBLE_ENTRY_COUNT_MAX) - 2))
        {
            int startIdxMax                      = std::max(0, entryCount - VISIBLE_ENTRY_COUNT_MAX);
            g_OptionsMenu_VisibleEntriesStartIdx = std::min((g_OptionsMenu_SelectedEntry - VISIBLE_ENTRY_COUNT_MAX) + 2, 
                                                            startIdxMax);
        }
    }

    void OptionsMenu_ResetSelection(int selectedEntryIdx)
    {
        g_OptionsMenu_SelectedEntry              = 
        g_OptionsMenu_PrevSelectedEntry          = selectedEntryIdx;
        g_OptionsMenu_VisibleEntriesStartIdx     = g_OptionsMenu_PrevVisibleEntriesStartIdx;
        g_OptionsMenu_PrevVisibleEntriesStartIdx = 0;
        g_OptionsMenu_SelectionHighlightTimer    = Q12(0.0f);
    }
}
