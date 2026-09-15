#pragma once

namespace Silent::Game
{
    /** Displays the Konami logo and starts loading base hero animations. */
    void GameState_KonamiLogo_Update();

    /** @brief Checks memory cards for free space & existing savegames.
     @return `e_KcetLogoStateStep`
    */
    s32 GameState_KcetLogo_MemCardCheck();

    /** Displays the KCET logo and starts loading various things. */
    void GameState_KcetLogo_Update();

    void BootScreen_KonamiScreenDraw();

    void BootScreen_KcetScreenDraw();
}
