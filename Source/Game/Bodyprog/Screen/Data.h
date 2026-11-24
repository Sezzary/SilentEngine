#pragma once

#include "Game/Bodyprog/Bodyprog.h"

namespace Silent::Game
{
    struct VC_WORK;
    struct VC_ROAD_DATA;

    constexpr int    TICKS_PER_SECOND = 60; /** Game has a variable timestep with 60 ticks max. */
    constexpr q19_12 TIMESTEP_30_FPS  = Q12(1.0f / (float)(TICKS_PER_SECOND / 2));
    constexpr q19_12 TIMESTEP_60_FPS  = Q12(1.0f / (float)(TICKS_PER_SECOND));

    constexpr int SCREEN_WIDTH                   = 320;
    constexpr int SCREEN_HEIGHT                  = 240;
    constexpr int FRAMEBUFFER_HEIGHT_PROGRESSIVE = 224;
    constexpr int FRAMEBUFFER_HEIGHT_INTERLACED  = FRAMEBUFFER_HEIGHT_PROGRESSIVE * 2;
    constexpr int ORDERING_TABLE_SIZE            = 2048;

    /** @brief Converts seconds to frames at 60 FPS.
     *
     * @param sec Seconds to convert.
     * @return Frames at 60 FPS.
     */
    #define SECONDS_60_FPS(sec) \
        (s32)((sec) * TICKS_PER_SECOND)

    /** @brief Converts seconds to frames at 60 FPS.
     *
     * @param sec Seconds to convert.
     * @return Frames at 60 FPS.
     */
    #define SECONDS_TO_FRAMES(sec) \
        (s32)((sec) * 30)

    /** @brief @deprecated Sync modes used by `DrawSync` and `VSync`. */
    enum e_SyncMode
    {
        SyncMode_Count     = -1,
        SyncMode_Wait      = 0,
        SyncMode_Immediate = 1,
        SyncMode_Wait2     = 2,
        SyncMode_Wait3     = 3,
        SyncMode_Wait8     = 8
    };

    extern GsOT_TAG g_OtTags0[2][16];
    extern GsOT_TAG g_OtTags1[2][ORDERING_TABLE_SIZE];
    extern s32      g_ActiveBufferIdx;

    extern s32 g_VBlanks;
    extern s32 g_UncappedVBlanks;

    /** Number of ticks since game start. */
    extern s32 g_TickCount;

    /** Main gameplay timer. Code pauses gameplay by setting this to `Q12(0.0f)`.
     * Demos override this with the time from a demo file to ensure deos remain in sync.
     *
     * Possible original name: `_shDeltaTimePerFrame`
     */
    extern q19_12 g_DeltaTime;

    extern q19_12 g_GravitySpeed; /** Gravity speed for this tick. */

    /** @note This is defined in `draw.c` because there's garbage data between this and the previous variable
     * (`g_Screen_PrevFadeProgressState`). It suggests the variable was defined in a file separate from `fade.c`.
     */
    extern q19_12 g_ScreenFadeTimestep;

    extern VC_WORK      vcWork;             /** View camera workspace. */
    extern VC_ROAD_DATA vcNullRoadArray[2]; /** Default camera paths. */

    extern GsOT g_OrderingTable0[2];
    extern GsOT g_OrderingTable1[2];
    extern GsOT g_OrderingTable2[2];

    extern q19_12 g_DeltaTimeRaw; /** Actual delta time since the previous tick. Unlike `g_DeltaTime`, this value is never overridden by demos or game code. Mostly seems used by UI code. */
    extern s32    g_IntervalVBlanks;

    extern s_FsImageDesc g_Font16AtlasImg;
    extern s_FsImageDesc g_KonamiLogoImg;
    extern s_FsImageDesc g_KcetLogoImg;
    extern s_FsImageDesc g_TitleImg;
    extern s_FsImageDesc g_PaperMapImg;
    extern s_FsImageDesc g_PaperMapMarkingAtlasImg;
    extern s_FsImageDesc g_ItemInspectionImg;
    extern s_FsImageDesc g_LoadingScreenImg;
    extern s_FsImageDesc g_ControllerButtonAtlasImg;
    extern s_FsImageDesc g_BrightnessScreenImg0;
    extern s_FsImageDesc g_BrightnessScreenImg1;
    extern s_FsImageDesc g_DeathTipImg;
    extern s_FsImageDesc g_HealthPortraitImg;
    extern s_FsImageDesc g_InventoryKeyItemTextureImg;
    extern s_FsImageDesc g_FirstAidKitItemTextureImg;

    /** Some intentory item texture (`ITEM/TIM07.TIM`). */
    extern s_FsImageDesc D_800A9074;

    extern s_FsImageDesc D_800A907C;

    extern s_FsImageDesc D_800A9084;

    extern s_FsImageDesc D_800A908C;

    extern s_FsImageDesc D_800A9094;

    extern s_FsImageDesc g_Font24AtlasImg;

    extern s_FsImageDesc D_800A90A4;

    extern s_FsImageDesc D_800A90B4;
}
