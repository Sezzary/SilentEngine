#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Options/Options.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Bodyprog/Sound/Sfx.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"
#include "Game/Screens/Options/Gameplay/Gameplay.h"
#include "Game/Screens/Options/Enhancements/Enhancements.h"
#include "Game/Screens/Options/Graphics/Brightness.h"
#include "Game/Screens/Options/Graphics/Graphics.h"
#include "Game/Screens/Options/Input/Bindings.h"
#include "Game/Screens/Options/Input/Input.h"
#include "Game/Screens/Options/MenuGraphics.h"
#include "Game/Screens/Options/System/System.h"
#include "Game/Screens/Options/Utils.h"
#include "Input/Input.h"

using namespace Silent::Assets;
using namespace Silent::Input;

namespace Silent::Game
{
    int    g_OptionsMenu_SelectedEntry              = 0;
    int    g_OptionsMenu_PrevSelectedEntry          = 0;
    int    g_OptionsMenu_VisibleEntriesStartIdx     = 0;
    int    g_OptionsMenu_PrevVisibleEntriesStartIdx = 0;
    q19_12 g_OptionsMenu_SelectionHighlightTimer    = Q12(0.0f);

    // @deprecated
    static int g_OptionsMenu_BulletMultMax = 0;

    static const auto ENTRIES = std::vector<MenuEntry>
    {
        MenuEntry
        {
            .EntryStringKey = KEY_OPTIONS_MENU_EXIT,
            .Binding        = MenuEntrySubmenuBinding{}
        },
        MenuEntry
        {
            .EntryStringKey = KEY_OPTIONS_MENU_GRAPHICS,
            .Binding        = MenuEntrySubmenuBinding{}
        },
        MenuEntry
        {
            .EntryStringKey = KEY_OPTIONS_MENU_GAMEPLAY,
            .Binding        = MenuEntrySubmenuBinding{}
        },
        MenuEntry
        {
            .EntryStringKey = KEY_OPTIONS_MENU_INPUT,
            .Binding        = MenuEntrySubmenuBinding{}
        },
        MenuEntry
        {
            .EntryStringKey = KEY_OPTIONS_MENU_ENHANCEMENTS,
            .Binding        = MenuEntrySubmenuBinding{}
        },
        MenuEntry
        {
            .EntryStringKey = KEY_OPTIONS_MENU_SYSTEM,
            .Binding        = MenuEntrySubmenuBinding{}
        },
    };

    static void Options_ControlOptionsMenu()
    {
        const auto& input = g_App.GetInput();

        // Submit graphics.
        Options_DrawEntries(KEY_OPTIONS_MENU_HEADING, ENTRIES);
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        // Block user input if transitioning to new menu.
        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_Options)
        {
            return;
        }

        // Leave to gameplay (if options menu was accessed with `Option` input action).
        if (g_GameWork.gameStatePrev == GameState_InGame &&
            !input.GetAction(In::Enter).IsClicked() && input.GetAction(In::Option).IsClicked())
        {
            Sd_SfxPlay(Sfx_MenuCancel, 0, 64);
            Game_StateStepSet(0, OptionsMenuState_Leave);
            return;
        }

        Options_UpdateSelection(MainOptionsMenuEntry_Count);

        switch (g_OptionsMenu_SelectedEntry)
        {
            case MainOptionsMenuEntry_Exit:
            {
                // Exit to gameplay.
                if (input.GetAction(In::Enter).IsClicked() || input.GetAction(In::Cancel).IsClicked())
                {
                    Sd_SfxPlay(Sfx_MenuCancel, 0, 64);

                    Game_StateStepSet(0, OptionsMenuState_Leave);
                }
                break;
            }
            case MainOptionsMenuEntry_Graphics:
            {
                // Enter submenu.
                if (input.GetAction(In::Enter).IsClicked())
                {
                    Sd_SfxPlay(Sfx_MenuConfirm, 0, 64);

                    ScreenFade_Start(true, false, false);
                    Game_StateStepSet(0, OptionsMenuState_EnterGraphics);
                }
                break;
            }
            case MainOptionsMenuEntry_Gameplay:
            {
                // Enter submenu.
                if (input.GetAction(In::Enter).IsClicked())
                {
                    Sd_SfxPlay(Sfx_MenuConfirm, 0, 64);

                    ScreenFade_Start(true, false, false);
                    Game_StateStepSet(0, OptionsMenuState_EnterGameplay);
                }
                break;
            }
            case MainOptionsMenuEntry_Input:
            {
                // Enter submenu.
                if (input.GetAction(In::Enter).IsClicked())
                {
                    Sd_SfxPlay(Sfx_MenuConfirm, 0, 64);

                    ScreenFade_Start(true, false, false);
                    Game_StateStepSet(0, OptionsMenuState_EnterInput);
                }
                break;
            }
            case MainOptionsMenuEntry_Enhancements:
            {
                // Enter submenu.
                if (input.GetAction(In::Enter).IsClicked())
                {
                    Sd_SfxPlay(Sfx_MenuConfirm, 0, 64);

                    ScreenFade_Start(true, false, false);
                    Game_StateStepSet(0, OptionsMenuState_EnterEnhancements);
                }
                break;
            }
            case MainOptionsMenuEntry_System:
            {
                // Enter submenu.
                if (input.GetAction(In::Enter).IsClicked())
                {
                    Sd_SfxPlay(Sfx_MenuConfirm, 0, 64);

                    ScreenFade_Start(true, false, false);
                    Game_StateStepSet(0, OptionsMenuState_EnterSystem);
                }
                break;
            }
        }

        // Reset selection cursor.
        if ((g_OptionsMenu_SelectedEntry != OptionsMenuEntry_Exit &&
            !input.GetAction(In::Enter).IsClicked()) &&
            input.GetAction(In::Cancel).IsClicked())
        {
            Sd_SfxPlay(Sfx_MenuCancel, 0, 64);

            g_OptionsMenu_SelectedEntry           = OptionsMenuEntry_Exit;
            g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
        }
    }

    void GameState_Options_Update()
    {
        if (g_GameWork.gameStatePrev == GameState_InGame)
        {
            //Bgm_MenuUpdate();
        }

        if (g_GameWork.gameStatePrev != GameState_MainMenu)
        {
            //Game_TimerUpdate();
        }

        // Handle options menu state.
        int unlockedOptFlags = 0;
        switch (g_GameWork.gameStateSteps[0])
        {
            case OptionsMenuState_Leave:
            {
                ScreenFade_Start(true, false, false);
                Game_StateStepSet(0, OptionsMenuState_LeaveOptions);
                break;
            }
            case OptionsMenuState_EnterOptions:
            {
                g_GameWork.background2dColor = { 0, 0, 0 };
                ScreenFade_Start(false, true, false);

                if (g_GameWork.gameStatePrev == GameState_InGame)
                {
                    //Game_RadioSoundStop();
                }

                g_OptionsMenu_SelectedEntry           = OptionsMenuEntry_Exit;
                g_OptionsMenu_PrevSelectedEntry       = 0;
                g_OptionsMenu_SelectedEntry           = 0;
                g_OptionsMenu_PrevSelectedEntry       = 0;
                g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
                unlockedOptFlags                      = g_GameWork.config.extraOptionsEnabled;

                // @todo Integrate this into new version.
                // Set available bullet multiplier.
                g_OptionsMenu_BulletMultMax = 1;
                for (int i = 0; i < 5; i++)
                {
                    if (unlockedOptFlags & (1 << i))
                    {
                        g_OptionsMenu_BulletMultMax++;
                    }
                }

                // @todo Visible extra options entries.
                //g_OptionsMenu_EntryCount = (g_GameWork.config.extraOptionsEnabled) ? OptionsMenuEntry_Count :
                //                                                                    (OptionsMenuEntry_Count - 2);

                Game_StateStepSet(0, OptionsMenuState_Options);
                break;
            }
            case OptionsMenuState_LeaveGraphics:
            case OptionsMenuState_LeaveGameplay:
            case OptionsMenuState_LeaveInput:
            case OptionsMenuState_LeaveEnhancements:
            case OptionsMenuState_LeaveSystem:
            {
                Game_StateStepSet(0, OptionsMenuState_Options);
                break;
            }
            case OptionsMenuState_LeaveOptions:
            {
                if (ScreenFade_IsFinished())
                {
                    Game_StateSetPrevious();
                }
                break;
            }
            case OptionsMenuState_EnterGraphics:
            {
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_Graphics);
                }
                break;
            }
            case OptionsMenuState_EnterGameplay:
            {
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_Gameplay);
                }
                break;
            }
            case OptionsMenuState_EnterInput:
            {
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_Input);
                }
                break;
            }
            case OptionsMenuState_EnterEnhancements:
            {
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_Enhancements);
                }
                break;
            }
            case OptionsMenuState_EnterSystem:
            {
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_System);
                }
                break;
            }
            case OptionsMenuState_Graphics:
            {
                Options_ControlGraphicsMenu();
                break;
            }
            case OptionsMenuState_Gameplay:
            {
                Options_ControlGameplayMenu();
                break;
            }
            case OptionsMenuState_Input:
            {
                Options_ControlInputMenu();
                break;
            }
            case OptionsMenuState_Enhancements:
            {
                Options_ControlEnhancementsMenu();
                break;
            }
            case OptionsMenuState_System:
            {
                Options_ControlSystemMenu();
                break;
            }

            case OptionsMenuState_EnterBrightness:
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_Brightness);
                }
                break;

            case OptionsMenuState_Brightness:
                Options_ControlBrightnessMenu();
                break;

            case OptionsMenuState_LeaveBrightness:
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_Graphics);
                }
                break;

            case OptionsMenuState_EnterBindings:
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_Bindings);
                }
                break;

            case OptionsMenuState_Bindings:
                Options_ControlBindingsConfigMenu();
                break;
        }

        switch (g_GameWork.gameStateSteps[0])
        {
            case OptionsMenuState_Leave:
            case OptionsMenuState_Options:
            case OptionsMenuState_LeaveOptions:
            case OptionsMenuState_EnterGraphics:
            case OptionsMenuState_EnterGameplay:
            case OptionsMenuState_EnterInput:
            case OptionsMenuState_EnterEnhancements:
            case OptionsMenuState_EnterSystem:
            {
                Options_ControlOptionsMenu();
                break;
            }
        }
    }
}
