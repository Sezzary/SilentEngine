#pragma once

namespace Silent::Game
{
    enum e_MainMenuEntry
    {
        MainMenuEntry_Load     = 0,
        MainMenuEntry_Continue = 1,
        MainMenuEntry_Start    = 2,
        MainMenuEntry_Option   = 3,
        MainMenuEntry_Quit     = 4, /** @unused The extra options menu may have been accessible via the main menu. */

        MainMenuEntry_Count    = 5
    };

    enum e_MainMenuState
    {
        MainMenuState_Start              = 0,
        MainMenuState_Main               = 1,
        MainMenuState_LoadGame           = 2,
        MainMenuState_DifficultySelector = 3,
        MainMenuState_NewGameStart       = 4,
        MainMenuState_Quit               = 5
    };

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
