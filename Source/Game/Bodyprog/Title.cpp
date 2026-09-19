#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Title.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Events/MapMsg.h"
#include "Game/Bodyprog/GameBoot/GameBoot.h"
#include "Game/Bodyprog/MemCard.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Screen/ScreenFade.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Bodyprog/Sys/FsScreens.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"
#include "Game/Main/Rng.h"
#include "Game/Screens/Stream/Stream.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Utils/Translator.h"

using namespace Silent::Assets;
using namespace Silent::Input;
using namespace Silent::Renderer;
using namespace Silent::Utils;

namespace Silent::Game
{
    constexpr int MAIN_MENU_FOG_COUNT = 21;

    static int g_MainMenuState              = MainMenuState_Start;
    static int g_MainMenu_SelectedEntry     = MainMenuEntry_Start;
    static int g_MainMenu_VisibleEntryFlags = (1 << MainMenuEntry_Start)  |
                                              (1 << MainMenuEntry_Option) |
                                              (1 << MainMenuEntry_Quit);

    s8 g_Demo_ReproducedCount = 0;
    s8* D_800BCDE0;

    static void MainMenu_MainTextDraw()
    {
        constexpr int  COLUMN_POS_X                = SCREEN_WIDTH / 2;
        constexpr int  COLUMN_POS_Y                = (SCREEN_HEIGHT / 6) * 5;
        constexpr int  STR_OFFSET_Y                = 10;
        constexpr auto MAIN_MENU_ENTRY_STRING_KEYS = std::array<const char*, MainMenuEntry_Count>
        {
            KEY_MAIN_MENU_LOAD,
            KEY_MAIN_MENU_CONTINUE,
            KEY_MAIN_MENU_START,
            KEY_MAIN_MENU_OPTION,
            KEY_MAIN_MENU_QUIT
        };

        const auto& translator = g_App.GetTranslator();

        // Draw selection strings.
        int columnPosTop = COLUMN_POS_Y - ((MainMenuEntry_Count - 1) * STR_OFFSET_Y);
        int offsetY      = 0;
        for (int i = 0; i < MainMenuEntry_Count; i++)
        {
            // Check if entry is visible.
            if (!(g_MainMenu_VisibleEntryFlags & (1 << i)))
            {
                continue;
            }

            Gfx_StringPositionSet(COLUMN_POS_X, columnPosTop + offsetY);
            Gfx_StringColorSet(StringColorId_White);

            if (i == g_MainMenu_SelectedEntry)
            {
                auto selectedEntryMsg = "{M}"                                      +
                                        translator(KEY_MAIN_MENU_OPENING_QUOTE)    +
                                        translator(MAIN_MENU_ENTRY_STRING_KEYS[i]) +
                                        translator(KEY_MAIN_MENU_CLOSING_QUOTE);
                Gfx_StringDraw(selectedEntryMsg, INT_MAX, true);
            }
            else
            {
                auto unselectedEntryMsg = "{M}" + translator(MAIN_MENU_ENTRY_STRING_KEYS[i]);
                Gfx_StringDraw(unselectedEntryMsg, INT_MAX, true);
            }

            offsetY += STR_OFFSET_Y;
        }
    }

    static void MainMenu_DifficultyTextDraw(int selectedEntryIdx)
    {
        constexpr int  COLUMN_POS_X                      = SCREEN_WIDTH / 2;
        constexpr int  COLUMN_POS_Y                      = (SCREEN_HEIGHT / 6) * 5;
        constexpr int  STR_OFFSET_Y                      = 10;
        constexpr int  DIFFICULTY_MENU_SELECTION_COUNT   = 3;
        constexpr auto DIFFICULTY_MENU_ENTRY_STRING_KEYS = std::array<const char*, DIFFICULTY_MENU_SELECTION_COUNT>
        {
            KEY_MAIN_MENU_EASY,
            KEY_MAIN_MENU_NORMAL,
            KEY_MAIN_MENU_HARD
        };

        const auto& translator = g_App.GetTranslator();

        // Draw selection strings.
        int columnPosTop = COLUMN_POS_Y - ((DIFFICULTY_MENU_SELECTION_COUNT - 1) * STR_OFFSET_Y);
        for (int i = 0; i < DIFFICULTY_MENU_SELECTION_COUNT; i++)
        {
            Gfx_StringPositionSet(COLUMN_POS_X, columnPosTop + (i * STR_OFFSET_Y));
            Gfx_StringColorSet(StringColorId_White);

            if (i == selectedEntryIdx)
            {
                auto selectedEntryStr = "{M}"                                            +
                                        translator(KEY_MAIN_MENU_OPENING_QUOTE)          +
                                        translator(DIFFICULTY_MENU_ENTRY_STRING_KEYS[i]) +
                                        translator(KEY_MAIN_MENU_CLOSING_QUOTE);
                Gfx_StringDraw(selectedEntryStr, INT_MAX, true);
            }
            else
            {
                auto unselectedEntryStr = "{M}" + translator(DIFFICULTY_MENU_ENTRY_STRING_KEYS[i]);
                Gfx_StringDraw(unselectedEntryStr, INT_MAX, true);
            }
        }
    }

    void MainMenu_BackgroundDraw()
    {
        auto& renderer = g_App.GetRenderer();

        if (g_SysWork.sysState == SysState_Gameplay)
        {
            SysWork_StateSetNext(SysState_OptionsMenu);
            func_8003BCF4();
        }

        Screen_BackgroundImgDraw("Psx/TIM/TITLE_E.TIM", true);
        MainMenu_FogUpdate();
    }

    void GameState_MainMenu_Update() // 0x8003AB28
    {
        constexpr int MAIN_MENU_GAME_STATE_COUNT = 5;

        static int newGameSelectedDifficultyIdx = 1;
        static int prevSavegameCount            = 0;

        const auto& input = g_App.GetInput();

        auto NEXT_GAME_STATES = std::array<int, MAIN_MENU_GAME_STATE_COUNT>
        {
            GameState_LoadSavegameScreen,
            GameState_AutoLoadSavegame,
            GameState_MovieOpening,
            GameState_OptionScreen,
            GameState_MovieIntro
        };

        //func_80033548();

        // After staying idle in the title screen for some time, this checks if the intro FMV or a
        // demo gameplay segment should be played. If the next value from `g_Demo_ReproducedCount`
        // is a value divisible by 3, the intro FMV will play. Otherwise, it defaults to a gameplay demo.
        bool playInGameDemo = ((g_Demo_ReproducedCount + 1) % 3) != 0;

        if (g_GameWork.gameStateSteps[0] == 0)
        {
            g_MainMenuState = MainMenuState_Start;

            // @todo Broken.
            //if (playInGameDemo)
            //{
            //    g_SysWork.processFlags = ProcessFlag_BootDemo;
            //}
            //else
            {
                g_GameWork.gameStateSteps[0] = 1;
            }
        }

        switch (g_MainMenuState)
        {
            case MainMenuState_Start:
                g_GameWork.background2dColor.r = 0;
                g_GameWork.background2dColor.g = 0;
                g_GameWork.background2dColor.b = 0;

                g_IntervalVBlanks = 1;
                ScreenFade_Start(true, true, false);
                g_ScreenFadeTimestep = Q12(2.0f);

                g_MainMenuState++;
                break;

            case MainMenuState_Main:
                if (playInGameDemo)
                {
                    //GameFs_MapStartup();

                    if (g_GameWork.gameStateSteps[0] == 1 && g_SysWork.gameStateStepCounter == 0)
                    {
                        g_Demo_ReproducedCount++;
                    }

                    if (g_GameWork.gameState == GameState_MainLoadScreen)
                    {
                        g_Demo_ReproducedCount++;
                    }
                }

                g_MainMenu_VisibleEntryFlags = (1 << MainMenuEntry_Start)  |
                                               (1 << MainMenuEntry_Option) |
                                               (1 << MainMenuEntry_Quit);

                if (g_GameWork.autosave.playerHealth > Q12(0.0f))
                {
                    g_MainMenu_VisibleEntryFlags = (1 << MainMenuEntry_Continue) |
                                                   (1 << MainMenuEntry_Start)    |
                                                   (1 << MainMenuEntry_Option)   |
                                                   (1 << MainMenuEntry_Quit);
                }

                // Memory card present and savegames exist.
                if (true)//(g_MemCard_SavegameCount > 0)
                {
                    g_MainMenu_VisibleEntryFlags |= (1 << MainMenuEntry_Load) |
                                                    (1 << MainMenuEntry_Continue);

                    /*if (prevSavegameCount < g_MemCard_SavegameCount && g_MainMenu_SelectedEntry != MainMenuEntry_Load)
                    {
                        g_MainMenu_SelectedEntry = MainMenuEntry_Continue;
                    }*/
                }
                // No savegames exist, but did previously (e.g. memory card removed before player death).
                else if (prevSavegameCount > 0)
                {
                    while (!(g_MainMenu_VisibleEntryFlags & (1 << g_MainMenu_SelectedEntry)))
                    {
                        g_MainMenu_SelectedEntry++;
                    }
                }

                g_MainMenu_VisibleEntryFlags |= g_MainMenu_VisibleEntryFlags << MainMenuEntry_Count;

                if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN),
                    input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    SD_Call(Sfx_MenuMove);
                    g_GameWork.gameState = GameState_MainMenu;

                    if (g_GameWork.gameStateSteps[0] != 1)
                    {
                        g_GameWork.gameStateSteps[0] = 1;
                        Fs_QueueReset();
                    }
                }

                if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    g_MainMenu_SelectedEntry += MainMenuEntry_Count;
                    while (!(g_MainMenu_VisibleEntryFlags & (1 << --g_MainMenu_SelectedEntry)));
                }

                if (input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    while (!(g_MainMenu_VisibleEntryFlags & (1 << ++g_MainMenu_SelectedEntry)));
                }

                // Wrap selection.
                g_MainMenu_SelectedEntry %= MainMenuEntry_Count;

                if (input.GetAction(In::Enter).IsClicked())
                {
                    g_GameWork.gameState = GameState_MainMenu;

                    if (g_GameWork.gameStateSteps[0] != 1)
                    {
                        g_GameWork.gameStateSteps[0] = 1;
                        Fs_QueueReset();
                    }

                    ScreenFade_Start(true, false, false);
                    g_MainMenuState++;

                    if (g_MainMenu_SelectedEntry < MainMenuEntry_Start)
                    {
                        SD_Call(Sfx_MenuStartGame);
                    }
                    else
                    {
                        SD_Call(Sfx_MenuConfirm);
                    }

                    switch (g_MainMenu_SelectedEntry)
                    {
                        case MainMenuEntry_Continue:
                            if (g_GameWork.autosave.playerHealth > Q12(0.0f))
                            {
                                g_GameWork.savegame = g_GameWork.autosave;
                            }
                            else
                            {
                                GameFs_SaveLoadBinLoad();
                            }

                            //Game_PlayerInit();
                            g_SysWork.processFlags = ProcessFlag_Continue;
                            //GameFs_MapLoad(g_SavegamePtr->mapIdx);
                            break;

                        case MainMenuEntry_Load:
                            GameFs_SaveLoadBinLoad();
                            break;

                        case MainMenuEntry_Start:
                            ScreenFade_Reset();
                            g_MainMenuState = MainMenuState_DifficultySelector;
                            break;

                        case MainMenuEntry_Option:
                            GameFs_OptionBinLoad();
                            break;

                        case MainMenuEntry_Quit:
                            g_MainMenuState = MainMenuState_Quit;
                            break;
                    }
                }

                prevSavegameCount = g_MemCard_SavegameCount;
                break;

            case MainMenuState_DifficultySelector:
                if (playInGameDemo)
                {
                    //GameFs_MapStartup();

                    if (g_GameWork.gameStateSteps[0] == 1 && g_SysWork.gameStateStepCounter == 0)
                    {
                        g_Demo_ReproducedCount++;
                    }

                    if (g_GameWork.gameState == GameState_MainLoadScreen)
                    {
                        g_Demo_ReproducedCount++;
                    }
                }

                if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) ||
                    input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) ||
                    input.GetAction(In::Enter).IsClicked() ||
                    input.GetAction(In::Cancel).IsClicked())
                {
                    g_GameWork.gameState = GameState_MainMenu;

                    if (g_GameWork.gameStateSteps[0] != 1)
                    {
                        g_GameWork.gameStateSteps[0] = 1;
                        Fs_QueueReset();
                    }
                }

                // Scroll game difficulty options.
                if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    int prevGameDifficultyIdx = 2;
                    if (newGameSelectedDifficultyIdx > 0)
                    {
                        prevGameDifficultyIdx = newGameSelectedDifficultyIdx - 1;
                    }
                    newGameSelectedDifficultyIdx = prevGameDifficultyIdx;
                }
                if (input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    int nextGameDifficultyIdx = 0;
                    if (newGameSelectedDifficultyIdx < 2)
                    {
                        nextGameDifficultyIdx = newGameSelectedDifficultyIdx + 1;
                    }
                    newGameSelectedDifficultyIdx = nextGameDifficultyIdx;
                }

                // Play scroll sound.
                if (input.GetAction(In::Up).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN) ||
                    input.GetAction(In::Down).IsPulsed(GUI_PULSE_DELAY_SEC, GUI_PULSE_INITIAL_DELAY_SEC, GUI_PULSE_STATE_MIN))
                {
                    SD_Call(Sfx_MenuMove);
                }

                // Select game difficulty.
                if (input.GetAction(In::Enter).IsClicked())
                {
                    //Game_SavegameInitialize(0, newGameSelectedDifficultyIdx - 1);
                    //Game_PlayerInit();

                    g_SysWork.processFlags = ProcessFlag_NewGame;

                    //GameFs_MapLoad(MapIdx_MAP0_S00);
                    SD_Call(Sfx_MenuStartGame);

                    ScreenFade_Start(true, false, false);
                    g_MainMenuState = MainMenuState_NewGameStart;
                }
                // Cancel.
                else if (input.GetAction(In::Cancel).IsClicked())
                {
                    SD_Call(Sfx_MenuCancel);
                    g_MainMenuState = MainMenuState_Main;
                }
                break;

            case MainMenuState_LoadGame:
            case MainMenuState_NewGameStart:
                if (ScreenFade_IsFinished())
                {
                    Screen_Refresh(SCREEN_WIDTH, 0);
                    Fs_QueueWaitForEmpty();

                    if (g_GameWork.autosave.playerHealth > Q12(0.0f))
                    {
                        NEXT_GAME_STATES[1] = GameState_MainLoadScreen;
                    }

                    if (g_MainMenu_SelectedEntry == MainMenuEntry_Start)
                    {
                        //Chara_PositionSet(&g_MapOverlayHdr.mapPoints[0]);
                    }

                    //MemCard_SysDisable();

                    auto prevState                 = g_GameWork.gameState;
                    g_GameWork.gameStateSteps[0]   = prevState;
                    g_GameWork.gameState           = (e_GameState)NEXT_GAME_STATES[g_MainMenu_SelectedEntry];
                    g_SysWork.gameStateCounter     = 0;
                    g_GameWork.gameStatePrev       = prevState;
                    g_GameWork.gameStateSteps[0]   = 0;
                    g_SysWork.gameStateStepCounter = 0;
                    g_GameWork.gameStateSteps[1]   = 0;
                    g_GameWork.gameStateSteps[2]   = 0;

                    SysWork_StateSetNext(SysState_Gameplay);
                }
                break;

            case MainMenuState_Quit:
            {
                if (ScreenFade_IsFinished())
                {
                    g_App.Quit();
                }
                break;
            }
        }

        if (input.HasUserActionInput())
        {
            g_SysWork.gameStateStepCounter = 0;
        }

        if (!playInGameDemo)
        {
            switch (g_GameWork.gameStateSteps[0])
            {
                case 1:
                    if (g_SysWork.gameStateStepCounter > SECONDS_60_FPS(29))
                    {
                        g_GameWork.gameStateSteps[0]++;
                    }
                    break;

                case 2:
                    if (Fs_QueueGetLength() == 0)
                    {
                        g_Demo_ReproducedCount++;

                        g_GameWork.background2dColor.r = 0;
                        g_GameWork.background2dColor.g = 0;
                        g_GameWork.background2dColor.b = 0;

                        Game_StateSetNext(GameState_MovieIntro);
                    }
                    break;
            }
        }

        if (g_GameWork.gameState == GameState_MainMenu)
        {
            MainMenu_BackgroundDraw();
            //func_8003B560();

            if (g_MainMenuState < MainMenuState_DifficultySelector ||
                g_MainMenuState == MainMenuState_Quit)
            {
                MainMenu_MainTextDraw();
                return;
            }

            MainMenu_DifficultyTextDraw(newGameSelectedDifficultyIdx);
            return;
        }
        else
        {
            //*(s32*)0x1F800000 = 0x200000;
            //*(s32*)0x1F800004 = 0x01C00140;
            //ClearImage2((RECT*)0x1F800000, 0u, 0u, 0u);
            return;
        }
    }

    void MainMenu_SelectedOptionIdxReset() // 0x8003B550
    {
        g_MainMenu_SelectedEntry = MainMenuEntry_Continue;
    }

    void func_8003B7BC() // 0x8003B7BC
    {
        // Can't be `s32*` since 462 doesn't divide by 4, so I'm guessing it's `s8`.
        /*s8* s0 = 0x801E2432;

        memset(s0, 0, 462);
        D_800BCDE0 = s0;*/
    }

    static Color g_MainMenu_FogGradientMap2[256] =
    {
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::Black,
        Color::Black, Color::Black, Color::Black, Color::From8Bit(1, 1, 1),
        Color::From8Bit(2, 2, 2), Color::From8Bit(3, 4, 4), Color::From8Bit(5, 5, 5), Color::From8Bit(6, 7, 7),
        Color::From8Bit(7, 8, 8), Color::From8Bit(9, 10, 10), Color::From8Bit(10, 11, 12), Color::From8Bit(11, 13, 13),
        Color::From8Bit(13, 14, 15), Color::From8Bit(14, 16, 16), Color::From8Bit(15, 18, 18), Color::From8Bit(17, 19, 19),
        Color::From8Bit(18, 21, 21), Color::From8Bit(19, 22, 23), Color::From8Bit(21, 24, 24), Color::From8Bit(22, 25, 26),
        Color::From8Bit(23, 27, 27), Color::From8Bit(25, 28, 29), Color::From8Bit(26, 30, 30), Color::From8Bit(28, 32, 32),
        Color::From8Bit(29, 33, 34), Color::From8Bit(30, 35, 35), Color::From8Bit(32, 36, 37), Color::From8Bit(33, 38, 38),
        Color::From8Bit(34, 39, 40), Color::From8Bit(36, 41, 41), Color::From8Bit(37, 42, 43), Color::From8Bit(38, 44, 45),
        Color::From8Bit(40, 45, 46), Color::From8Bit(41, 47, 48), Color::From8Bit(42, 49, 49), Color::From8Bit(44, 50, 51),
        Color::From8Bit(45, 52, 52), Color::From8Bit(46, 53, 54), Color::From8Bit(48, 55, 56), Color::From8Bit(49, 56, 57),
        Color::From8Bit(50, 58, 59), Color::From8Bit(52, 59, 60), Color::From8Bit(53, 61, 62), Color::From8Bit(55, 63, 64),
        Color::From8Bit(55, 63, 65), Color::From8Bit(56, 64, 66), Color::From8Bit(57, 65, 67), Color::From8Bit(57, 65, 68),
        Color::From8Bit(58, 66, 69), Color::From8Bit(59, 67, 70), Color::From8Bit(59, 68, 71), Color::From8Bit(60, 68, 72),
        Color::From8Bit(61, 69, 73), Color::From8Bit(61, 70, 74), Color::From8Bit(62, 70, 75), Color::From8Bit(63, 71, 76),
        Color::From8Bit(63, 72, 77), Color::From8Bit(64, 73, 78), Color::From8Bit(65, 73, 80), Color::From8Bit(66, 74, 81),
        Color::From8Bit(66, 75, 82), Color::From8Bit(67, 76, 83), Color::From8Bit(68, 76, 84), Color::From8Bit(68, 77, 85),
        Color::From8Bit(69, 78, 86), Color::From8Bit(70, 78, 87), Color::From8Bit(70, 79, 88), Color::From8Bit(71, 80, 89),
        Color::From8Bit(72, 81, 90), Color::From8Bit(72, 81, 91), Color::From8Bit(73, 82, 92), Color::From8Bit(74, 83, 93),
        Color::From8Bit(75, 84, 95), Color::From8Bit(76, 84, 96), Color::From8Bit(77, 85, 97), Color::From8Bit(77, 86, 98),
        Color::From8Bit(78, 87, 99), Color::From8Bit(79, 87, 100), Color::From8Bit(80, 88, 101), Color::From8Bit(80, 89, 102),
        Color::From8Bit(81, 89, 104), Color::From8Bit(82, 90, 105), Color::From8Bit(83, 91, 106), Color::From8Bit(83, 92, 107),
        Color::From8Bit(84, 92, 108), Color::From8Bit(85, 93, 109), Color::From8Bit(86, 94, 110), Color::From8Bit(86, 94, 111),
        Color::From8Bit(87, 95, 112), Color::From8Bit(88, 96, 113), Color::From8Bit(89, 96, 114), Color::From8Bit(89, 97, 115),
        Color::From8Bit(90, 98, 116), Color::From8Bit(91, 99, 117), Color::From8Bit(92, 99, 119), Color::From8Bit(92, 100, 120),
        Color::From8Bit(93, 101, 121), Color::From8Bit(94, 101, 122), Color::From8Bit(95, 102, 123), Color::From8Bit(95, 103, 124),
        Color::From8Bit(96, 104, 125), Color::From8Bit(97, 104, 126), Color::From8Bit(98, 105, 127), Color::From8Bit(98, 106, 128),
        Color::From8Bit(99, 106, 129), Color::From8Bit(100, 107, 130), Color::From8Bit(101, 108, 131), Color::From8Bit(102, 109, 133),
        Color::From8Bit(103, 109, 128), Color::From8Bit(104, 110, 128), Color::From8Bit(104, 111, 129), Color::From8Bit(105, 112, 130),
        Color::From8Bit(106, 112, 131), Color::From8Bit(107, 113, 132), Color::From8Bit(108, 114, 133), Color::From8Bit(109, 115, 134),
        Color::From8Bit(110, 116, 135), Color::From8Bit(110, 117, 136), Color::From8Bit(111, 118, 137), Color::From8Bit(112, 119, 137),
        Color::From8Bit(113, 120, 138), Color::From8Bit(114, 120, 139), Color::From8Bit(115, 121, 140), Color::From8Bit(116, 122, 141),
        Color::From8Bit(116, 123, 142), Color::From8Bit(117, 124, 143), Color::From8Bit(118, 125, 144), Color::From8Bit(119, 126, 145),
        Color::From8Bit(120, 127, 146), Color::From8Bit(121, 128, 146), Color::From8Bit(122, 128, 147), Color::From8Bit(122, 129, 148),
        Color::From8Bit(123, 130, 149), Color::From8Bit(124, 131, 150), Color::From8Bit(125, 132, 151), Color::From8Bit(126, 133, 152),
        Color::From8Bit(127, 134, 153), Color::From8Bit(128, 135, 154), Color::From8Bit(129, 136, 155), Color::From8Bit(130, 137, 156),
        Color::From8Bit(131, 138, 156), Color::From8Bit(132, 139, 157), Color::From8Bit(133, 140, 158), Color::From8Bit(134, 141, 159),
        Color::From8Bit(135, 142, 160), Color::From8Bit(136, 143, 160), Color::From8Bit(137, 143, 161), Color::From8Bit(138, 144, 162),
        Color::From8Bit(139, 145, 163), Color::From8Bit(140, 146, 164), Color::From8Bit(141, 147, 164), Color::From8Bit(142, 148, 165),
        Color::From8Bit(143, 149, 166), Color::From8Bit(143, 150, 167), Color::From8Bit(144, 151, 167), Color::From8Bit(145, 152, 168),
        Color::From8Bit(146, 152, 169), Color::From8Bit(147, 153, 170), Color::From8Bit(148, 154, 171), Color::From8Bit(149, 155, 171),
        Color::From8Bit(150, 156, 172), Color::From8Bit(151, 157, 173), Color::From8Bit(152, 158, 174), Color::From8Bit(153, 159, 175),
        Color::From8Bit(154, 160, 175), Color::From8Bit(155, 161, 176), Color::From8Bit(156, 162, 177), Color::From8Bit(157, 162, 178),
        Color::From8Bit(158, 163, 179), Color::From8Bit(159, 164, 179), Color::From8Bit(159, 165, 180), Color::From8Bit(160, 166, 181),
        Color::From8Bit(161, 167, 182), Color::From8Bit(162, 168, 182), Color::From8Bit(163, 169, 183), Color::From8Bit(164, 170, 184),
        Color::From8Bit(165, 171, 185), Color::From8Bit(166, 171, 186), Color::From8Bit(167, 172, 186), Color::From8Bit(168, 173, 187),
        Color::From8Bit(169, 174, 188), Color::From8Bit(170, 175, 189), Color::From8Bit(171, 176, 190), Color::From8Bit(172, 177, 190),
        Color::From8Bit(173, 178, 191), Color::From8Bit(174, 179, 192), Color::From8Bit(175, 180, 193), Color::From8Bit(176, 181, 194)
    };

    static u32 g_MainMenu_FogGradientMap[256] =
    {
        0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000,
        0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000,
        0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000,
        0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000,
        0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000,
        0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000,
        0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000,
        0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000,
        0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A000000, 0x3A010101,
        0x3A020202, 0x3A040403, 0x3A050505, 0x3A070706, 0x3A080807, 0x3A0A0A09, 0x3A0C0B0A, 0x3A0D0D0B,
        0x3A0F0E0D, 0x3A10100E, 0x3A12120F, 0x3A131311, 0x3A151512, 0x3A171613, 0x3A181815, 0x3A1A1916,
        0x3A1B1B17, 0x3A1D1C19, 0x3A1E1E1A, 0x3A20201C, 0x3A22211D, 0x3A23231E, 0x3A252420, 0x3A262621,
        0x3A282722, 0x3A292924, 0x3A2B2A25, 0x3A2D2C26, 0x3A2E2D28, 0x3A302F29, 0x3A31312A, 0x3A33322C,
        0x3A34342D, 0x3A36352E, 0x3A383730, 0x3A393831, 0x3A3B3A32, 0x3A3C3B34, 0x3A3E3D35, 0x3A403F37,
        0x3A413F37, 0x3A424038, 0x3A434139, 0x3A444139, 0x3A45423A, 0x3A46433B, 0x3A47443B, 0x3A48443C,
        0x3A49453D, 0x3A4A463D, 0x3A4B463E, 0x3A4C473F, 0x3A4D483F, 0x3A4E4940, 0x3A504941, 0x3A514A42,
        0x3A524B42, 0x3A534C43, 0x3A544C44, 0x3A554D44, 0x3A564E45, 0x3A574E46, 0x3A584F46, 0x3A595047,
        0x3A5A5148, 0x3A5B5148, 0x3A5C5249, 0x3A5D534A, 0x3A5F544B, 0x3A60544C, 0x3A61554D, 0x3A62564D,
        0x3A63574E, 0x3A64574F, 0x3A655850, 0x3A665950, 0x3A685951, 0x3A695A52, 0x3A6A5B53, 0x3A6B5C53,
        0x3A6C5C54, 0x3A6D5D55, 0x3A6E5E56, 0x3A6F5E56, 0x3A705F57, 0x3A716058, 0x3A726059, 0x3A736159,
        0x3A74625A, 0x3A75635B, 0x3A77635C, 0x3A78645C, 0x3A79655D, 0x3A7A655E, 0x3A7B665F, 0x3A7C675F,
        0x3A7D6860, 0x3A7E6861, 0x3A7F6962, 0x3A806A62, 0x3A816A63, 0x3A826B64, 0x3A836C65, 0x3A856D66,
        0x3A806D67, 0x3A806E68, 0x3A816F68, 0x3A827069, 0x3A83706A, 0x3A84716B, 0x3A85726C, 0x3A86736D,
        0x3A87746E, 0x3A88756E, 0x3A89766F, 0x3A897770, 0x3A8A7871, 0x3A8B7872, 0x3A8C7973, 0x3A8D7A74,
        0x3A8E7B74, 0x3A8F7C75, 0x3A907D76, 0x3A917E77, 0x3A927F78, 0x3A928079, 0x3A93807A, 0x3A94817A,
        0x3A95827B, 0x3A96837C, 0x3A97847D, 0x3A98857E, 0x3A99867F, 0x3A9A8780, 0x3A9B8881, 0x3A9C8982,
        0x3A9C8A83, 0x3A9D8B84, 0x3A9E8C85, 0x3A9F8D86, 0x3AA08E87, 0x3AA08F88, 0x3AA18F89, 0x3AA2908A,
        0x3AA3918B, 0x3AA4928C, 0x3AA4938D, 0x3AA5948E, 0x3AA6958F, 0x3AA7968F, 0x3AA79790, 0x3AA89891,
        0x3AA99892, 0x3AAA9993, 0x3AAB9A94, 0x3AAB9B95, 0x3AAC9C96, 0x3AAD9D97, 0x3AAE9E98, 0x3AAF9F99,
        0x3AAFA09A, 0x3AB0A19B, 0x3AB1A29C, 0x3AB2A29D, 0x3AB3A39E, 0x3AB3A49F, 0x3AB4A59F, 0x3AB5A6A0,
        0x3AB6A7A1, 0x3AB6A8A2, 0x3AB7A9A3, 0x3AB8AAA4, 0x3AB9ABA5, 0x3ABAABA6, 0x3ABAACA7, 0x3ABBADA8,
        0x3ABCAEA9, 0x3ABDAFAA, 0x3ABEB0AB, 0x3ABEB1AC, 0x3ABFB2AD, 0x3AC0B3AE, 0x3AC1B4AF, 0x3AC2B5B0
    };

    const Color& GetFogColor(int idx)
    {
        int         idx0 = D_800BCDE0[idx];
        const auto& val  = g_MainMenu_FogGradientMap2[idx0];

        if (idx < 210)
        {
            return Color::Black;
        }

        return val;
    }

    u32 func_8003B7FC(s32 idx) // 0x8003B7FC
    {
        u8  idx0 = D_800BCDE0[idx];
        u32 val  = g_MainMenu_FogGradientMap[idx0];

        if (idx < 210)
        {
            return 0x3A000000;
        }

        return val;
    }

    PACKET* MainMenu_FogPacketGet(GsOT* ot, PACKET* packet) // 0x8003B838
    {
        s32      yOffset;
        s32      i;
        s32      j;
        s32      color0;
        s32      color2;
        s32      color3;
        s32      color1;
        POLY_G4* poly;

        for (i = 10; i < MAIN_MENU_FOG_COUNT; i++)
        {
            color1 = func_8003B7FC(MAIN_MENU_FOG_COUNT * (i - 1));
            color3 = func_8003B7FC(MAIN_MENU_FOG_COUNT * i);

            for (j = 1; j < MAIN_MENU_FOG_COUNT; j++)
            {
                color2 = color3;
                color0 = color1;

                color1 = func_8003B7FC(j + (MAIN_MENU_FOG_COUNT * (i - 1)));
                color3 = func_8003B7FC(j + (MAIN_MENU_FOG_COUNT * i));

                poly = (POLY_G4*)packet;
                setPolyG4(poly);

                yOffset = (i - 1) * 24;

                setXY4(poly,
                    -176 + (16 * j), yOffset - 208,
                    -160 + (16 * j), yOffset - 208,
                    -176 + (16 * j), yOffset - 184,
                    -160 + (16 * j), yOffset - 184);

                // Set colours.
                //*((u32*)&poly->r0) = color0;
                //*((u32*)&poly->r1) = color1;
                //*((u32*)&poly->r2) = color2;
                //*((u32*)&poly->r3) = color3;

                // Submit.
                //addPrim(ot, poly);
                //packet += sizeof(POLY_G4);
            }
        }

        return packet;
    }

    void MainMenu_FogDraw() // 0x8003BA08
    {
        PACKET*   packet;
        GsOT_TAG* tag;

        //tag    = g_OrderingTable2[g_ActiveBufferIdx].org;
        //packet = MainMenu_FogPacketGet((GsOT*)&tag[6], (PACKET*)GsOUT_PACKET_P);
        //SetDrawMode((DR_MODE*)packet, 0, 1, 42, nullptr);
        //addPrim(&tag[6], packet);
        //GsOUT_PACKET_P = packet + sizeof(DR_MODE);
    }

    void MainMenu_FogRandomize() // 0x8003BAC4
    {
        s32 idx;
        s32 i;
        s32 val;
        s8* ptr;
        u8* ptr1;
        s8* ptr2;

        static q19_12 randAngle = Q12_ANGLE(0.0f);

        return; // @todo

        ptr   = D_800BCDE0;
        ptr1  = (u8*)ptr + 441;
        randAngle += Rng_GenerateInt(4, 11u);
        val   = Q12_MULT(Math_Sin(randAngle), 10) - 122;
        ptr2  = ptr + 461;

        for (i = 20; i >= 0; i--)
        {
            *ptr2-- = val;
        }

        for (i = 0; i < 16; i++)
        {
            idx       = Rng_GenerateInt(0, MAIN_MENU_FOG_COUNT - 1);
            ptr1[idx] = NO_VALUE;
        }

        for (i = 0; i < 9; i++)
        {
            idx       = Rng_GenerateInt(0, MAIN_MENU_FOG_COUNT - 1);
            ptr1[idx] = 0;
        }
    }

    void MainMenu_FogScatter() // 0x8003BBF4
    {
        s32 i;
        s32 j;
        s32 val;
        u8* ptr;

        return; // @todo segfault

        MainMenu_FogRandomize();

        for (i = 0; i < MAIN_MENU_FOG_COUNT; i++)
        {
            ptr = (u8*)&D_800BCDE0[i * MAIN_MENU_FOG_COUNT];

            for (j = 0; j < MAIN_MENU_FOG_COUNT; j++)
            {
                val   = ptr[j + MAIN_MENU_FOG_COUNT];
                val  += ptr[j - 1];
                val  += ptr[j];
                val  += ptr[j + 1];
                val >>= 2;
                val--;

                if (val <= 0)
                {
                    ptr[j] = 0;
                }
                else
                {
                    ptr[j] = val;
                }
            }
        }
    }

    void MainMenu_FogUpdate() // 0x8003BC8C
    {
        static s32 foxCount = 0;

        if (foxCount == ((foxCount / 5) * 5))
        {
            MainMenu_FogScatter();
        }

        foxCount++;
        MainMenu_FogDraw();
    }

    void func_8003BCF4() // 0x8003BCF4
    {
        s32 i;

        func_8003B7BC();

        for (i = 0; i < 30; i++)
        {
            MainMenu_FogScatter();
        }
    }
}
