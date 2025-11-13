#pragma once

#include "Game/Common.h"

/** @brief Screen position menu states. Facilitates menu switching via `s_GameWork::gameStateStep[1]`. */
enum e_ScreenPosMenuState
{
    ScreenPosMenuState_0     = 0,
    ScreenPosMenuState_1     = 1,
    ScreenPosMenuState_2     = 2,
    ScreenPosMenuState_Leave = 3
};

extern bool     g_ScreenPosMenu_InvertBackgroundFade;
extern int      g_ScreenPosMenu_BackgroundFade;
extern Vector2i g_ScreenPosMenu_Position;

/** @brief Controller for the screen position options menu.
 *
 * Handles menu states, user input, SFX, and graphics drawing.
 */
void Options_ScreenPosMenu_Control();

/** @brief Draws the arrows at the edges of the screen indicating input directions in the screen position options menu.
 *
 * Called by `Options_ScreenPosMenu_Control`.
 */
void Options_ScreenPosMenu_ArrowsDraw();

/** @brief Draws the screen position configuration string and its background box. */
void Options_ScreenPosMenu_ConfigDraw();
