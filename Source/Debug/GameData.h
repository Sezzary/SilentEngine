#pragma once

namespace Silent::Debug
{
    /** @temp Need debug pointers to circumvent the problem of dirty PSX macros clashing with the C++ standard library
     * and other dependencies.
     */

    extern int* g_SysState;
    extern int* g_SysStateSteps;
    extern int* g_SysStateStepData;
    extern int* g_SysStateCounter;
    extern int* g_GameState;
    extern int* g_GameStateSteps;
    extern int* g_GameStateCounter;
    extern int* g_GameStateStepCounter;
}
