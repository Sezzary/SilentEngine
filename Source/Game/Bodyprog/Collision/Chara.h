#pragma once

#include "Game/Bodyprog/Chara/Chara.h"

namespace Silent::Game
{
    /** @brief Character collision cylinder. */
    struct s_CollisionCylinder
    {
        /* 0x0  */ VECTOR3 position; /** Q19.12 */
        /* 0xC  */ q3_12   bottom;
        /* 0xE  */ q3_12   top;
        /* 0x10 */ q3_12   radius;
        /* 0x14 */ s8      collisionState; /** `e_CharaCollisionState` */
    };

    /** @brief Character collision state. TODO: Name clash with `e_CharaCollisionState`. */
    typedef struct _CollisionCharaState
    {
        /* 0x0  */ s32        collisionState; /** `e_CharaCollisionState` */
        /* 0x4  */ bool       field_4;        // `allowDefaultCeilHeight`? Set when the character collision being checked is any of the ones in
                                              // `Collision_CharaCollisionSetup`.
        /* 0x8  */ q19_12     distance;
        /* 0xC  */ SVECTOR    offset; /** Q23.8 | Character movement offset. */
        /* 0x14 */ DVECTOR_XZ direction;
        /* 0x18 */ q23_8      positionFromX;
        /* 0x1C */ q23_8      positionFromZ;
        /* 0x20 */ q23_8      positionToX;
        /* 0x24 */ q23_8      positionToZ;
        /* 0x28 */ q7_8       radius;
        /* 0x2A */ q7_8       top;
        /* 0x2C */ q7_8       bottom;
    } s_CollisionCharaState;

    // TODO: Rough name.
    typedef struct _CharaCollisionDeltas
    {
        /* 0x0  */ DVECTOR_XZ charaVertDiff;
        /* 0x4  */ DVECTOR_XZ charaMoveVertDiff;
        /* 0x8  */ u32        subChunkTransDir;   /** `e_SubChunkTransitionDirection` */
        /* 0xC  */ q23_8      radiusCollDiffDist; // Radius?
        /* 0x10 */ q7_8       collDiffDist;
        /* 0x12 */ s8         __pad_12[2];
        /* 0x14 */ DVECTOR_XZ charaMoveOffset;
    } s_CharaCollisionDeltas;

    bool func_80070320(void);

    /** TODO: Return type uncertain. */
    q19_12 func_80070360(s_SubCharacter* chara, q19_12 someDist, q3_12 arg2);

    /** @brief Sets a character's interpolated collision shapes for the active animation frame.
     *
     * @param chara Character to update.
     * @param keyframe0 First keyframe.
     * @param keyframe1 Second keyframe.
     */
    void Collision_CharaCollisionSet(s_SubCharacter* chara, s_Keyframe* keyframe0, s_Keyframe* keyframe1);

    /** @brief Sets the scale of a character's model bone.
     *
     * @param boneCoords Character model bone coords.
     * @param boneIdx Index of the model bone to scale.
     * @param scaleX X scale.
     * @param scaleY Y scale.
     * @param scaleZ Z scale.
     */
    void Chara_ModelBoneScaleSet(GsCOORDINATE2* boneCoords, s32 boneIdx, q19_12 scaleX, q19_12 scaleY, q19_12 scaleZ);
}
