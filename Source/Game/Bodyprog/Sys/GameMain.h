#pragma once

namespace Silent::Game
{
    /** @brief Reset types used in `MainLoop_ShouldWarmReset`. */
    enum e_ResetType
    {
        ResetType_None     = 0,
        ResetType_WarmBoot = 2
    };

    extern s32 g_WarmBootTimer;

    extern void (*g_GameStateUpdateFuncs[])(); // Not originally extern.

    /** @brief Checks for a specific combination of inputs to determine if a soft reboot should take place.
     *
     * @return `ResetType_WarmBoot` if a soft reboot should take place, `ResetType_None` otherwise.
     * @note OPM counterpart of this function also returns `4` which is used to kill game's process.
     */
    s32 MainLoop_ShouldWarmReset();

    void GameState_AutoLoadSavegame_Update();
    void GameState_MovieIntro_Update();
    void GameState_MainMenu_Update();
    void GameState_MovieOpening_Update();
    void GameState_LoadScreen_Update();
    void GameState_InGame_Update();
    void GameState_MapEvent_Update();
    void GameState_ExitMovie_Update();
    void GameState_ItemScreens_Update();
    void GameState_PaperMapScreen_Update();
    void GameState_LoadSavegameScreen_Update();
    void GameState_Options_Update();
    void GameState_LoadMapScreen_Update();
    void GameState_Credits_Update();
}
