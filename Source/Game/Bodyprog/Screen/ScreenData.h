#pragma once

#include "Game/Bodyprog/Bodyprog.h"

namespace Silent::Game
{
    extern s32  g_ActiveBufferIdx;
    extern GsOT g_OrderingTable0[2];
    extern GsOT g_OrderingTable1[2];
    extern GsOT g_OrderingTable2[2];

    /** Actual delta time since the previous tick.
     * Unlike `g_DeltaTime`, this value is never overridden by demos or game code.
     * Mostly seems used by UI code.
     */
    extern q19_12 g_DeltaTimeRaw;

    extern s32 g_IntervalVBlanks;

    // ====================
    // GLOBALS (BSS; Hack)
    // ====================
    // To match the order of the BSS segment, extern declarations
    // are required in a predetermined order.
    // This is done until a way to replicate `common`
    // segment behavior is found.

    extern s32      g_VBlanks;
    extern s32      g_UncappedVBlanks;
    extern GsOT_TAG g_OtTags0[2][16];

    /** Main gameplay timer. Code pauses gameplay by setting this to `Q12(0.0f)`.
     * Demos override this with the time from a demo file to ensure demos remain in sync. */
    extern q19_12 g_DeltaTime;

    extern GsOT_TAG g_OtTags1[2][ORDERING_TABLE_SIZE];

    /** Gravity speed for this tick. */
    extern q19_12 g_GravitySpeed;

    /** Number of ticks since game start. */
    extern s32 g_TickCount;

    extern s32 g_MainLoop_FrameCount;
}
