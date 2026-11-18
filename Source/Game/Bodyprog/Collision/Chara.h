#pragma once

#include "Game/Bodyprog/Chara/Chara.h"

namespace Silent::Game
{
    struct s_Keyframe;
    struct s_SubCharacter;

    /** @brief Character collision cylinder. */
    struct s_CollisionCylinder
    {
        VECTOR3 position; /** Q19.12 */
        q3_12   bottom;
        q3_12   top;
        q3_12   radius;
        s8      collisionState; /** `e_CharaCollisionState` */
    };

    /** @brief Character collision state. TODO: Name clash with `e_CharaCollisionState`. */
    typedef struct _CollisionCharaState
    {
        s32        collisionState; /** `e_CharaCollisionState` */
        bool       field_4;        // `allowDefaultCeilHeight`? Set when the character collision being checked is any of the ones in
                                   // `Collision_CharaCollisionSetup`.
        q19_12     distance;
        SVECTOR    offset; /** Q23.8 | Character movement offset. */
        DVECTOR_XZ direction;
        q23_8      positionFromX;
        q23_8      positionFromZ;
        q23_8      positionToX;
        q23_8      positionToZ;
        q7_8       radius;
        q7_8       top;
        q7_8       bottom;
    } s_CollisionCharaState;

    // TODO: Rough name.
    typedef struct _CharaCollisionDeltas
    {
        DVECTOR_XZ charaVertDiff;
        DVECTOR_XZ charaMoveVertDiff;
        u32        subChunkTransDir;   /** `e_SubChunkTransitionDirection` */
        q23_8      radiusCollDiffDist; // Radius?
        q7_8       collDiffDist;
        s8         __pad_12[2];
        DVECTOR_XZ charaMoveOffset;
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
