#pragma once

namespace Silent::Game
{
    /** @brief Main menu entries. */
    enum e_MainMenuEntry
    {
        MainMenuEntry_Load     = 0,
        MainMenuEntry_Continue = 1,
        MainMenuEntry_Start    = 2,
        MainMenuEntry_Option   = 3,
        MainMenuEntry_Quit     = 4, /** @unused The extra options menu may have been accessible via the main menu. */

        MainMenuEntry_Count    = 5
    };

    /** @brief Main menu states. */
    enum e_MainMenuState
    {
        MainMenuState_Start              = 0,
        MainMenuState_Main               = 1,
        MainMenuState_LoadGame           = 2,
        MainMenuState_DifficultySelector = 3,
        MainMenuState_NewGameStart       = 4,
        MainMenuState_Quit               = 5
    };

    /** Counts the amount of times that demos has been play in the current game session. */
    extern s8 g_Demo_ReproducedCount;

    /** Related to main menu fog randomization. */
    extern s8* D_800BCDE0; // Type assumed.

    void MainMenu_SelectedOptionIdxReset();

    void MainMenu_BackgroundDraw();

    void func_8003B7BC();

    u32 func_8003B7FC(s32 idx);

    PACKET* MainMenu_FogPacketGet(GsOT* ot, PACKET* packet);

    void MainMenu_FogDraw();

    void MainMenu_FogRandomize();

    void MainMenu_FogScatter();

    void MainMenu_FogUpdate();

    /** Related to main menu fog. */
    void func_8003BCF4();
}
