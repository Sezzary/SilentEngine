#pragma once

// TODO: These structs are conceptually different from the ones in `bodyprog/collision/chara.h`. Need to distinguish
// them more clearly.

namespace Silent::Game
{
    /** @brief Character collision states. */
    enum e_CharaCollisionState
    {
        CharaCollisionState_Ignore  = 0,
        CharaCollisionState_Player  = 1,
        CharaCollisionState_2       = 2,
        CharaCollisionState_Npc     = 3,
        CharaCollisionState_4       = 4,
        CharaCollisionState_Default = 5
    };

    /** @brief Character collision box. */
    struct s_CharaBox
    {
        q3_12 topExtent;
        q3_12 bottomExtent;
        q3_12 height;
        q3_12 offsetY;
        // TODO: Maybe nested inner and outer XZ extents. See `s_CharaCylinder`.
        q3_12 field_8; // Outer XZ extent? Always nagative, might be unsigned? `s_CharaCylinder::outerRadius` is set to this.
        q3_12 field_A; // Innter XZ extent? `s_CharaCylinder::innerRadius` is set to this.
    };

    // TODO: Too similar to `s_CharaCollisionCylinder`. Maybe more like character detection ranges?
    /** @brief Character collision cylinder. */
    struct s_CharaCylinder
    {
        q3_12 outerRadius;
        q3_12 innerRadius;
    };

    /** @brief Character shape offsets for `s_CharaBox` and `s_CharaCylinder`. */
    struct s_CharaShapeOffsets
    {
        DVECTOR_XZ box;
        DVECTOR_XZ cylinder;
    };

    /** @brief Character keyframe collision info. */
    struct s_CharaKeyframe
    {
        s_CharaBox          box;
        s_CharaShapeOffsets shapeOffsets;
    };

    /** @brief Recorded character-to-character collision.
     * Similar to `s_CharaCollisionCylinder` but with extra `passStamp` field.
     */
    struct s_CharaCollisionRecord
    {
        VECTOR3 position;     /** Q19.12 */
        q3_12   bottomExtent;
        q3_12   topExtent;
        q3_12   radius;
        s8      state;        /** `e_CharaCollisionState` */
        u8      passStamp;    /** Tracks visited characters per query pass. */
    };

    /** @brief Character collision info for the active tick. */
    struct s_CharaCollision
    {
        s_CharaBox              box;
        s_CharaCylinder         cylinder;
        s_CharaShapeOffsets     shapeOffsets;         // Translation data?
        u8                      passStamp;            /** Same usage as `s_IpdCollisionData::shapePassStampBuffer`. */
        s8                      state            : 4; /** `e_CharaCollisionState` */
        u8                      charaRecordCount : 4; /** `charaRecords` array size. */
        s_CharaCollisionRecord* charaRecords;
    };
}
