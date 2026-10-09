#pragma once

namespace Silent::Game
{
    void Anim_CharaTypeAnimInfoClear(void);

    void GameState_LoadScreen_Update(void);

    /** Handles `g_GameWork.gameStateSteps[0]`.
     * Used to handle map loading and room changes.
     */
    void GameBoot_InGameStartup(void);
}
