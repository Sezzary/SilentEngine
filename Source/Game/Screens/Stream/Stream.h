#pragma once

namespace Silent::Game
{
    /** @brief Updates intro movie game state. */
    void GameState_MovieIntro_Update(void);

    /** @brief Updates opening movie game state. */
    void GameState_MovieOpening_Update(void);

    /** @brief Updates movie exit game state. */
    void GameState_ExitMovie_Update(void);

    /** @brief @unused Updates debug movie player game state. The movie to play is decided by `LStickLeft`/`LStickRight`. */
    void GameState_DebugMoviePlayer_Update(void);

    /** @brief Updates alternative intro movie game state. */
    void GameState_MovieIntroAlternate_Update(void);

    /** @brief Plays a fullscreen FMV.
     *
     * @param name FMV name.
     */
    bool PlayFmv(const std::string& name);
}
