#include "Framework.h"
#include "Game/Screens/Options/Utils.h"

#include "Application.h"
#include "Game/Screens/Options/Options.h"
#include "Input/Input.h"

using namespace Silent::Input;

namespace Silent::Game
{
    void UpdateOptionsSelection(int entryCount)
    {
        constexpr int LINE_CURSOR_TIMER_MAX = 8;

        const auto& input = g_App.GetInput();

        // Increment line move timer.
        if ((LINE_CURSOR_TIMER_MAX - 1) < g_OptionsMenu_SelectionHighlightTimer)
        {
            g_OptionsMenu_SelectionHighlightTimer = LINE_CURSOR_TIMER_MAX;
        }
        else
        {
            g_OptionsMenu_SelectionHighlightTimer += 2;
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

            g_OptionsMenu_SelectionHighlightTimer = 0;
            g_OptionsMenu_SelectedEntry           = (g_OptionsMenu_SelectedEntry + (entryCount - 1)) % entryCount;
        }
        if (input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
        {
            //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

            g_OptionsMenu_SelectionHighlightTimer = 0;
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

    void ResetOptionsSelection(int selectedEntryIdx)
    {
        g_OptionsMenu_SelectedEntry              = 
        g_OptionsMenu_PrevSelectedEntry          = selectedEntryIdx;
        g_OptionsMenu_VisibleEntriesStartIdx     = g_OptionsMenu_PrevVisibleEntriesStartIdx;
        g_OptionsMenu_PrevVisibleEntriesStartIdx = 0;
        g_OptionsMenu_SelectionHighlightTimer    = 0;
    }
}
