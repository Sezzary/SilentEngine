#pragma once

#include "Game/Bodyprog/Chara/Chara.h"
#include "Game/Bodyprog/Collision/Chara.h"
#include "Game/Bodyprog/Collision/Los.h"
#include "Game/Bodyprog/Collision/Ray.h"
#include "Game/Bodyprog/Collision/Trigger.h"
#include "Game/Bodyprog/Formats/Ipd.h"
#include "Game/Bodyprog/Map/Map.h"

// @todo Copy header again when decompiled code is finalised.

namespace Silent::Game
{
    constexpr q19_12 INTERSECTION_BUFFER    = Q12(0.1f);
    constexpr float  DEFAULT_CEILING_HEIGHT = -16.0f;

    /** @brief Collision types. */
    enum e_CollisionType
    {
        CollisionType_None = 0,
        CollisionType_Wall = 1,
        CollisionType_Unk2 = 2
    };

    enum e_SubChunkTransitionDirection
    {
        SubChunkTransitionDirection_None = 0,
        SubChunkTransitionDirection_Z    = 1,
        SubChunkTransitionDirection_X    = 2
    };

    /** @brief Collision point surface data. */
    struct s_CollisionSurface
    {
        q19_12 groundHeight;
        q3_12  tiltAngleX;
        q3_12  tiltAngleZ;
        s8     groundType; /** `e_GroundType` */
    };

    /** @brief Collision point data. */
    struct s_CollisionPoint
    {
        VECTOR3            position;   /** Q19.12 */
        s_CollisionSurface surface;
        s32                groundType; /** `e_GroundType` */
    };

    /** @brief Collision point probe result. */
    struct s_CollisionResult
    {
        VECTOR3            offset; /** Q19.12 */
        s_CollisionSurface surface;
        q19_12             ceilingHeight;
    };

    typedef struct
    {
        u8    subChunkTransDir; /**`e_SubChunkTransitionDirection` */
        u8    surfaceIdx;       // Index for `s_IpdCollisionData::surfaces`.
        q7_8  collDiffDist;
        q23_8 radiusCollDiffDist;
    } s_CollisionState_A8;

    typedef struct
    {
        s8      field_0;
        DVECTOR field_2; // Q3.12 | Angle constraint. X is min, Y is max. TODO: Don't use `DVECTOR` for this anymore.
    } s_CollisionState_44_0;

    typedef struct
    {
        s_CollisionState_44_0 field_0;
        s16                   field_6;
        s_CollisionState_44_0 field_8;
        s16                   field_E;
        s8*                   field_10[8];
        s_CollisionState_44_0 field_30;
        s16                   field_36;
    } s_CollisionState_44;

    typedef struct
    {
        DVECTOR_XZ charaVertDiff;
        DVECTOR_XZ charaMoveVertDiff;
        u32        subChunkTransDir;   /** `e_SubChunkTransitionDirection` */
        q23_8      radiusCollDiffDist; // Radius?
        q7_8       collDiffDist;
        s8         __pad[2];
        DVECTOR_XZ charaMoveOffset;
    } s_CollisionState_CC_20;

    typedef union
    {
        q7_8 field_0; // Something related to character radius???
        struct
        {
            u8 surfaceIdx0;
            u8 surfaceIdx1;
        } cellSurfaces;
    } s_CollisionState_CC_C;

    /** @brief Collision cell point data. */
    struct s_CollisionCellPoint
    {
        s_IpdCollisionData*    ipdCollisionData;
        u8                     subcellIdx; // `s_IpdCollisionData::subcells` index.
        u8                     heightDisabled;
        SVECTOR3               field_6; // Q7.8 | Probe position inside cell/subcell?
        s_CollisionState_CC_C  field_C;
        u8                     field_E;               // } Index from `s_IpdCollisionData::field_6_8`.
        u8                     field_F;               // }
        u8                     disableSurface0Height; /** `bool` */
        u8                     disableSurface1Height; /** `bool` */
        SVECTOR3               splitVertex0;          /** Q23.8 | Data from `s_IpdCollisionData::splitVertices` */
        SVECTOR3               splitVertex1;          /** Q23.8 | Data from `s_IpdCollisionData::splitVertices` */
        s_CollisionState_CC_20 field_20;
    };

    /** @brief Character collision state. */
    struct s_CollisionCharaState
    {
        s32        collisionState; /** `e_CharaCollisionState` */
        bool       field_4;        // Flag set when the character collisions being check is any of the ones being
                                 // in the of `Collision_CharaCollisionSetup`.
        q19_12     distance;
        SVECTOR    offset; /** Q23.8 | Character movement offset. */
        DVECTOR_XZ direction;
        q23_8      positionFromX;
        q23_8      positionFromZ;
        q23_8      positionToX;
        q23_8      positionToZ;
        q7_8       radius;
        q7_8       topPos;
        q7_8       bottomPos;
    };

    struct s_CollisionState
    {
        u8                    field_0_0     : 8;  /** `bool` TODO: Check, code only assigns 0 or 1. */
        s8                    isCharaMoving : 1;  /** `bool` */
        s8                    field_0_9     : 1;  /** `bool` */
        s8                    field_0_10    : 1;  /** `bool` */
        u16                   flags         : 16; /** `e_CollisionTriggerFlags` */
        s_CollisionCharaState charaState;
        s32                   field_34;
        s16                   field_38;
        q11_4                 field_3A;
        s16                   field_3C; // X?
        s16                   field_3E; // Z?
        s8*                   field_40;
        s_CollisionState_44   field_44;
        q23_8                 slopedGroundHeight; // TODO: How is `groundHeight` different?
        q23_8                 charaCellOffsetX; // } Character position in cell.
        q23_8                 charaCellOffsetZ; // } 
        q19_12                tiltAngleX;
        q19_12                tiltAngleZ;
        bool                  heightDisabled; /** `bool` | TODO: Does disabled height indicate a wall or pit? */
        s32                   groundType;     /** `e_GroundType` */

        union
        {
            //DVECTOR_XZ offset; /** Q23.8 */
            s32        field_0;
        } charaPositionFrom;
        union
        {
            //DVECTOR_XZ offset; /** Q23.8 */
            s32        field_0;
        } charaPositionTo;
         union
         {
            struct
            {
                u8                     closestXSubcellIdx;
                u8                     closestZSubcellIdx;
                u8                     closeFarXSubcellIdxDiff;
                u8                     closeFarZSubcellIdxDiff;
                s_IpdCollSubcellRange* subcellRange;
                s_CollisionState_A8    field_8[4];
            } s_0;
            struct
            {
                q7_8 field_0; // Set to absolute character bottom height.
                q7_8 field_2; // Set to absolute character top height.
                s16  field_4;
                u8   collisionState; /** `e_CharaCollisionState` */
                u8*  field_8;
            } s_1;
        } field_A0;

        u8                   subcellIdx; /** Secondary. Used only if detecting a subcell after not finding one. */
        q7_8                 groundHeight; // TODO: Base ground height with slope unaccounted for?
        s_CollisionCellPoint point;
        // TODO: Maybe incomplete. Maybe not, added the final padding based on `Collision_SurfaceGet`.
    };

    extern u16 g_CollisionTriggerFlags;

    // @todo Decompiled functions need more work before porting.
}
