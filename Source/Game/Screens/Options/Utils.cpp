#include "Framework.h"
#include "Game/Screens/Options/Utils.h"

#include "Application.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Screens/Options/Options.h"
#include "Services/Options.h"
#include "Input/Input.h"

using namespace Silent::Input;
using namespace Silent::Services;

namespace Silent::Game
{
    void OptionsMenu_UpdateConfig(const std::vector<MenuEntry>& entries)
    {
        const auto& input   = g_App.GetInput();
        auto&       options = g_App.GetOptions();

        const auto& entry = entries[g_OptionsMenu_SelectedEntry];

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

            if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC * 0.25f, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) &&
                binding.GetValue() > 0)
            {
                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetValue(binding.GetValue() - (binding.Max / BAR_NOTCH_COUNT));
                isOptChanged = true;
            }
            else if (input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC * 0.25f, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) &&
                     binding.GetValue() < binding.Max)
            {
                //Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                binding.SetValue(binding.GetValue() + (binding.Max / BAR_NOTCH_COUNT));
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

    void Options_ControlSubmenu(int stateStep, const std::string& headingKey, const std::vector<MenuEntry>& entries,
                                int prevMenuEntryIdx, int state, int leaveState)
    {
        const auto& input = g_App.GetInput();

        // Draw graphics.
        OptionsMenu_DrawEntries(headingKey, entries);
        //Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        if (g_GameWork.gameStateSteps[0] != state)
        {
            return;
        }

        OptionsMenu_UpdateConfig(entries);
        OptionsMenu_UpdateSelection(entries.size());

        // Handle menu state.
        switch (g_GameWork.gameStateSteps[stateStep])
        {
            case 0:
            {
                OptionsMenu_ResetSelection();

                //ScreenFade_Start(true, true, false);
                Game_StateStepIncrement(stateStep);
                break;
            }
            case 1:
            {
                if (input.GetAction(In::Cancel).IsClicked())
                {
                    //Sd_SfxPlay(Sfx_Cancel, 0, Q8(0.25f));

                    //ScreenFade_Start(true, false, false);
                    Game_StateStepIncrement(stateStep);
                }
                break;
            }
            case 2:
            {
                // Switch to previous menu.
                /*if (ScreenFade_IsFinished())
                {
                    OptionsMenu_ResetSelection(prevMenuEntryIdx);

                    ScreenFade_Start(true, true, false);
                    Game_StateStepSet(stateStep - 1, leaveState);
                }*/
                break;
            }
        }
    }
}
