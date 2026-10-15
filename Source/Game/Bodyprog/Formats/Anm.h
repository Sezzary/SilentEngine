#pragma once

namespace Silent::Game
{
    // See: https://github.com/laura-a-n-n/silent-hill-museum/blob/main/ksy/sh1anm.ksy
    struct s_AnmBindPose
    {
        s8 parentBone;
        s8 rotationDataIdx;
        s8 translationDataIdx;
        s8 translationInitial[3];
    };

    /** @brief ANM file header. */
    struct s_AnmHeader
    {
        u16           dataOffset;
        u8            rotationBoneCount;
        u8            translationBoneCount;
        u16           keyframeDataSize; /** Size per keyframe. `(rotationBoneCount * 9) + (translationBoneCount * 3)`? */
        u8            boneCount;        /** Size of `bindPoses`. */
        s8            __pad_7;
        u32           activeBones; /** Bit field of bones to update. */
        u32           fileSize;
        u16           keyframeCount;
        u8            scaleLog2;
        u8            rootYOffset;
        s_AnmBindPose bindPoses[0];
    };
}
