#pragma once

// @todo temp
#include "Assets/AssetStreamer.h"
using namespace Silent::Assets;

namespace Silent::Game
{
    /** @brief DMS cutscene segment states. */
    enum class DmsSegmentState
    {
        Interpolating = 0,
        SingleFrame   = 1,
        Ending        = 2
    };

    /** @brief Gets a character transformation at a given playback time from a DMS asset by entry name.
     *
     * @param pos Output character position.
     * @param rot Output character rotation.
     * @param charaName DMS character entry name.
     * @param time Playback time.
     * @param dmsAsset DMS aset.
     */
    void Dms_CharacterTransformGet(VECTOR3* pos, SVECTOR3* rot, const std::string& charaName, q19_12 time, const Asset& dmsAsset);

    /** @brief Gets a DMS character entry index by name.
     *
     * @param charaName Name of the DMS character entry to find.
     * @param dmsAsset DMS asset.
     * @return DMS character entry index.
     */
    int Dms_CharacterGetIdxByName(const std::string& charaName, const Asset& dmsAsset);

    /** @brief Gets a character transformation at a given playback time from a DMS asset by entry index.
     *
     * @param pos Output character position.
     * @param rot Output character rotation.
     * @param charaIdx DMS character entry index.
     * @param time Playback time.
     * @param dmsAsset DMS asset.
     */
    void Dms_CharacterTransformGetByIdx(VECTOR3* pos, SVECTOR3* rot, int charaIdx, q19_12 time, const Asset& dmsAsset);

    /** @brief Linearly interpolates between two DMS character keyframes.
     *
     * @param result Interpolation result.
     * @param prevKeyframe Previous character keyframe.
     * @param nextKeyframe Next character keyframe.
     * @param alpha Interpolation alpha.
     */
    void Dms_CharacterKeyframeInterpolate(DmsKeyframeCharacter& result,
                                          const DmsKeyframeCharacter& prevKeyframe, const DmsKeyframeCharacter& nextKeyframe,
                                          q19_12 alpha);

    /** @brief Gets the camera position and look-at targets from a DMS asset.
     *
     * @param posTarget Output camera position target.
     * @param lookAtTarget Output camera look-at target.
     * @param unusedAngle @unused
     * @param time Playback time.
     * @param dmsAsset DMS asset.
     * @return Projection distance.
     */
    q19_12 Dms_CameraTargetsGet(VECTOR3* posTarget, VECTOR3* lookAtTarget, q3_12* unusedAngle, q19_12 time,
                                const DmsAsset& dmsAsset);

    /** @brief Linearly interpolates between two DMS camera keyframes.
     *
     * @param result Interpolation result.
     * @param prevKeyframe Previous camera keyframe.
     * @param nextKeyframe Next camera keyframe.
     * @param alpha Interpolation alpha.
     * @return Projection distance.
     */
    q19_12 Dms_CameraKeyframeLerp(DmsKeyframeCamera& result,
                                  const DmsKeyframeCamera& prevKeyframe, const DmsKeyframeCamera& nextKeyframe,
                                  q19_12 alpha);

    /** @brief Gets keyframe interpolation data at a given playback time from a given DMS camera entry.
     *
     * @param prevKeyframeIdx Output previous keyframe index.
     * @param nextKeyframeIdx Output next keyframe index.
     * @param alpha Output interpolation alpha.
     * @param time Playback time.
     * @param camEntry DMS camera entry.
     * @param dmsAsset DMS asset.
     */
    void Dms_KeyframeInterpGet(int& prevKeyframeIdx, int& nextKeyframeIdx, q19_12& alpha, q19_12 time,
                               const DmsEntry& camEntry, const Asset& dmsAsset);

    /** @brief Gets the state of a segment at a given playback time from a DMS asset.
     *
     * @param time Playback time.
     * @param dmsAsset DMS asset.
     * @return DMS segment state.
     */
    DmsSegmentState Dms_SegmentStateGet(q19_12 time, const Asset& dmsAsset);

    /** @brief Gets a keyframe index at a given playback frame from a DMS entry.
     *
     * @param frameIdx Playback frame index.
     * @param entry DMS entry.
     */
    int Dms_KeyframeIdxGet(int frameIdx, const DmsEntry& entry);

    /** @brief Linearly interpolates between two angles.
     *
     * @param angleFrom Angle from.
     * @param angleTo Angle to.
     * @param alpha Interpolation alpha.
     */
    q19_12 Dms_AngleLerp(q3_12 angleFrom, q3_12 angleTo, q19_12 alpha);
}
