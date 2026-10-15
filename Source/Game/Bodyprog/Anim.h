#pragma once

#include "Psx.h"

namespace Silent::Game
{
    struct s_AnmHeader;
    struct s_Model;

    /** @brief Character animation flags. */
    enum e_AnimFlags
    {
        AnimFlag_None     = 0,
        AnimFlag_Unlocked = 1 << 0,
        AnimFlag_Visible  = 1 << 1
    };

    /** @brief Animation playback states. Returned by `Chara_AnimPlaybackStateGet`. */
    enum e_AnimPlaybackState
    {
        AnimPlaybackState_Blend   = -2,
        AnimPlaybackState_Invalid = -1, // Unsure.
        AnimPlaybackState_Active  = 0,
        AnimPlaybackState_End     = 1
    };

    /** @brief Constant character animation info passed to `Anim_Update` functions. */
    struct s_AnimInfo
    {
        void (*playbackFunc)(s_Model* model, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords, s_AnimInfo* animInfo);
        int  status;                      /** Packed anim status. Init base? See `s_ModelAnimData::status`. */
        bool hasVariableDuration;         /** Use `duration.variableFunc`: `true`, Use `duration.constant`: `false`. */
        int  linkStatus;                  /** Packed anim status link target. See `s_ModelAnim::status`. */
        union
        {
            q19_12 constant;              /** Constant duration at 30 FPS. */
            q19_12 (*variableFunc)(void); /** Variable duration at 30 FPS via a function. Allows animations to be sped up or slowed down. */
        } duration;
        int startKeyframeIdx;             /** Start keyframe index. Sometimes `NO_VALUE`, unknown why. */
        int endKeyframeIdx;               /** End keyframe index. */
    };

    /** @brief Packs an animation status containing an animation index and active flag.
     * `isActive` is `false` in the blend phase and `true` in the playback phase.
     *
     * @param animIdx Animation index.
     * @param isActive Active status.
     * @return Packed animation status containing the animation index and active flag.
     */
    constexpr int ANIM_STATUS(int animIdx, bool isActive)
    {
        return (animIdx << 1) | (isActive ? 1 : 0);
    }

    /** @brief Retrieves the animation index from a packed animation status.
     *
     * @param animStatus Packed animation status containing an animation index and active flag.
     * @return Anim index.
     */
    constexpr int ANIM_STATUS_IDX_GET(int animStatus)
    {
        return animStatus >> 1;
    }

    /** @brief Checks if an animation is active.
     *
     * @param animStatus Packed animation status containing an animation index and active flag.
     * @return `true` if active, `false` otherwise.
     */
    constexpr bool ANIM_STATUS_IS_ACTIVE(int animStatus)
    {
        return animStatus & (1 << 0);
    }

    /** @brief Checks if an animation time is within the keyframe index range `[low, high]`.
     *
     * @param animTime Animation time to check.
     * @param low Low keyframe index.
     * @param high High keyframe index.
     * @return `true` if the animation time is within the keyframe index range, `false` otherwise.
     */
    constexpr bool ANIM_TIME_RANGE_CHECK(q19_12 animTime, int low, int high)
    {
        return FP_FROM(animTime, Q12_SHIFT) >= low &&
               FP_FROM(animTime, Q12_SHIFT) <= high;
    }

    /** @brief Gets the relative keyframe index of an animation time with a base offset.
     *
     * @param animTime Animation time.
     * @param baseOffset Base keyframe index offset.
     * @return Relative keyframe index.
     */
    constexpr int ANIM_TIME_REL_KEYFRAME_IDX_GET(q19_12 animTime, int baseOffset)
    {
        return FP_FROM(animTime, Q12_SHIFT) - baseOffset;
    }
}
