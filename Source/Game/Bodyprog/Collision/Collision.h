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
    constexpr float  DEFAULT_FLOOR_HEIGHT   = 30.0f;
    constexpr float  DEFAULT_CEILING_HEIGHT = -16.0f;
    constexpr q19_12 INTERSECTION_BUFFER    = Q12(0.1f);

    /** @brief Collision types. */
    typedef enum _CollisionType
    {
        CollisionType_None  = 0,
        CollisionType_Wall  = 1,
        CollisionType_Floor = 2 // TODO: Unsure.
    } e_CollisionType;

    /** @brief Collision push types. */
    typedef enum _CollisionPushType
    {
        CollisionPushType_Hard  = 0, /** Hard inner shape proximity. */
        CollisionPushType_Soft  = 1, /** Soft outer shape proximity. */
        CollisionPushType_Chara = 2  /** Character-to-character cylinder. */
    } s_CollisionPushType;

    // TODO: "Subchunk" term may be misused here, need to check.
    typedef enum _SubChunkTransitionDirection
    {
        SubChunkTransitionDirection_None = 0,
        SubChunkTransitionDirection_Z    = 1,
        SubChunkTransitionDirection_X    = 2
    } e_SubChunkTransitionDirection;

    /** @brief Collision point surface data. */
    typedef struct _CollisionSurface
    {
        /* 0x0 */ q19_12 floorHeight;
        /* 0x4 */ q7_8   slopeGradientX;
        /* 0x6 */ q7_8   slopeGradientZ;
        /* 0x8 */ s8     floorType; /** `e_FloorType` */
                // 3 bytes of padding.
    } s_CollisionSurface;

    /** @brief Collision point data. */
    typedef struct _CollisionPoint
    {
        /* 0x0  */ VECTOR3            position;   /** Q19.12 */
        /* 0xC  */ s_CollisionSurface surface;
        /* 0x18 */ s32                floorType; /** `e_FloorType` */
    } s_CollisionPoint;

    /** @brief Collision point probe result. */
    typedef struct _CollisionResult
    {
        /* 0x0  */ VECTOR3            offset; /** Q19.12 */
        /* 0xC  */ s_CollisionSurface surface;
        /* 0x18 */ q19_12             ceilingHeight;
    } s_CollisionResult;

    /** @brief Cached collision surface. */
    typedef struct _CollisionCachedSurface
    {
        /* 0x0 */ u8    subChunkTransDir; /**`e_SubChunkTransitionDirection` */
        /* 0x1 */ u8    surfaceIdx;       // Index for `s_IpdCollisionData::polygonSurfaces`.
        /* 0x2 */ q7_8  collDiffDist;
        /* 0x4 */ q23_8 radiusCollDiffDist;
    } s_CollisionCachedSurface;

    /** @brief Collision push constraint. */
    typedef struct _CollisionPushConstraint
    {
        /* 0x0 */ s8    shapeCount;
        /* 0x2 */ q3_12 angleMin;
        /* 0x4 */ q3_12 angleMax;
    } s_CollisionPushConstraint;

    /** @brief CollisionObstacleContext */
    typedef struct _CollisionObstacleContext
    {
        /* 0x0  */ s_CollisionPushConstraint hardPushConstraint;
        /* 0x6  */ q7_8                      hardIntersectDepth;
        /* 0x8  */ s_CollisionPushConstraint softPushConstraint;
        /* 0xE  */ s8                        __pad_E[2];
        /* 0x10 */ s8*                       shapePassStamps[8];
        /* 0x30 */ s_CollisionPushConstraint charaPushConstraint;
        /* 0x36 */ q7_8                      charaIntersectDepth;
    } s_CollisionObstacleContext;

    /** @brief Copied collision polygon or cylinder attributes. */
    typedef union _CollisionShapeAttribs
    {
                struct
                {
                    /* 0x0 */ q7_8 radius;
        /* 0x0 */ } cylinder;
                struct
                {
                    /* 0x0 */ u8       surfaceIdx0;
                    /* 0x1 */ u8       surfaceIdx1;
                    /* 0x2 */ u8       surfaceCacheIdx0;
                    /* 0x3 */ u8       surfaceCacheIdx1;
                    /* 0x4 */ u8       disableSurfaceHeight0; /** `bool` */
                    /* 0x5 */ u8       disableSurfaceHeight1; /** `bool` */
                    /* 0x8 */ SVECTOR3 polygonSplitVertex0;   /** Q7.8 | Vertex from `s_IpdCollisionData::polygonSplitVertices`. */
                    /* 0xE */ SVECTOR3 polygonSplitVertex1;   /** Q7.8 | Vertex from `s_IpdCollisionData::polygonSplitVertices`. */
        /* 0x0 */ } polygon;
    } s_CollisionShapeAttribs;

    /** @brief Collision shape point data. TODO: Come up with better naame. */
    typedef struct _CollisionShapePoint
    {
        /* 0x0  */ s_IpdCollisionData*     ipdCollisionData;
        /* 0x4  */ u8                      shapeIdx; // `s_IpdCollisionData::polygons` index.
        /* 0x5  */ u8                      disableHeight;
        /* 0x6  */ SVECTOR3                offset; // Q7.8 | Probe position inside cell/cell? Wrong struct? Y is maybe Z.
        /* 0xC  */ s_CollisionShapeAttribs shapeAttribs;
        /* 0x20 */ s_CharaCollisionDeltas  charaDeltas;
    } s_CollisionShapePoint;

    typedef struct _CollisionState
    {
        /* 0x0+0  */ u8                         offsetX       : 8;  /** `bool` | Related to `Collision_ShapePassReset`. Maybe name `hasCellChecks`? */
        /* 0x0+8  */ s8                         isCharaMoving : 1;  /** `bool` */
        /* 0x0+9  */ s8                         field_0_9     : 1;  /** `bool` */
        /* 0x0+10 */ s8                         field_0_10    : 1;  /** `bool` */
        /* 0x2    */ u16                        triggerFlags  : 16; /** `e_CollisionTriggerFlags` */
        /* 0x4    */ s_CollisionCharaState      charaState;
        /* 0x34   */ s32                        field_34;
        /* 0x38   */ s16                        field_38; // `distanceAlpha` for something.
        /* 0x3A   */ q11_4                      field_3A;
        /* 0x3C   */ q7_8                       field_3C; // X?
        /* 0x3E   */ q7_8                       field_3E; // Z?
        /* 0x40   */ u8*                        passStamp; /** Tracks visited shapes/characters per query pass. */
        /* 0x44   */ s_CollisionObstacleContext obstacleContext;
        /* 0x7C   */ q23_8                      slopedFloorHeight; // TODO: How is `floorHeight` different?
        /* 0x80   */ q23_8                      charaCellOffsetX; // } Character position in cell.
        /* 0x84   */ q23_8                      charaCellOffsetZ; // } 
        /* 0x88   */ q23_8                      slopeGradientX;
        /* 0x8C   */ q23_8                      slopeGradientZ;
        /* 0x90   */ bool                       disableHeight; /** `bool` | TODO: Does disabled height indicate a wall or pit? */
        /* 0x94   */ s32                        floorType;      /** `e_FloorType` */

                    union
                    {
                        DVECTOR_XZ offset; /** Q23.8 */
                        q23_8      vx;
        /* 0x98   */ } charaPositionFrom;
                    union
                    {
                        DVECTOR_XZ offset; /** Q23.8 */
                        q23_8      vz;
        /* 0x9C   */ } charaPositionTo;
                    union
                    {
                        struct
                        {
                            /* 0x0 */ u8                       closestXCellIdx;
                            /* 0x1 */ u8                       closestZCellIdx;
                            /* 0x2 */ u8                       cellSpanX;
                            /* 0x3 */ u8                       cellSpanZ;
                            /* 0x4 */ s_IpdShapeRange*         shapeRangePair;
                            /* 0x8 */ s_CollisionCachedSurface surfaceCache[4];
                        } s_0;
                        struct
                        {
                            /* 0x0 */ q7_8 bottom; // Set to absolute character bottom height.
                            /* 0x2 */ q7_8 top;    // Set to absolute character top height.
                            /* 0x4 */ q7_8 radius;
                            /* 0x6 */ u8   collisionState; /** `e_CharaCollisionState` */
                            /* 0x8 */ u8*  passStamp; /** Tracks visited characters per query pass. */
                        } chara;
        /* 0xA0   */ } field_A0; // Floor info? Cell info? Cell state?

        /* 0xC8   */ u8                    shapeIdx; /** Secondary. Used only if detecting a cell after not finding one. */
        /* 0xCA   */ q7_8                  floorHeight; // TODO: Base floor height with slope unaccounted for?
        /* 0xCC   */ s_CollisionShapePoint point;
        // TODO: Maybe incomplete. Maybe not, added the final padding based on `Collision_SurfaceGet`.
    } s_CollisionState;

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
     * @param floorHeight Floorh height.
     */
    void Collision_DefaultResultSet(s_CollisionResult* collResult, q19_12 offsetX, q19_12 offsetY, q19_12 offsetZ, q19_12 floorHeight);

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
