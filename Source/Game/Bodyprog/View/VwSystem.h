#pragma once

#include "Game/Bodyprog/View/Enums.h"
#include "Game/Bodyprog/View/Structs.h"
#include "Game/Bodyprog/View/VcMain.h"
#include "Game/Bodyprog/View/VcUtil.h"
#include "Game/Bodyprog/View/VwCalc.h"
#include "Game/Bodyprog/View/VwMain.h"

// TODO:
// - Split into 4 separate headers.
// - Add doc comments above func declarations.
// - Flags below are from SH2, most seem to match with SH but there might be some differences.

/** @brief GLOSSARY OF ABBREVIATIONS
 *
 * ACCEL: Acceleration
 * ANG:   Angle
 * ARY:   Array
 * CIR:   Circle
 * DEFLT: Default
 * EFF:   Effective?
 * EV:    Elevation
 * EXCL:  Exclusion
 * F:     Boolean or bit flag
 * FIX:   Fixed in place
 * FS:    Bit flags
 * GRND:  Ground
 * H:     Height
 * LIM:   Limit
 * MV:    Move
 * OFS:   Offset
 * P:     Pointer
 * PRIO:  Priority
 * PRM:   Parameter/parameters
 * R:     Radius
 * RD:    Road
 * SCR:   Screen
 * SPD:   Speed
 * STA:   Start
 * SV:    Swivel?
 * SW:    Switch
 * TGT:   Target
 * VELO:  Velocity
 * VB:    ?
 * VB:    ?
 * VC:    Virtual camera
 * VW:    View
 */

/** @brief GLOSSARY OF JAPANGLISH TERMS
 *
 * Area:         A camera path's spatial constraint defining its area of influence.
 * Entou:        "Cylinder" in Japanese. Refers to a 2D radius on the XZ plane.
 * Exclusion:    Threshold or boundary. Used in the context of a camera sphere.
 * Eye:          Camera heading angle.
 * Flipped:      ?
 * Limit area:   2D AABB parameters defining a camera path's spatial constraint.
 * Marge:        Merge.
 * Near road:    Nearby camera path collision containing info about a path's relation to the player in space
 *               (distance, priority, etc.).
 *               An array of these is kept to track which path should take effect.
 * Oresen hokan: Polyline interpolation, linear interpolation over an array of values.
 * Renewal:      Reset.
 * Road:         Camera path. A region which dictates specific camera behaviors while the player is inside.
 * Self view:    First-person view.
 * Suu:          "Count" in Japanese. Usually refers to the number of entries in an array or is included in enums to
 *               denote the number of entries it contains.
 * Switch:       ?
 * Through door: Rail camera.
 * Watch:        Camera look-at.
 */

namespace Silent::Game
{
    extern const VC_ROAD_DATA vcNullRoadArray[2];     /** Default camera paths. */
    extern VC_NEAR_ROAD_DATA  vcNullNearRoad;         /** Default nearby camera path collision. */
    extern VC_WATCH_MV_PARAM  deflt_watch_mv_prm;     /** Default look-at move parameters. */
    extern VC_WATCH_MV_PARAM  self_view_watch_mv_prm; /** First-person look-at move parameters. */
    extern VC_CAM_MV_PARAM    cam_mv_prm_user;        /** User camera move parameters. */
    extern q19_12             excl_r_ary[9];          /** Q19.12 | Exclusion radius array. */
    extern VC_WORK            vcWork;                 /** View camera workspace. */
    extern VW_VIEW_WORK       vwViewPointInfo;
    extern MATRIX             VwCrdVwMat;             /** Camera coordinate's world matrix. Its translation is the camera position. */
    extern MATRIX             VwCrdWsRotMat;          /** World-to-screen rotation, the transpose of `VwCrdVwMat`'s rotation. */
    extern VC_WATCH_MV_PARAM  vcWatchMvPrmSt;         /** View camera look-at move parameters start. */
    extern q19_12             vcSelfViewTimer;        /** View camera first-person timer. */

    /** @brief Sets the active camera path collision.
     *
     * @param work Camera workspace.
     * @param road Camera path collision to set as active.
     */
    static inline void Vc_CurNearRoadSet(VC_WORK* work, VC_NEAR_ROAD_DATA* road)
    {
        memcpy(&work->cur_near_road, road, sizeof(VC_NEAR_ROAD_DATA));
    }

    /** @brief Clears a set of camera flags.
     *
     * @param flag Camera flags to clear.
     */
    static inline void Vc_FlagClear(s32 flags)
    {
        vcWork.flags &= ~flags;
    }

    /** @brief Sets a set of camera flags.
     *
     * @param flag Camera flags to set.
 */
    static inline void Vc_FlagSet(s32 flags)
    {
        vcWork.flags |= flags;
    }
}
