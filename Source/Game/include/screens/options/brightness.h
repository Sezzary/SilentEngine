#pragma once

#include "Game/include/common.h"

namespace Silent::Game
{
    /** @brief Brightness menu states.
     *
     * When the user navigates the brightness menu, this facilitates menu switching
     * via `s_GameWork::gameStateStep[1]`.
     */
    enum e_BrightnessMenuState
    {
        BrightnessMenuState_0     = 0,
        BrightnessMenuState_1     = 1,
        BrightnessMenuState_2     = 2,
        BrightnessMenuState_Leave = 3
    };

    /** @brief Controller for the brightness options menu.
     *
     * Handles menu states, user input, SFX, and graphics drawing.
     */
    void Options_BrightnessMenu_Control(void);

    /** @brief Draws the string indicating the light level in the brightness options menu. */
    void Options_BrightnessMenu_ConfigDraw(void);

    /** @brief Draws arrows on the left and right edges of the screen indicating input directions in the brightness options menu.
     *
     * Called by `Options_BrightnessMenu_Control`.
     */
    void Options_BrightnessMenu_ArrowsDraw(void);
}
