#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Bodyprog.h"
#include "Game/Bodyprog/Dms.h"

//#include "Game/Bodyprog/ItemScreens.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Main/FsQueue.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"

using namespace Silent::Assets;

namespace Silent::Game
{
    void Dms_CharacterTransformGet(VECTOR3* pos, SVECTOR3* rot, const std::string& charaName, q19_12 time, const Asset& dmsAsset)
    {
        int charaEntryIdx = Dms_CharacterGetIdxByName(charaName, dmsAsset);
        if (charaEntryIdx == NO_VALUE)
        {
            // Character not found in DMS.
            Math_Vector3Zero(pos);
            Math_SVectorZero(rot);

            Debug::Msg((charaName + " doesn't exist in DMS.").c_str());
        }
        else
        {
            Dms_CharacterTransformGetByIdx(pos, rot, charaEntryIdx, time, dmsAsset);
        }
    }

    int Dms_CharacterGetIdxByName(const std::string& charaName, const Asset& dmsAsset)
    {
        const auto dmsData = dmsAsset.GetData<DmsAsset>();

        // Find matching name.
        for (int i = 0; i < dmsData->CharacterEntries.size(); i++)
        {
            const auto& charaEntry = dmsData->CharacterEntries[i];
            if (charaName == charaEntry.Name)
            {
                return i;
            }
        }

        return NO_VALUE;
    }

    void Dms_CharacterTransformGetByIdx(VECTOR3* pos, SVECTOR3* rot, int charaIdx, q19_12 time, const Asset& dmsAsset)
    {
        const auto  dmsData    = dmsAsset.GetData<DmsAsset>();
        const auto& charaEntry = dmsData->CharacterEntries[charaIdx];

        // Get keyframe interpolation data.
        int    prevKeyframeIdx = 0;
        int    nextKeyframeIdx = 0;
        q19_12 alpha           = Q12(0.0f);
        Dms_KeyframeInterpGet(prevKeyframeIdx, nextKeyframeIdx, alpha, time, charaEntry, dmsAsset);

        // Interpolate frame.
        auto        activeCharaKeyframe = DmsKeyframeCharacter{};
        const auto& charaKeyframes      = charaEntry.Keyframes;
        Dms_CharacterKeyframeInterpolate(activeCharaKeyframe,
                                         std::get<DmsKeyframeCharacter>(charaKeyframes[prevKeyframeIdx]),
                                         std::get<DmsKeyframeCharacter>(charaKeyframes[nextKeyframeIdx]),
                                         alpha);

        // Set position.
        pos->vx = Q8_TO_Q12(activeCharaKeyframe.Position.x + dmsData->Origin.x);
        pos->vy = Q8_TO_Q12(activeCharaKeyframe.Position.y + dmsData->Origin.y);
        pos->vz = Q8_TO_Q12(activeCharaKeyframe.Position.z + dmsData->Origin.z);

        // Set rotation.
        rot->vx = activeCharaKeyframe.Rotation.x;
        rot->vy = activeCharaKeyframe.Rotation.y;
        rot->vz = activeCharaKeyframe.Rotation.z;
    }

    void Dms_CharacterKeyframeInterpolate(DmsKeyframeCharacter& result,
                                          const DmsKeyframeCharacter& prevKeyframe, const DmsKeyframeCharacter& nextKeyframe,
                                          q19_12 alpha)
    {
        // Low-precision lerp between positions.
        result.Position.x = prevKeyframe.Position.x + Q12_MULT_PRECISE(nextKeyframe.Position.x - prevKeyframe.Position.x, alpha);
        result.Position.y = prevKeyframe.Position.y + Q12_MULT_PRECISE(nextKeyframe.Position.y - prevKeyframe.Position.y, alpha);
        result.Position.z = prevKeyframe.Position.z + Q12_MULT_PRECISE(nextKeyframe.Position.z - prevKeyframe.Position.z, alpha);

        // High-precision lerp between rotations.
        result.Rotation.x = Dms_AngleLerp(prevKeyframe.Rotation.x, nextKeyframe.Rotation.x, alpha);
        result.Rotation.y = Dms_AngleLerp(prevKeyframe.Rotation.y, nextKeyframe.Rotation.y, alpha);
        result.Rotation.z = Dms_AngleLerp(prevKeyframe.Rotation.z, nextKeyframe.Rotation.z, alpha);
    }

    q19_12 Dms_CameraTargetsGet(VECTOR3* posTarget, VECTOR3* lookAtTarget, q3_12* unusedAngle, q19_12 time,
                                const Asset& dmsAsset)
    {
        const auto  dmsData  = dmsAsset.GetData<DmsAsset>();
        const auto& camEntry = dmsData->CameraEntry;

        // Get keyframe interpolation.
        int    prevKeyframeIdx = 0;
        int    nextKeyframeIdx = 0;
        q19_12 alpha           = Q12(0.0f);
        Dms_KeyframeInterpGet(prevKeyframeIdx, nextKeyframeIdx, alpha, time, camEntry, dmsAsset);
        
        // Interpolate current keyframe.
        auto   activeCamKeyframe = DmsKeyframeCamera{};
        q19_12 camProjDist       = Dms_CameraKeyframeLerp(activeCamKeyframe,
                                                          std::get<DmsKeyframeCamera>(camEntry.Keyframes[prevKeyframeIdx]),
                                                          std::get<DmsKeyframeCamera>(camEntry.Keyframes[nextKeyframeIdx]),
                                                          alpha);

        // Set position target.
        posTarget->vx = Q8_TO_Q12(activeCamKeyframe.PositionTarget.x + dmsData->Origin.x);
        posTarget->vy = Q8_TO_Q12(activeCamKeyframe.PositionTarget.y + dmsData->Origin.y);
        posTarget->vz = Q8_TO_Q12(activeCamKeyframe.PositionTarget.z + dmsData->Origin.z);

        // Set look-at target.
        lookAtTarget->vx = Q8_TO_Q12(activeCamKeyframe.LookAtTarget.x + dmsData->Origin.x);
        lookAtTarget->vy = Q8_TO_Q12(activeCamKeyframe.LookAtTarget.y + dmsData->Origin.y);
        lookAtTarget->vz = Q8_TO_Q12(activeCamKeyframe.LookAtTarget.z + dmsData->Origin.z);

        // `camProjDist` comes from `curFrame.projectionDistance`, return value is passed to `vcChangeProjectionValue`.
        // Might be related to FOV?
        return camProjDist;
    }

    q19_12 Dms_CameraKeyframeLerp(DmsKeyframeCamera& result,
                                  const DmsKeyframeCamera& prevKeyframe, const DmsKeyframeCamera& nextKeyframe,
                                  q19_12 alpha)
    {
        // Set position target.
        result.PositionTarget.x = prevKeyframe.PositionTarget.x + Q12_MULT_PRECISE(nextKeyframe.PositionTarget.x - prevKeyframe.PositionTarget.x, alpha);
        result.PositionTarget.y = prevKeyframe.PositionTarget.y + Q12_MULT_PRECISE(nextKeyframe.PositionTarget.y - prevKeyframe.PositionTarget.y, alpha);
        result.PositionTarget.z = prevKeyframe.PositionTarget.z + Q12_MULT_PRECISE(nextKeyframe.PositionTarget.z - prevKeyframe.PositionTarget.z, alpha);

        // Set look-at target.
        result.LookAtTarget.x = prevKeyframe.LookAtTarget.x + Q12_MULT_PRECISE(nextKeyframe.LookAtTarget.x - prevKeyframe.LookAtTarget.x, alpha);
        result.LookAtTarget.y = prevKeyframe.LookAtTarget.y + Q12_MULT_PRECISE(nextKeyframe.LookAtTarget.y - prevKeyframe.LookAtTarget.y, alpha);
        result.LookAtTarget.z = prevKeyframe.LookAtTarget.z + Q12_MULT_PRECISE(nextKeyframe.LookAtTarget.z - prevKeyframe.LookAtTarget.z, alpha);

        // Set projection distance.
        result.ProjectionDistance = prevKeyframe.ProjectionDistance + Q12_MULT_PRECISE(nextKeyframe.ProjectionDistance - prevKeyframe.ProjectionDistance, alpha);

        return result.ProjectionDistance;
    }

    void Dms_KeyframeInterpGet(int& prevKeyframeIdx, int& nextKeyframeIdx, q19_12& alpha, q19_12 time,
                               const DmsEntry& camEntry, const Asset& dmsAsset)
    {
        int prevFrameIdx = 0;
        int nextFrameIdx = 0;

        // Define playback frames and set interpolation alpha.
        switch (Dms_SegmentStateGet(time, dmsAsset))
        {
            case DmsSegmentState::Interpolating:
                prevFrameIdx = FP_FROM(time, Q12_SHIFT);
                nextFrameIdx = prevFrameIdx + 1;
                alpha        = Q12_FRACT(time);
                break;

            case DmsSegmentState::SingleFrame:
                prevFrameIdx = FP_FROM(time, Q12_SHIFT);
                nextFrameIdx = prevFrameIdx;
                alpha        = Q12(0.0f);
                break;

            case DmsSegmentState::Ending:
                prevFrameIdx = FP_FROM(time, Q12_SHIFT) - 1;
                nextFrameIdx = prevFrameIdx + 1;
                alpha        = Q12_FRACT(time) + Q12(1.0f);
                break;
        }

        // Set previous and next keyframe indices.
        prevKeyframeIdx = Dms_KeyframeIdxGet(prevFrameIdx, camEntry);
        nextKeyframeIdx = Dms_KeyframeIdxGet(nextFrameIdx, camEntry);
    }

    DmsSegmentState Dms_SegmentStateGet(q19_12 time, const Asset& dmsAsset)
    {
        const auto dmsData = dmsAsset.GetData<DmsAsset>();

        int frameIdx = FP_FROM(time, Q12_SHIFT);

        // Run through segments.
        for (const auto& curSegment: dmsData->Segments)
        {
            // Check if at end playback frame.
            if (frameIdx != ((curSegment.StartFrameIdx + curSegment.FrameCount) - 1))
            {
                continue;
            }

            if (curSegment.FrameCount > 1)
            {
                return DmsSegmentState::Ending;
            }

            return DmsSegmentState::SingleFrame;
        }

        return DmsSegmentState::Interpolating;
    }

    int Dms_KeyframeIdxGet(int frameIdx, const DmsEntry& entry)
    {
        int keyframeIdx = frameIdx;

        // Run through ranges to freeze or offset keyframe.
        for (const auto& curHoldRange : entry.HoldRanges)
        {
            // Check if target playback frame falls before hold range.
            if (frameIdx < curHoldRange.StartFrameIdx)
            {
                break;
            }

            // If target playback frame is within hold range, freeze at mapped keyframe.
            if (frameIdx <= curHoldRange.EndFrameIdx)
            {
                keyframeIdx = curHoldRange.KeyframeIdx;
                break;
            }

            // Align keyframe index.
            keyframeIdx -= curHoldRange.EndFrameIdx - curHoldRange.StartFrameIdx;
        }

        // Clamp keyframe index.
        int clampedKeyframeIdx = CLAMP(keyframeIdx, 0, entry.Keyframes.size() - 1);
        return clampedKeyframeIdx;
    }

    q19_12 Dms_AngleLerp(q3_12 angleFrom, q3_12 angleTo, q19_12 alpha)
    {
        return Q12_ANGLE_NORM_S((q19_12)Q12_MULT_PRECISE(Q12_ANGLE_NORM_S(angleTo - angleFrom), alpha) + angleFrom);
    }
}
