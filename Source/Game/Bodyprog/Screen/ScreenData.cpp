#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Screen/ScreenData.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Game/Bodyprog/Screen/ScreenDraw.h"

namespace Silent::Game
{
    s32      g_ActiveBufferIdx; // Not originally here.
    s32      g_VBlanks;
    s32      g_UncappedVBlanks;
    GsOT_TAG g_OtTags0[2][16];
    q19_12   g_DeltaTime;
    GsOT_TAG g_OtTags1[2][ORDERING_TABLE_SIZE];
    q19_12   g_GravitySpeed;
    s32      g_TickCount;

    // SH2 symbols indicate this should be in `vc_main.c`, but the file already has its data segment assigned.
    // It's possible this is part of another split as there is also some inbetween data in
    // the bss data segment related to the view camera system.
    VC_ROAD_DATA vcNullRoadArray[] =
    {
        {
            .lim_sw          = { Q8(-30.0f), Q8(30.0f), Q8(-30.0f), Q8(30.0f) },
            .lim_rd          = { Q8(-30.0f), Q8(30.0f), Q8(-30.0f), Q8(30.0f) },
            .flags          = VC_RD_NOFLAG,
            .area_size_type = VC_AREA_WIDE,
            .rd_type        = VC_RD_TYPE_ROAD,
            .mv_y_type      = VC_MV_SELF_VIEW,
            .lim_rd_max_hy  = Q4(6.0f),
            .lim_rd_min_hy  = Q4(-6.0f),
            .ofs_watch_hy   = Q4(0.5f),
            .field_15          = 1,
            .cam_mv_type    = VC_MV_CHASE,
            .fix_ang_x      = Q8_ANGLE(0.0f),
            .fix_ang_y      = Q8_ANGLE(0.0f)
        },
        {
            .lim_sw          = { Q8(0.0f), Q8(0.0f), Q8(0.0f), Q8(0.0f) },
            .lim_rd          = { Q8(0.0f), Q8(0.0f), Q8(0.0f), Q8(0.0f) },
            .flags          = VC_RD_END_DATA_F,
            .area_size_type = VC_AREA_TINY,
            .rd_type        = VC_RD_TYPE_ROAD,
            .mv_y_type      = VC_MV_CHASE,
            .lim_rd_max_hy  = Q4(0.0f),
            .lim_rd_min_hy  = Q4(0.0f),
            .ofs_watch_hy   = Q4(0.0f),
            .field_15          = 0,
            .cam_mv_type    = VC_MV_CHASE,
            .fix_ang_x      = Q8_ANGLE(0.0f),
            .fix_ang_y      = Q8_ANGLE(0.0f)
        },
    };

    GsOT g_OrderingTable0[2] =
    {
        { 11, &g_OtTags1[0][1], 0, 0, 0 },
        { 11, &g_OtTags1[1][1], 0, 0, 0 }
    };

    GsOT g_OrderingTable1[2] =
    {
        { 9, (GsOT_TAG*)FS_BUFFER_1, 0, 0, 0 },
        { 9, (GsOT_TAG*)0x801E2E00, 0, 0, 0 }
    };

    GsOT g_OrderingTable2[2] =
    {
        { 4, &g_OtTags0[0][0], 0, 0, 0 },
        { 4, &g_OtTags0[1][0], 0, 0, 0 }
    };

    q19_12 g_DeltaTimeRaw    = TIMESTEP_60_FPS;
    s32    g_IntervalVBlanks = 1;

    s_FsImageDesc g_PaperMapImg                = {};
    s_FsImageDesc g_PaperMapMarkingAtlasImg    = {};
    s_FsImageDesc g_ItemInspectionImg          = {};
    s_FsImageDesc g_LoadingScreenImg           = {}; // @unused? Uses a ground texture.
    s_FsImageDesc g_ControllerButtonAtlasImg   = {};
    s_FsImageDesc g_HealthPortraitImg          = {};
    s_FsImageDesc g_InventoryKeyItemTextureImg = {};
    s_FsImageDesc g_FirstAidKitItemTextureImg  = {};
    s_FsImageDesc D_800A9074                   = {};
    s_FsImageDesc D_800A907C                   = {};
    s_FsImageDesc D_800A9084                   = {};
    s_FsImageDesc D_800A908C                   = {}; // } Effect textures.
    s_FsImageDesc D_800A9094                   = {}; // }
    /* Not sure if these are really `s_FsImageDesc`. Seems plausible but not sure about negative values here. */
    s_FsImageDesc D_800A90A4                   = {};
    s_FsImageDesc D_800A90AC                   = {};
    s_FsImageDesc D_800A90B4                   = {};
    s_FsImageDesc D_800A90BC                   = {};
    s_FsImageDesc D_800A90C4                   = {};
    s_FsImageDesc D_800A90CC                   = {};
    s_FsImageDesc D_800A90D4                   = {};
    s_FsImageDesc D_800A90DC                   = {};
    s_FsImageDesc D_800A90E4                   = {};
    s_FsImageDesc D_800A90EC                   = {};
    s_FsImageDesc D_800A90F4                   = {};
}
