#pragma once

#include "Game/Bodyprog/Chara/Chara.h"
#include "Game/Bodyprog/Collision/Chara.h"
#include "Game/Bodyprog/Collision/Los.h"
#include "Game/Bodyprog/Collision/Ray.h"
#include "Game/Bodyprog/Collision/Trigger.h"
#include "Game/Bodyprog/Formats/Ipd.h"
#include "Game/Bodyprog/Map/Map.h"

namespace Silent::Game
{
    constexpr q19_12 INTERSECTION_BUFFER    = Q12(0.1f);
    constexpr float  DEFAULT_CEILING_HEIGHT = -16.0f;

    /** @brief Global collision flags.
     * Applies for both NPCs and the player.
     */
    enum e_CollisionTriggerFlags
    {
        CollisionTriggerFlag_None = 0,
        CollisionTriggerFlag_0    = 1 << 0, // Enables map collisions.
        CollisionTriggerFlag_1    = 1 << 1, // Enables objects collisions.
        CollisionTriggerFlag_2    = 1 << 2, // Enables alternative objects collisions?
        CollisionTriggerFlag_3    = 1 << 3, /** @unused Only ever called in `MAP6_S05`. */
        CollisionTriggerFlag_All  = 0xFFFF
    };

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
        // 3 bytes of padding.
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

    // emoose: Also works: `extern u16 g_ActiveCollisionTriggers[];`, `arg0->field_4 = g_ActiveCollisionTriggers[0];`.
    // Didn't see any array accesses in Ghidra though, struct might be more likely.
    extern s_ActiveCollisionTriggers g_ActiveCollisionTriggers;

    extern u16 g_CollisionTriggerFlags;

    // ========================================
    // @split? Collision init, flags, getters.
    // ========================================

    /** @brief Initializes the collision subsystem, resetting flags and clearing the trigger zone count. */
    void Collision_Init(void);

    /** @brief Gets the active collision flags. */
    u16 Collision_FlagsGet(void);

    /** @brief Sets the active collision flags.
     *
     * @param collFlags New collision flags.
     */
    void Collision_FlagsSet(u16 collFlags);

    /** @brief Sets additional active collision flags using OR.
     *
     * @param collFlags Collision flag bits to set.
     */
    void Collision_FlagBitsSet(u16 collFlags);

    void Collision_FlagBitsClear(s32 collFlags);

    /** @brief Runs through collision triggers and collects those containing the given XZ position.
     *
     * @param posX Query X position.
     * @param posZ Query Z position.
     * @param triggers Array of collision triggers.
     */
    void Collision_NearbyTriggersGet(q19_12 posX, q19_12 posZ, s_CollisionTrigger* triggers);

    // ========================================
    // @split? IPD functions.
    // ========================================

    void Ipd_CollisionPtrsInit(s_IpdCollisionData* collData);

    void Collision_SubcellChecksReset(s_IpdCollisionData* collData);

    void Collision_SubcellChecksCountUpdate(s_IpdCollisionData* collData);

    // ========================================
    // @split? IPD/ground collision detection.
    // ========================================

    /** @brief Gets the collision surface at a given 2D world position.
     *
     * @param surface Output collision surface.
     * @param posX X world position.
     * @param posZ Z world position.
     */
    void Collision_SurfaceGet(s_CollisionSurface* surface, q19_12 posX, q19_12 posZ);

    /** @brief Detects a wall collision using the scratchpad for performance.
     *
     * @param collResult Output collision result.
     * @param moveOffset Movement offset.
     * @param chara Character to check.
     * @return Wall response code.
     */
    s32 Collision_WallDetect(s_CollisionResult* collResult, const VECTOR3* moveOffset, s_SubCharacter* chara);

    /** @brief Handles a wall collision response by sampling the ground at 9 points around a character.
     *
     * @param collResult Output collision result.
     * @param moveOffset Movement offset (unused).
     * @param chara Character to check.
     * @param response Initial collision pass response.
     * @return Wall response code.
     */
    s32 Collision_WallResponse(s_CollisionResult* collResult, const VECTOR3* moveOffset, s_SubCharacter* chara, s32 response);

    /** @brief Probes ground heights at 16 radial points to determine the terrain slope direction.
     * Creates a terrain avoidance force to push away from illegal positions.
     *
     * @param collResult Output collision result with XZ offset vector pointing away from the highest terrain.
     * @param pos Center position.
     * @param startGroundHeight Starting character Y position.
     * @param startHeadingAngle Starting heading angle on the XZ plane for the probe circle.
     */
    void Collision_GroundProbeRadial(s_CollisionResult* collResult, const VECTOR3* pos,
                                    q19_12 startGroundHeight, q19_12 startHeadingAngle);

    /** @brief Applies collision detection for a character's movement offset.
     *
     * @param collResult Output collision result.
     * @param moveOffset Movement offset.
     * @param chara Character performing the movement.
     * @return Collision result response.
     */
    bool Collision_CharaCollisionSetup(s_CollisionResult* collResult, const VECTOR3* moveOffset, s_SubCharacter* chara);

    /** @brief Initializes a default collision result with a position and ground height.
     *
     * @param result Collision result to initialize.
     * @param offsetX X offset.
     * @param offsetY Y offset.
     * @param offsetZ Z offset.
     * @param groundHeight Ground height.
     */
    void Collision_DefaultResultSet(s_CollisionResult* collResult, q19_12 offsetX, q19_12 offsetY, q19_12 offsetZ, q19_12 groundHeight);

    /** @brief Gets an array of collidable characters for collision testing.
     *
     * @param collCharaCount Output number of collidable characters.
     * @param excludedChara Character to exclude, typically the one being tested.
     * @param includePlayer Filter out the player.
     * @return Collidable characters.
     */
    s_SubCharacter** Collision_CollidableCharasGet(s32* collCharaCount, const s_SubCharacter* excludedChara, bool includePlayer);

    /** @brief Checks a movement offset against a collision result.
     *
     * @param collResult Collision result.
     * @param offset Movement offset to test.
     * @param cylinder Collision cylinder.
     * @return `true` if movement is possible, `false` otherwise.
     */
    s32 Collision_OffsetCheck(s_CollisionResult* collResult, VECTOR* offset, const s_CollisionCylinder* cylinder);

    s32 func_8006A42C(s_CollisionResult* collResult, const VECTOR3* offset, const s_CollisionCylinder* cylinder);

    bool func_8006A4A8(s_CollisionResult* collResult, VECTOR3* moveOffset, const s_CollisionCylinder* cylinder, bool arg3,
                    s_IpdCollisionData** collDataPtrs, s32 collDataIdx, s_func_8006CF18* arg6, s32 arg7,
                    s_SubCharacter** charas, s32 charaCount);

    /** @brief Slows down colliding characters according to relational cylinder collision. */
    void Collision_TargetCharaCollidingSlowDown(VECTOR3* offset, const s_CollisionCylinder* cylinder, s_SubCharacter** charas, s32 charaCount);

    /** @brief Initializes a collision state for a new pass.
     *
     * @param state Collision state to initialize.
     * @param moveOffset Movement offset.
     * @param cylinder Collision cylinder.
     * @param arg3 Configuration flag. TODO: What is it?
     */
    //void Collision_CollStateInit(s_CollisionState* state, VECTOR3* moveOffset, const s_CollisionCylinder* cylinder, bool arg3);
//
    //// ========================================
    //// @split? Collision state fill.
    //// ========================================
//
    ///** @brief Calculates the movement direction vector and distance from a position offset.
    // *
    // * @param charaState Character collision state.
    // * @param moveOffset Movement offset.
    // * @param cylinder Collision cylinder.
    // */
    //void Collision_MoveDirectionCalc(s_CollisionCharaState* charaState,
    //                                const VECTOR3* moveOffset, const s_CollisionCylinder* cylinder);
//
    //void Collision_CharaCollisionHandle(s_CollisionState* state, s_IpdCollisionData* collData);
//
    //bool Collision_SubcellInit(s_CollisionState* state, const s_IpdCollisionData* collData);
//
    //bool Collision_SubcellIdxGet(s_CollisionState* state, const s_IpdCollisionData* collData);
//
    //void func_8006B1C8(s_CollisionState* state, s_IpdCollisionData* collData, s_IpdCollSubcellRange* subcellRanges);
//
    //bool func_8006B318(s_CollisionState* state, const s_IpdCollisionData* collData, s32 subcellIdx);
//
    ///** `arg1` is unused, but `func_8006B1C8` passes second arg to this. */
    //void func_8006B6E8(s_CollisionState* state, s_IpdCollSubcellRange* subcellRanges);
//
    //bool func_8006B7E0(s_CollisionState_A8* cur, s_CollisionState_CC_20* prev);
//
    //void func_8006B8F8(s_CollisionCellPoint* cellPoint);
//
    //void func_8006B9C8(s_CollisionState* state);
//
    //void func_8006BB50(s_CollisionState* state, s32 arg1);
//
    //q23_8 func_8006BC34(s_CollisionState* state);
//
    ///** `arg3` and `arg4` might be XY or XZ position components. */
    //void func_8006BCC4(s_CollisionState_44* arg0, s8* arg1, u32 arg2, q7_8 distX, q7_8 distZ, q7_8 arg5);
//
    //void func_8006BDDC(s_CollisionState_44_0* arg0, q3_12 rotX, q3_12 rotY);
//
    //void func_8006BE40(s_CollisionState* state);
//
    //void func_8006BF88(s_CollisionState* state, const SVECTOR3* splitVert);
//
    //void func_8006C0C8(s_CollisionState* state, s16 arg1, q7_8 arg2);
//
    //bool func_8006C1B8(u32 arg0, q3_12 arg1, s_CollisionState* state);
//
    //q3_12 func_8006C248(s32 packedDir, q3_12 arg1, q3_12 deltaX, q7_8 deltaZ, q7_8 arg4);
//
    //bool func_8006C3D4(s_CollisionState* state, s_IpdCollisionData* collData, s32 subcellIdx);
//
    //void func_8006C45C(s_CollisionState* state);
//
    //void func_8006C794(s_CollisionState* state, s32 arg1, q23_8 dist);
//
    //void func_8006C838(s_CollisionState* state, s_IpdCollisionData* collData);
//
    //// Accounts for slopes in ground height?
    //void func_8006CA18(s_CollisionState* state, s_IpdCollisionData* collData, s_IpdCollSubcellRange* subcellRanges);
//
    //// TODO: Computes alpha related to the ground slope?
    //q3_12 Collision_OffsetAlphaGet(s_CollisionState* state);
//
    //// Gets sloped ground height, not the base height.
    //q23_8 Ipd_GroundHeightGet(q23_8 posX, q23_8 posZ, const s_CollisionState* state);
//
    //void func_8006CC9C(s_CollisionState* state);
//
    //void func_8006CF18(s_CollisionState* state, s_func_8006CF18* arg1, s32 idx);
//
    //void func_8006D01C(VECTOR3* arg0, VECTOR3* offset, q3_12 offsetAlpha, s_CollisionState* state);
//
    //void func_8006D2B4(VECTOR3* arg0, s_CollisionState_44* arg1);
//
    //void func_8006D600(VECTOR3* pos, q19_12 angle, q19_12 rotX, q19_12 rotY, q19_12 dist);
//
    //void func_8006D774(s_CollisionState* state, VECTOR3* arg1, VECTOR3* arg2);
//
    ///** Offsets are Q23.8. */
    //void func_8006D7EC(s_CollisionCharaState* charaState, SVECTOR* offset0, SVECTOR* offset1);

    // ========================================
    // @split? Harry weapon anim infos.
    // ========================================
}
