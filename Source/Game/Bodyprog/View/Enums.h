#pragma once

namespace Silent::Game
{
    /** @brief Lens flare rotation types. TODO: Rough name. Also probably belongs in a different file. */
    enum e_LensFlareType
    {
        LensFlareType_Default = 0,
        LensFlareType_Custom  = 1,
        LensFlareType_Preset1 = 2,
        LensFlareType_Preset2 = 3,
        LensFlareType_Preset3 = 4,
        LensFlareType_Preset4 = 5
    };

    /** @brief Camera path flags. */
    enum VC_ROAD_FLAGS
    {
        VC_RD_NOFLAG            = 0,
        VC_RD_END_DATA_F        = 1 << 0,
        VC_RD_WARP_IN_F         = 1 << 1,                             /** Warp in. */
        VC_RD_WARP_OUT_F        = 1 << 2,                             /** Warp out. */
        VC_RD_WARP_IN_OUT_FS    = VC_RD_WARP_IN_F | VC_RD_WARP_OUT_F, /** Warp in-out. */
        VC_RD_NO_FRONT_FLIP_F   = 1 << 3,
        VC_RD_LIM_UP_FAR_VIEW_F = 1 << 4,
        VC_RD_USE_NO_ENEMY_F    = 1 << 5,
        VC_RD_USE_NEAR_ENEMY_F  = 1 << 6,
        VC_RD_MARGE_ROAD_F      = 1 << 7
    };

    /** @brief Camera flags. */
    enum VC_FLAGS
    {
        VC_NOFLAG                = 0,
        VC_USER_CAM_F            = 1 << 0, /** User in control? */
        VC_USER_WATCH_F          = 1 << 1, /** Lock look-at on player? */
        VC_WARP_CAM_F            = 1 << 2, /** Warp position. */
        VC_WARP_WATCH_F          = 1 << 3, /** Warp look-at. */
        VC_WARP_CAM_TGT_F        = 1 << 4, /** Warp target position? */
        VC_SWITCH_NEAR_RD_DATA_F = 1 << 5,
        VC_PROJ_MOMENT_CHANGE_F  = 1 << 6,
        VC_VISIBLE_CHARA_F       = 1 << 7,
        VC_INHIBIT_FAR_WATCH_F   = 1 << 8,
        VC_PRS_F_VIEW_F          = 1 << 9,
        VC_OLD_PRS_F_VIEW_F      = 1 << 10
    };

    /** @brief Camera move types. */
    enum VC_CAM_MV_TYPE
    {
        VC_MV_CHASE        = 0, /** Chase player. */
        VC_MV_SETTLE       = 1, /** Settle to rest. */
        VC_MV_FIX_ANG      = 2, /** Fixed angle. */
        VC_MV_SELF_VIEW    = 3, /** Third-person view. */
        VC_MV_THROUGH_DOOR = 4, /** Along rail. */

        VC_MV_SUU          = 5  /** Count. */
    };

    /** @brief Camera check types. */
    enum VC_CAM_CHK_TYPE
    {
        VC_CHK_NEAREST_ROAD_TYPE   = 0, /** Closest camera path. */
        VC_CHK_NEAREST_SWITCH_TYPE = 1  /** Closest switch type. */
    };

    /** @brief Camera path types. */
    enum VC_ROAD_TYPE
    {
        VC_RD_TYPE_ROAD           = 0, /** Regular camera path. */
        VC_RD_TYPE_EFFECT         = 1, /** Effect camera path. */
        VC_RD_TYPE_EVENT          = 2, /** Event camera path. */
        VC_RD_TYPE_ROAD_PRIO_LOW  = 3, /** Low-priority camera path. */
        VC_RD_TYPE_ROAD_PRIO_HIGH = 4, /** High-priority camera path. */
        VC_RD_TYPE_SV_ONLY        = 5, /** @todo */

        VC_RD_TYPE_SUU            = 6  /** Count. */
    };

    /** @brief Camera path direction binding types.
     *
     * @note Not defined in SH2 symbols. Values are taken from the `vrDispMsgVC_ROAD_DIR_TYPE` SH2 debug function.
     */
    enum VC_ROAD_DIR_TYPE
    {
        VC_RD_DIR_Z = 0, /** Bound on X-axis. */
        VC_RD_DIR_X = 1  /** Bound on Z-axis. */
    };

    /** @brief Camera path spatial constraint size types. */
    enum VC_AREA_SIZE_TYPE
    {
        VC_AREA_TINY    = 0,
        VC_AREA_SMALL   = 1,
        VC_AREA_WIDE    = 2,
        VC_AREA_OUTDOOR = 3,

        VC_AREA_SUU     = 4 /** @unused Count. */
    };

    /** @brief Rail camera set command types. */
    enum THROUGH_DOOR_SET_CMD_TYPE
    {
        VC_TDSC_START = 0,
        VC_TDSC_END   = 1,
        VC_TDSC_MAIN  = 2
    };

    /** @brief Camera anchor types. */
    enum e_CameraAnchor
    {
        CameraAnchor_Character = 0,
        CameraAnchor_Ground    = 1,
        CameraAnchor_Camera    = 2 // TODO: Name uncertain. Relies on some backup camera Y value? -- Sezz
    };

    /** @brief Debug camera mode state steps. */
    enum e_DebugCameraMode
    {
        DebugCameraMode_Collide            = 0,
        DebugCameraMode_SetReference       = 1,
        DebugCameraMode_AnalogStickControl = 2,
        DebugCameraMode_ResetReference     = 3, // TODO: Name uncertain.
        DebugCameraMode_Init               = 4  // TODO: Name uncertain.
    };
}
