#pragma once

namespace Silent::Game
{
    /** @brief Sets general environmental effects such as lighting, fog, and screen effects.
     *
     * @note Weathers conditions like snow, rain, and the ending's fire rain are set by
     * code exclusively found in the map overlay.
     */
    void GameBoot_WorldEnvInit(s32 unused);

    /** @brief Unused. */
    void func_80035B98(void);

    /** @unused and broken.
     * Intended to draw a background image when a loading screen with the string "STAGE X-X" appears.
     *
     * This loading screen will only work with the first two map overlays.
     */
    void GameBoot_LoadScreen_BackgroundImg(void);

    void GameBoot_LoadScreen_PlayerRun(void);
}
