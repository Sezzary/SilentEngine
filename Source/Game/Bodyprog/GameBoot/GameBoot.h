#pragma once

namespace Silent::Game
{
    /** @brief Handles the loading screen game state. */
    void GameState_LoadScreen_Update(void);

    /** @brief Handles the load of the in-game state and room transitions. */
    void GameBoot_InGameStartup(void);

    /** @brief Initalizes environmental effects and character info. Additionally, it resets the player state and sets
     * some info about the currently loaded overlay.
     */
    void GameBoot_InGameInit(void);

    /** @brief Load save's information. */
    void GameBoot_SavegameInitialize(s8 overlayId, s32 difficulty);

    /** @brief Initializes player and world data. */
    void GameBoot_WorldInit(void);

    /** @brief Loads a map file into `g_OvlDynamic`.
     *
     * @param mapIdx Index of the map to load.
     */
    void GameBoot_MapLoad(s32 mapIdx);
}
