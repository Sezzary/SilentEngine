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
    static int g_OptionsMenu_SelectedBloodColorEntry = 0;
    static int g_OptionsMenu_BulletMultMax           = 0;

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

    static void ControlOptionsMenu()
    {
        const auto& input = g_App.GetInput();

        // Submit graphics.
        auto widths = OptionsMenu_DrawEntries(KEY_OPTIONS_MENU_HEADING, ENTRIES);
        //OptionsMenu_ConfigDraw();
        OptionsMenu_DrawSelectionHighlight(widths);
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

        OptionsMenu_UpdateSelection(MainOptionsMenuEntry_Count);

        switch (g_OptionsMenu_SelectedEntry)
        {
            case MainOptionsMenuEntry_Exit:
            {
                // Exit to gameplay.
                if (input.GetAction(In::Enter).IsClicked() || input.GetAction(In::Cancel).IsClicked())
                {
                    Sd_SfxPlay(Sfx_MenuCancel, 0, 64);
                    g_OptionsMenu_PrevVisibleEntriesStartIdx = g_OptionsMenu_VisibleEntriesStartIdx;

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
                    g_OptionsMenu_PrevVisibleEntriesStartIdx = g_OptionsMenu_VisibleEntriesStartIdx;

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
                    g_OptionsMenu_PrevVisibleEntriesStartIdx = g_OptionsMenu_VisibleEntriesStartIdx;

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
                    g_OptionsMenu_PrevVisibleEntriesStartIdx = g_OptionsMenu_VisibleEntriesStartIdx;

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
                    g_OptionsMenu_PrevVisibleEntriesStartIdx = g_OptionsMenu_VisibleEntriesStartIdx;

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
                    g_OptionsMenu_PrevVisibleEntriesStartIdx = g_OptionsMenu_VisibleEntriesStartIdx;

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
                g_OptionsMenu_BulletMultMax           = 1;
                unlockedOptFlags                      = g_GameWork.config.extraOptionsEnabled;
                
                // Set available bullet multiplier.
                for (int i = 0; i < 5; i++)
                {
                    if (unlockedOptFlags & (1 << i))
                    {
                        g_OptionsMenu_BulletMultMax++;
                    }
                }

                // Set selected blood color.
                switch (g_GameWork.config.extraBloodColor)
                {
                    case BloodColor_Normal:
                        g_OptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Normal;
                        break;

                    case BloodColor_Green:
                        g_OptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Green;
                        break;

                    case BloodColor_Violet:
                        g_OptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Violet;
                        break;

                    case BloodColor_Black:
                        g_OptionsMenu_SelectedBloodColorEntry = BloodColorMenuEntry_Black;
                        break;
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
                OptionsMenu_ControlGraphicsMenu();
                break;
            }
            case OptionsMenuState_Gameplay:
            {
                Options_GameplayMenu_Control();
                break;
            }
            case OptionsMenuState_Input:
            {
                OptionsMenu_ControlInputMenu();
                break;
            }
            case OptionsMenuState_Enhancements:
            {
                Options_EnhancementsMenu_Control();
                break;
            }
            case OptionsMenuState_System:
            {
                Options_SystemMenu_Control();
                break;
            }

            case OptionsMenuState_EnterBrightness:
                if (ScreenFade_IsFinished())
                {
                    Game_StateStepSet(0, OptionsMenuState_Brightness);
                }
                break;

            case OptionsMenuState_Brightness:
                ControlBrightnessOptionsMenu();
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
                Options_BindingsMenu_Control();
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
                ControlOptionsMenu();
                break;
            }
        }
    }

    // @deprecated
    void OptionsMenu_Control()
    {
        constexpr int LINE_CURSOR_TIMER_MAX = 8;
        constexpr int SOUND_VOL_STEP        = 8;

        /*static const auto ENTRIES = std::vector<MenuEntry>
        {
            MenuEntry
            {
                .Type           = MenuEntryType::Submenu,
                .EntryStringKey = KEY_OPTIONS_MENU_EXIT
            },
            MenuEntry
            {
                .Type           = MenuEntryType::Submenu,
                .EntryStringKey = KEY_OPTIONS_MENU_BRIGHT_LEVEL
            },
            MenuEntry
            {
                .Type           = MenuEntryType::Submenu,
                .EntryStringKey = KEY_OPTIONS_MENU_CONT_CONFIG
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_VIBRATION,
                .ConfigStringKeys = 
                {
                    KEY_OPTIONS_MENU_ON,
                    KEY_OPTIONS_MENU_OFF
                }
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_AUTO_LOAD,
                .ConfigStringKeys =
                {
                    KEY_OPTIONS_MENU_ON,
                    KEY_OPTIONS_MENU_OFF
                }
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_SOUND,
                .ConfigStringKeys =
                {
                    KEY_OPTIONS_MENU_STEREO,
                    KEY_OPTIONS_MENU_MONAURAL
                }
            },
            MenuEntry
            {
                .Type           = MenuEntryType::Bar,
                .EntryStringKey = KEY_OPTIONS_MENU_BGM_VOL
            },
            MenuEntry
            {
                .Type           = MenuEntryType::Bar,
                .EntryStringKey = KEY_OPTIONS_MENU_SE_VOL
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_LANGUAGE,
                .ConfigStringKeys = {} // @todo Determined dynamically.
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_WEAPON_CONTROL,
                .ConfigStringKeys =
                {
                    KEY_OPTIONS_MENU_PRESS,
                    KEY_OPTIONS_MENU_SWITCH
                }
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_BLOOD_COLOR,
                .ConfigStringKeys = 
                {
                    KEY_OPTIONS_MENU_NORMAL,
                    KEY_OPTIONS_MENU_GREEN,
                    KEY_OPTIONS_MENU_VIOLET,
                    KEY_OPTIONS_MENU_BLACK
                }
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_VIEW_CONTROL,
                .ConfigStringKeys = 
                {
                    KEY_OPTIONS_MENU_NORMAL,
                    KEY_OPTIONS_MENU_REVERSE
                }
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_RETREAT_TURN,
                .ConfigStringKeys = 
                {
                    KEY_OPTIONS_MENU_NORMAL,
                    KEY_OPTIONS_MENU_REVERSE
                }
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_WALK_RUN_CONTROL,
                .ConfigStringKeys = 
                {
                    KEY_OPTIONS_MENU_NORMAL,
                    KEY_OPTIONS_MENU_REVERSE
                }
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_AUTO_AIMING,
                .ConfigStringKeys = 
                {
                    KEY_OPTIONS_MENU_ON,
                    KEY_OPTIONS_MENU_OFF
                }
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_VIEW_MODE,
                .ConfigStringKeys = 
                {
                    KEY_OPTIONS_MENU_NORMAL,
                    KEY_OPTIONS_MENU_SELF_VIEW
                }
            },
            MenuEntry
            {
                .Type             = MenuEntryType::List,
                .EntryStringKey   = KEY_OPTIONS_MENU_BULLET_ADJUST,
                .ConfigStringKeys =
                {
                    "x1",
                    "x2",
                    "x3",
                    "x4",
                    "x5",
                    "x6"
                }
            }
        };*/

        const auto& input = g_App.GetInput();

        // Draw graphics.
        //auto widths = OptionsMenu_DrawEntries(KEY_OPTIONS_MENU_HEADING, ENTRIES);
        //OptionsMenu_ConfigDraw();
        //OptionsMenu_DrawSelectionHighlight(widths);
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);
        //OptionsMenu_BgmVolumeBarDraw();
        //OptionsMenu_SfxVolumeBarDraw();

        // Block user input if transitioning to new menu.
        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_Options)
        {
            return;
        }

        // Increment line move timer.
        g_OptionsMenu_SelectionHighlightTimer = CLAMP(g_OptionsMenu_SelectionHighlightTimer + g_DeltaTime,
                                                      Q12(0.0f), Q12(1.0f));

        if (g_OptionsMenu_SelectionHighlightTimer != LINE_CURSOR_TIMER_MAX)
        {
            return;
        }

        g_OptionsMenu_PrevSelectedEntry = g_OptionsMenu_SelectedEntry;

        // Leave to gameplay (if options menu was accessed with `Option` input action).
        if (g_GameWork.gameStatePrev == GameState_InGame &&
            !input.GetAction(In::Enter).IsClicked() && input.GetAction(In::Option).IsClicked())
        {
            Sd_SfxPlay(Sfx_MenuCancel, 0, 64);
            Game_StateStepSet(0, OptionsMenuState_Leave);
            return;
        }

        // Move selection cursor up/down.
        if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
        {
            Sd_SfxPlay(Sfx_MenuMove, 0, 64);

            g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
            g_OptionsMenu_SelectedEntry           = (g_OptionsMenu_SelectedEntry + (OptionsMenuEntry_Count - 1)) % OptionsMenuEntry_Count;
        }
        if (input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
        {
            Sd_SfxPlay(Sfx_MenuMove, 0, 64);

            g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
            g_OptionsMenu_SelectedEntry           = (g_OptionsMenu_SelectedEntry + 1) % OptionsMenuEntry_Count;
        }

        // Update visible entry region.
        if (g_OptionsMenu_SelectedEntry < (g_OptionsMenu_VisibleEntriesStartIdx + 1))
        {
            g_OptionsMenu_VisibleEntriesStartIdx = std::max(g_OptionsMenu_SelectedEntry - 1, 0);
        }
        else if (g_OptionsMenu_SelectedEntry > ((g_OptionsMenu_VisibleEntriesStartIdx + VISIBLE_ENTRY_COUNT_MAX) - 2))
        {
            int startIdxMax                      = std::max(0, OptionsMenuEntry_Count - VISIBLE_ENTRY_COUNT_MAX);
            g_OptionsMenu_VisibleEntriesStartIdx = std::min((g_OptionsMenu_SelectedEntry - VISIBLE_ENTRY_COUNT_MAX) + 2, 
                                                            startIdxMax);
        }

        // Handle config change.
        int vol = 0;
        switch (g_OptionsMenu_SelectedEntry)
        {
            case OptionsMenuEntry_Exit:
                // Exit menu to gameplay.
                if (input.GetAction(In::Enter).IsClicked() || input.GetAction(In::Cancel).IsClicked())
                {
                    Sd_SfxPlay(Sfx_MenuCancel, 0, 64);
                    Game_StateStepSet(0, OptionsMenuState_Leave);
                }
                break;

            case OptionsMenuEntry_Controller:
                // Enter controller screen.
                if (input.GetAction(In::Enter).IsClicked())
                {
                    Sd_SfxPlay(Sfx_MenuConfirm, 0, 64);
                    Fs_QueueStartReadTim(FILE_TIM_OPTION2_TIM, IMAGE_BUFFER_3, &g_ControllerButtonAtlasImg);
                    ScreenFade_Start(true, false, false);
                    Game_StateStepSet(0, OptionsMenuState_EnterBindings);
                }
                break;

            case OptionsMenuEntry_Brightness:
                if (input.GetAction(In::Enter).IsClicked())
                {
                    Sd_SfxPlay(Sfx_MenuConfirm, 0, 64);
                    ScreenFade_Start(true, false, false);
                    Game_StateStepSet(0, OptionsMenuState_EnterBrightness);
                }
                break;

            case OptionsMenuEntry_Vibration:
                if (input.GetAction(In::Left).IsClicked(ACTION_HALF_STATE) ||
                    input.GetAction(In::Right).IsClicked(ACTION_HALF_STATE))
                {
                    Sd_SfxPlay(Sfx_MenuMove, 0, 64);
                    g_GameWork.config.vibrationEnabled = !g_GameWork.config.vibrationEnabled << 7;
                }
                break;

            case OptionsMenuEntry_AutoLoad:
                if (input.GetAction(In::Left).IsClicked(ACTION_HALF_STATE) ||
                    input.GetAction(In::Right).IsClicked(ACTION_HALF_STATE))
                {
                    Sd_SfxPlay(Sfx_MenuMove, 0, 64);
                    g_GameWork.config.autoLoad = (s8)g_GameWork.config.autoLoad == 0;
                }
                break;

            case OptionsMenuEntry_Sound:
                if (input.GetAction(In::Left).IsClicked(ACTION_HALF_STATE) ||
                    input.GetAction(In::Right).IsClicked(ACTION_HALF_STATE))
                {
                    Sd_SfxPlay(Sfx_MenuMove, 0, 64);

                    // Set config.
                    /*int audioType                           = AudioMode_Stereo;
                    g_GameWork.config.soundType = !g_GameWork.config.soundType;
                    if (g_GameWork.config.soundType)
                    {
                        audioType = AudioMode_Mono;
                    }
                    Sd_EngineCmd(audioType);*/
                }
                break;

            case OptionsMenuEntry_BgmVolume:
                vol = g_GameWork.config.volumeBgm;

                /*if ((vol < OPT_SOUND_VOLUME_MAX && input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN)) ||
                    (vol > 0                    && input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN)))
                {
                    Sd_EngineCmd(Sfx_MenuMove);
                }
                if ((vol == OPT_SOUND_VOLUME_MAX && input.GetAction(In::Right).IsClicked()) ||
                    (vol == 0                    && input.GetAction(In::Left).IsClicked()))
                {
                    Sd_EngineCmd(Sfx_MenuError);
                }*/

                // Scroll left/right.
                if (input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    vol += SOUND_VOL_STEP;
                }
                if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    vol -= SOUND_VOL_STEP;
                }

                // Set config.
                //vol = CLAMP(vol, 0, OPT_SOUND_VOLUME_MAX);
                //Sd_SetVolume(OPT_SOUND_VOLUME_MAX, vol, g_GameWork.config.volumeSe);
                g_GameWork.config.volumeBgm = vol;
                break;

            case OptionsMenuEntry_SfxVolume:
                vol = g_GameWork.config.volumeSe;

                /*if ((vol < OPT_SOUND_VOLUME_MAX && input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN)) ||
                    (vol > 0                    && input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN)))
                {
                    Sd_EngineCmd(Sfx_MenuMove);
                }
                if ((vol == OPT_SOUND_VOLUME_MAX && input.GetAction(In::Right).IsClicked()) ||
                    (vol == 0                    && input.GetAction(In::Left).IsClicked()))
                {
                    Sd_EngineCmd(Sfx_MenuError);
                }*/

                if (input.GetAction(In::Left).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    vol = vol - SOUND_VOL_STEP;
                }
                if (input.GetAction(In::Right).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    vol = vol + SOUND_VOL_STEP;
                }

                /*vol = CLAMP(vol, 0, OPT_SOUND_VOLUME_MAX);

                Sd_SetVolume(OPT_SOUND_VOLUME_MAX, vol, g_GameWork.config.volumeSe);*/
                g_GameWork.config.volumeSe = vol;
                break;

            case OptionsMenuEntry_Language:
            {
                // @todo Implement lanugage selection.
                break;
            }
        }

        // Reset selection cursor.
        if ((g_OptionsMenu_SelectedEntry != OptionsMenuEntry_Exit &&
            !input.GetAction(In::Enter).IsClicked()) &&
            input.GetAction(In::Cancel).IsClicked())
        {
            Sd_SfxPlay(Sfx_MenuCancel, 0, 64);

            g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
            g_OptionsMenu_SelectedEntry           = OptionsMenuEntry_Exit;
        }
    }

    void Options_ExtraOptionsMenu_Control()
    {
/*
        //Options_OptionsMenu_DrawEntries();
        //Options_OptionsMenu_ConfigDraw();
        //Options_OptionsMenu_DrawSelectionHighlight();
        Screen_BackgroundImgDraw(&g_ItemInspectionImg);

        // Block user input if transitioning to new menu.
        if (g_GameWork.gameStateSteps[0] != OptionsMenuState_Options)
        {
            return;
        }

        // Increment line move timer.
        g_OptionsMenu_SelectionHighlightTimer = CLAMP(g_OptionsMenu_SelectionHighlightTimer + g_DeltaTime,
                                                      Q12(0.0f), Q12(1.0f));

        if (g_OptionsMenu_SelectionHighlightTimer == LINE_CURSOR_TIMER_MAX)
        {
            g_OptionsMenu_PrevSelectedEntry = g_OptionsMenu_SelectedEntry;

            // Leave to gameplay (if options menu was accessed with `Option` input action).
            if (g_GameWork.gameStatePrev == GameState_InGame && 
                !(g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.enter) &&
                (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.option))
            {
                Sd_SfxPlay(Sfx_Cancel, 0, 64);
                Game_StateStepSet(0, OptionsMenuState_Leave);
                return;
            }

            // Move selection cursor up/down.
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighUp)
            {
                s32 var = 1;
                Sd_SfxPlay(Sfx_Back, 0, 64);
                g_OptionsMenu_SelectedEntry  = ((g_OptionsMenu_SelectedEntry - var) + g_OptionsMenu_EntryCount) % g_OptionsMenu_EntryCount;
                g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
            }
            if (g_Controller0->buttonFlags.pulsed & ControllerFlag_LStickHighDown)
            {
                Sd_SfxPlay(Sfx_Back, 0, 64);
                g_OptionsMenu_SelectedEntry++;
                g_OptionsMenu_SelectedEntry  = g_OptionsMenu_SelectedEntry % g_OptionsMenu_EntryCount;
                g_OptionsMenu_SelectionHighlightTimer = Q12(0.0f);
            }

            // Handle config change.
            switch (g_OptionsMenu_SelectedEntry)
            {
                case OptionsMenuEntry_WeaponCtrl:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);
                        g_GameWork.config.extraWeaponCtrl = !g_GameWork.config.extraWeaponCtrl;
                    }
                    break;

                case OptionsMenuEntry_Blood:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighRight)
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);
                        g_OptionsMenu_SelectedBloodColorEntry++;
                    }
                    if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighLeft)
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);
                        g_OptionsMenu_SelectedBloodColorEntry += 3;
                    }

                    // Set config.
                    g_OptionsMenu_SelectedBloodColorEntry = g_OptionsMenu_SelectedBloodColorEntry % BloodColorMenuEntry_Count;
                    switch (g_OptionsMenu_SelectedBloodColorEntry)
                    {
                        case BloodColorMenuEntry_Normal:
                            g_GameWork.config.extraBloodColor = BloodColor_Normal;
                            break;

                        case BloodColorMenuEntry_Green:
                            g_GameWork.config.extraBloodColor = BloodColor_Green;
                            break;

                        case BloodColorMenuEntry_Violet:
                            g_GameWork.config.extraBloodColor = BloodColor_Violet;
                            break;

                        case BloodColorMenuEntry_Black:
                            g_GameWork.config.extraBloodColor = BloodColor_Black;
                            break;
                    }
                    break;

                case OptionsMenuEntry_ViewCtrl:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraViewCtrl = !g_GameWork.config.extraViewCtrl;
                    }
                    break;

                case OptionsMenuEntry_RetreatTurn:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraRetreatTurn = (s8)g_GameWork.config.extraRetreatTurn == 0;
                    }
                    break;

                case OptionsMenuEntry_Control:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraWalkRunCtrl = (s8)g_GameWork.config.extraWalkRunCtrl == 0;
                    }
                    break;

                case OptionsMenuEntry_AutoAiming:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraAutoAiming = (s8)g_GameWork.config.extraAutoAiming == 0;
                    }
                    break;

                case OptionsMenuEntry_ViewMode:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraViewMode = !g_GameWork.config.extraViewMode;
                    }
                    break;

                case OptionsMenuEntry_BulletAdjust:
                    // Scroll left/right.
                    if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighRight)
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraBulletAdjust++;
                    }
                    if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighLeft)
                    {
                        Sd_SfxPlay(Sfx_Back, 0, 64);

                        // Set config.
                        g_GameWork.config.extraBulletAdjust = g_GameWork.config.extraBulletAdjust + (g_OptionsMenu_BulletMultMax - 1);
                    }
                    g_GameWork.config.extraBulletAdjust = g_GameWork.config.extraBulletAdjust % g_OptionsMenu_BulletMultMax;
                    break;
            }
        }

        // Leave menu.
        if ((g_Controller0->buttonFlags.clicked & (g_GameWorkPtr->config.controllerConfig.cance |
                                            (ControllerFlag_L2 | ControllerFlag_R2 | ControllerFlag_L1 | ControllerFlag_R1))) &&
            g_GameWork.gameStateSteps[0] != OptionsMenuState_LeaveExtraOptions)
        {
            if (g_Controller0->buttonFlags.clicked & g_GameWorkPtr->config.controllerConfig.cancel)
            {
                Sd_SfxPlay(Sfx_Cancel, 0, 64);
            }
            else
            {
                Sd_SfxPlay(Sfx_Confirm, 0, 64);
            }

            ScreenFade_Start(true, false, false);
            Game_StateStepSet(0, OptionsMenuState_LeaveExtraOptions);
        }
*/
    }
}
