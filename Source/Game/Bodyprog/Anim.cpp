#include "Framework.h"
#include "Game/Bodyprog/Anim.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Game/Bodyprog/Model.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Game.h"

using namespace Silent::Assets;

namespace Silent::Game
{
    /** @brief Computes the timestep of the target animation for the current tick.
     *
     * @param model Character model.
     * @param animInfo Animation info.
     * @return Animation timestep for the current tick.
     */
    static inline q19_12 Anim_TimestepGet(const s_Model& model, const s_AnimInfo& animInfo)
    {
        // Check if animation is unlocked.
        if (model.anim.flags & AnimFlag_Unlocked)
        {
            q19_12 duration = Anim_DurationGet(animInfo);
            return Q12_MULT_PRECISE(duration, g_DeltaTime);
        }

        return Q12(0.0f);
    }

    void Anim_BonesInit(const Asset& anmAsset, std::vector<BoneTransform>& transforms)
    {
        const auto anmData = anmAsset.GetData<AnmAsset>();

        // @todo Maybe resize `transforms` here? Depends if they're already initialised elsewhere, will have to do that.

        // Run through bones.
        for (int i = 1; i < anmData->Bones.size(); i++)
        {
            auto& bone      = anmData->Bones[i];
            auto& transform = transforms[i];

            transform.ParentIdx = bone.ParentBoneIdx;

            // If no translation for this bone, copy over `BindTranslation`.
            if (bone.TranslationIdx < 0)
            {
                // @todo Check conversion to float in loader.
                transform.Translation = bone.BindTranslation;
            }

            // If no rotation for this bone, create identity matrix.
            if (bone.RotationIdx < 0)
            {
                transform.Rotation = EulerAngles::Identity;
            }
        }
    }

    void Anim_BonesUpdate(const Asset& anmAsset, std::vector<BoneTransform>& transforms,
                          int keyframe0, int keyframe1, q19_12 alpha)
    {
        const auto anmData = anmAsset.GetData<AnmAsset>();

        const auto& keyframeData0 = anmData->Keyframes[keyframe0];
        const auto& keyframeData1 = anmData->Keyframes[keyframe1];

        bool isPlayer = &transforms == &g_SysWork.playerWork.player.model.anim.BoneTransforms;

        // For player, use inverted mask of `extra.disabledAnimBones` to facilitate masking of upper and lower body.
        int activeBoneIdxs = 0;
        if (isPlayer)
        {
            activeBoneIdxs = ~g_SysWork.playerWork.extra.disabledAnimBones;
        }
        else
        {
            // @todo
            //activeBoneIdxs = anmHdr->activeBones;
        }

        // Run through bones.
        for (int i = 1; i < anmData->Bones.size(); i++)
        {
            const auto& bone      = anmData->Bones[i];
            auto&       transform = transforms[i];

            // Process bones marked as active.
            if (activeBoneIdxs & (1 << i))
            {
                if (bone.TranslationIdx >= 0)
                {
                    const auto& translation0 = keyframeData0.BoneTranslations[bone.TranslationIdx];
                    const auto& translation1 = keyframeData1.BoneTranslations[bone.TranslationIdx];

                    transform.Translation = Vector3::Lerp(translation0, translation1, Q12_TO_FLT(alpha));
                    if (bone.TranslationIdx == 0)
                    {
                        // @todo Is this right? Previously used `anmHdr->rootYOffset`, which doesn't exist in parsed data.
                        transform.Translation.y -= anmData->Bones[0].BindTranslation.y; // TODO: Not sure of purpose of this yet.
                    }
                }

                if (bone.RotationIdx >= 0)
                {
                    const auto& rot0 = keyframeData0.BoneRotationMats[bone.RotationIdx];
                    const auto& rot1 = keyframeData1.BoneRotationMats[bone.RotationIdx];

                    // @todo Using matrix here.
                    transform.Rotation = EulerAngles::Lerp(rot0.ToEulerAngles(), rot1.ToEulerAngles(), Q12_TO_FLT(alpha));
                }
            }
        }

        // Copy player hips translation to torso.
        if (isPlayer)
        {
            auto& player = g_SysWork.playerWork.player;

            auto& torsoTranslation = player.model.anim.BoneTransforms[HarryBone_Torso].Translation;
            auto& hipTranslation   = player.model.anim.BoneTransforms[HarryBone_Hips].Translation;
            torsoTranslation       = hipTranslation;
        }
    }

    const s_AnimInfo& Anim_InfoGet(const s_ModelAnim& anim)
    {
        auto* animInfos          = anim.baseAnimInfos;
        auto* mapAnimInfos       = anim.mapAnimInfos;
        int   animStatus         = anim.status;
        int   mapAnimStatusStart = anim.mapAnimStatusStart;

        if (mapAnimInfos != nullptr && animStatus >= mapAnimStatusStart)
        {
            animInfos  = mapAnimInfos;
            animInfos -= mapAnimStatusStart;
        }

        return animInfos[animStatus];
    }

    void Anim_CharaUpdate(s_SubCharacter& chara, const Asset& anmAsset, const s_AnimInfo& animInfo)
    {
        animInfo.playbackFunc(chara.model, anmAsset, Anim_InfoGet(chara.model.anim));
    }

    q19_12 Anim_DurationGet(const s_AnimInfo& animInfo)
    {
        if (!animInfo.hasVariableDuration)
        {
            return animInfo.duration.constant;
        }

        return animInfo.duration.variableFunc();
    }

    void Anim_PlayOnce(s_Model& model, const Asset& anmAsset, const s_AnimInfo& animInfo)
    {
        const auto anmData = anmAsset.GetData<AnmAsset>();

        bool setNewAnimStatus = false;

        // Get timestep.
        q19_12 timestep = Anim_TimestepGet(model, animInfo);

        // Compute new time and keyframe index.
        q19_12 newTime        = model.anim.time;
        int    newKeyframeIdx = FP_FROM(newTime, Q12_SHIFT);
        if (timestep != Q12(0.0f))
        {
            newTime += timestep;

            // Clamp new time to valid keyframe range.
            q19_12 endTime = Q12(animInfo.endKeyframeIdx);
            if (newTime >= endTime)
            {
                newTime          = endTime;
                setNewAnimStatus = true;
            }
            else
            {
                q19_12 startTime = Q12(animInfo.startKeyframeIdx);
                if (newTime <= startTime)
                {
                    newTime          = startTime;
                    setNewAnimStatus = true;
                }
            }

            newKeyframeIdx = FP_FROM(newTime, Q12_SHIFT);
        }

        // Update skeleton.
        q19_12 alpha = Q12_FRACT(newTime);
        if (model.anim.flags & (AnimFlag_Unlocked | AnimFlag_Visible))
        {
            Anim_BonesUpdate(anmAsset, model.anim.BoneTransforms, newKeyframeIdx, newKeyframeIdx + 1, alpha);
        }

        // Update frame data.
        model.anim.time        = newTime;
        model.anim.keyframeIdx = newKeyframeIdx;
        model.anim.alpha       = Q12(0.0f);

        // Link to new anim status.
        if (setNewAnimStatus)
        {
            model.anim.status = animInfo.linkStatus;
        }
    }

    void Anim_PlayLoop(s_Model& model, const Asset& anmAsset, const s_AnimInfo& animInfo)
    {
        int startKeyframeIdx     = animInfo.startKeyframeIdx;
        int endKeyframeIdx       = animInfo.endKeyframeIdx;
        int nextStartKeyframeIdx = endKeyframeIdx + 1;
        int keyframeCount        = nextStartKeyframeIdx - startKeyframeIdx;

        q19_12 startTime     = Q12(startKeyframeIdx);
        q19_12 nextStartTime = Q12(nextStartKeyframeIdx);
        q19_12 duration      = Q12(keyframeCount);

        // Get timestep.
        q19_12 timestep = Anim_TimestepGet(model, animInfo);

        // Wrap new time to valid keyframe range.
        q19_12 newTime = model.anim.time + timestep;
        while (newTime < startTime)
        {
            newTime += duration;
        }
        while (newTime >= nextStartTime)
        {
            newTime -= duration;
        }

        // Compute new keyframe 1. Wrap to start to facilitate loop.
        int newKeyframeIdx0 = FP_FROM(newTime, Q12_SHIFT);
        int newKeyframeIdx1 = newKeyframeIdx0 + 1;
        if (newKeyframeIdx1 == nextStartKeyframeIdx)
        {
            newKeyframeIdx1 = startKeyframeIdx;
        }

        // Update skeleton.
        q19_12 alpha = Q12_FRACT(newTime);
        if (model.anim.flags & (AnimFlag_Unlocked | AnimFlag_Visible))
        {
            Anim_BonesUpdate(anmAsset, model.anim.BoneTransforms, newKeyframeIdx0, newKeyframeIdx1, alpha);
        }

        // Update frame data.
        model.anim.time        = newTime;
        model.anim.keyframeIdx = newKeyframeIdx0;
        model.anim.alpha       = Q12(0.0f);
    }

    void Anim_BlendLinear(s_Model& model, const Asset& anmAsset, const s_AnimInfo& animInfo)
    {
        bool setNewAnimStatus = false;
        int startKeyframeIdx  = animInfo.startKeyframeIdx;
        int endKeyframeIdx    = animInfo.endKeyframeIdx;

        // If no start keyframe exists, default to active keyframe.
        if (startKeyframeIdx == NO_VALUE)
        {
            startKeyframeIdx = model.anim.keyframeIdx;
        }

        // Get timestep.
        q19_12 timestep = Anim_TimestepGet(model, animInfo);

        // Update time to start or end keyframe, whichever is closest.
        q19_12 alpha = model.anim.alpha + timestep;
        if (alpha >= Q12(0.5f))
        {
            model.anim.time = Q12(endKeyframeIdx);
        }
        else
        {
            model.anim.time = Q12(startKeyframeIdx);
        }

        // Update frame data.
        if (alpha >= Q12(1.0f))
        {
            startKeyframeIdx       = endKeyframeIdx;
            model.anim.keyframeIdx = endKeyframeIdx;

            alpha            = Q12(0.0f);
            setNewAnimStatus = true;
        }

        // Update skeleton.
        if (model.anim.flags & (AnimFlag_Unlocked | AnimFlag_Visible))
        {
            Anim_BonesUpdate(anmAsset, model.anim.BoneTransforms, startKeyframeIdx, endKeyframeIdx, alpha);
        }

        // Update alpha.
        model.anim.alpha = alpha;

        // Link to new anim status.
        if (setNewAnimStatus)
        {
            model.anim.status = animInfo.linkStatus;
        }
    }

    void Anim_BlendEaseOut(s_Model& model, const Asset& anmAsset, const s_AnimInfo& animInfo)
    {
        int startKeyframeIdx = animInfo.startKeyframeIdx;
        int endKeyframeIdx   = animInfo.endKeyframeIdx;

        q19_12 timestep = Anim_TimestepGet(model, animInfo);

        // Update alpha.
        q19_12 newAlpha  = model.anim.alpha;
        q19_12 alpha     = newAlpha + timestep;
        model.anim.alpha = alpha;

        // Compute ease-out alpha.
        q19_12 sinVal = Math_Sin((alpha / 2) - Q12(0.25f));
        newAlpha      = (sinVal / 2) + Q12(0.5f);

        // Update time to start or end keyframe, whichever is closest.
        q19_12 newTime = Q12(0.0f);
        if (newAlpha >= Q12(0.5f))
        {
            newTime = Q12(startKeyframeIdx);
        }
        else
        {
            newTime = Q12(endKeyframeIdx);
        }
        alpha = newAlpha;

        // Update time.
        model.anim.time = newTime;

        // Update skeleton.
        if (model.anim.flags & (AnimFlag_Unlocked | AnimFlag_Visible))
        {
            Anim_BonesUpdate(anmAsset, model.anim.BoneTransforms, startKeyframeIdx, endKeyframeIdx, alpha);
        }

        // Update active keyframe.
        model.anim.keyframeIdx = FP_FROM(newTime, Q12_SHIFT);
    }
}
