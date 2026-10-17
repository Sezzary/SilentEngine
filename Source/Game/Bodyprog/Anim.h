#pragma once

namespace Silent::Assets{ struct Asset; }

using namespace Silent::Assets;

namespace Silent::Game
{
    struct s_Model;
    struct s_ModelAnim;
    struct s_SubCharacter;

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

    /** @brief Bone transform node. */
    struct BoneTransform
    {
        int        ParentIdx   = NO_VALUE;
        Vector3    Translation = Vector3::Zero;
        Quaternion Rotation    = Quaternion::Identity;
    };

    /** @brief Constant character animation info passed to `Anim_Update` functions. */
    struct s_AnimInfo
    {
        void (*playbackFunc)(s_Model& model, const Asset& anmAsset, const s_AnimInfo& animInfo);
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

    void Anim_BonesInit(const Asset& anmAsset, std::vector<BoneTransform>& transforms);

    void Anim_BonesUpdate(const Asset& anmAsset, std::vector<BoneTransform>& transforms,
                          int keyframe0, int keyframe1, q19_12 alpha);

    /** @brief Gets the active animation info of a model, accounting for overflow map animations.
     *
     * @param anim Model animation data.
     */
    const s_AnimInfo& Anim_InfoGet(const s_ModelAnim& anim);

    /** @brief Updates a character's animation using the callback function of its active animation.
     *
     * @param chara Character to update.
     * @param anmAsset Character animation asset.
     * @param animInfo Character animation info.
     */
    void Anim_CharaUpdate(s_SubCharacter& chara, const Asset& anmAsset, const s_AnimInfo& animInfo);

    /** @brief Gets the duration of an animation. A duration can be constant or variable.
     *
     * @param animInfo Animation info from which to get the duration.
     * @return Animation duration.
     */
    q19_12 Anim_DurationGet(const s_AnimInfo& animInfo);

    /** @brief Plays an animation once and clamps to the end keyframe, triggering a status transition to the linked
     * animation upon completion.
     *
     * @note Used for one-shot actions such as stopping and attacking.
     *
     * @param model Character model to animate.
     * @param anmAsset Character animation asset.
     * @param animInfo Active character animation info.
     */
    void Anim_PlayOnce(s_Model& model, const Asset& anmAsset, const s_AnimInfo& animInfo);

    /** @brief Loops an animation continuously, wrapping the time around to the start and end keyframes without
     * triggering a status transition.
     *
     * @note Used for looped actions such as running and walking.
     *
     * @param model Character model to animate.
     * @param anmAsset Character animation asset.
     * @param animInfo Character animation info.
     */
    void Anim_PlayLoop(s_Model& model, const Asset& anmAsset, const s_AnimInfo& animInfo);

    /** @brief Linearly interpolates between the current pose into a new target animation, triggering a status transition
     * to the linked animation when complete.
     *
     * @note Used as the entry transition for almost every new animation status.
     *
     * @param model Character model to animate.
     * @param anmAsset Character animation asset.
     * @param animInfo Character animation info.
     */
    void Anim_BlendLinear(s_Model& model, const Asset& anmAsset, const s_AnimInfo& animInfo);

    /** @brief Smoothly blends between the current pose into a new target animation using a sine-based ease-out
     * without triggering a status transition.
     *
     * @unused?
     *
     * @param model Character model to animate.
     * @param anmAsset Character animation asset.
     * @param animInfo Character animation info.
     */
    void Anim_BlendEaseOut(s_Model& model, const Asset& anmAsset, const s_AnimInfo& animInfo);
}
