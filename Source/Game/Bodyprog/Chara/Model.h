#pragma once

#include "Game/Bodyprog/Chara/Anim.h"
#include "Game/Bodyprog/Formats/Lm.h"
#include "Game/Bodyprog/Formats/Model.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Game
{
    /** @brief IPD skeleton model bone. */
    struct s_Bone
    {
        s_ModelInfo modelInfo;
        s8          idx;
        // 3 bytes of padding.
    };

    /** @brief IPD skeleton model bone node. */
    struct s_LinkedBone
    {
        s_Bone              bone;
        struct _LinkedBone* next;
    };

    struct s_Skeleton
    {
        u8            boneCount;
        u8            boneIdx;
        u8            field_2;
        s8            field_3;
        s_LinkedBone* bones_4;
        s_LinkedBone* bones_8;
        s_LinkedBone  bones_C[56];
    };

    /** @brief Skeletal character model. */
    struct s_CharaModel
    {
        u8            charaId;  /** `e_CharaId` */
        u8            isLoaded; /** `bool` */
        // 2 bytes of padding.
        s32           queueIdx;
        s_LmHeader*   lmHdr;
        s_FsImageDesc texture;
        s_Skeleton    skeleton;
    };

    /** @brief Character model animation. */
    struct s_ModelAnim
    {
        int    status;             /** Is active: bit 0, Anim index: bits 1-7. */
        int    mapAnimStatusStart; /** Start anim status of map-specific anim infos. Only used for Harry. */
        int    flags;              /** `e_AnimFlags` */
        q19_12 time;               /** Time on timeline. */
        int    keyframeIdx;        /** Active keyframe index. */
        q3_12  alpha;              /** Keyframe progress alpha. */

        std::vector<BoneTransform> BoneTransforms = {};

        s_AnimInfo* baseAnimInfos; /** Anim infos. For Harry, used for base anims. */
        s_AnimInfo* mapAnimInfos;  /** Map-specific anim infos. Only used for Harry. */
    };

    /** @brief Character model. TODO: Ambiguity with `s_CharaModel`. */
    struct s_Model
    {
        int         charaId;      /** `e_CharaId` */
        int         paletteIdx;   /** Changes the texture palette index for this model. */
        int         controlState; /** Active character control state. */
        int         stateStep;    /** Step for the current `controlState`. */ 
                                  // In `s_PlayerExtra` always 1, set to 0 for 1 tick when anim state appears to change.
                                  // Used differently in player's `s_SubCharacter`. 0: anim transitioning(?), bit 1: animated, bit 2: turning.
                                  // Sometimes holds actual anim index?
        s_ModelAnim anim;
    };

    /** @brief Packs a mesh swap status containing a swappable ID and variant ID.
     *
     * @param swappableId Swappable mesh ID.
     * @param variantId Mesh variant ID.
     * @return Packed mesh swap status containing the swappable ID and variant ID.
     */
    constexpr int MESH_SWAP_STATUS(int swappableId, int variantId)
    {
        return (swappableId << 4) | variantId;
    }

    /** @brief Retrieves the swappable mesh ID from a packed mesh swap status.
     *
     * @note The value remains shifted left by 4.
     *
     * @param meshSwap Packed mesh swap status containing a swappable ID and variant ID.
     * @return Swappable ID.
     */
    constexpr int MESH_SWAP_STATUS_SWAPPABLE_ID_GET(int meshSwap)
    {
        return meshSwap & 0xF0;
    }

    /** @brief Retrieves the variant mesh ID from a packed mesh swap status.
     *
     * @param meshSwap Packed mesh swap status containing a swappable ID and variant ID.
     * @return Variant ID.
     */
    constexpr int MESH_SWAP_STATUS_VARIANT_ID_GET(int meshSwap)
    {
        return meshSwap & 0xF;
    }

    /** @brief Sets given animation flags for a model.
     *
     * @param model Model to update.
     * @param flag Flags to set.
     */
    inline void Model_AnimFlagsSet(s_Model* model, int setFlags)
    {
        model->anim.flags |= setFlags;
    }

    /** @brief Clears given animation flags for a model.
     *
     * @param model Model to update.
     * @param clearFlags Flags to clear.
     */
    inline void Model_AnimFlagsClear(s_Model* model, int clearFlags)
    {
        model->anim.flags &= ~clearFlags;
    }

    /** @brief Similar to `Model_AnimStatusSet`, but also sets `anim.time` and `anim.keyframeIdx` from the `animInfos`
     * `s_AnimInfo` array.
     *
     * @param model Model to update.
     * @param animIdx Anim index to set.
     * @param isActive Active status to set.
     * @param animInfos Reference anim infos.
     * @param animInfosOffset Anim infos offset.
     */
    inline void Model_AnimStatusKeyframeSet(s_Model& model, int animIdx, bool isActive, s_AnimInfo* animInfos, int animInfosOffset)
    {
        if (model.stateStep == 0)
        {
            model.anim.status = ANIM_STATUS(animIdx, isActive);
            model.stateStep++;
            model.anim.time        = Q12((animInfos)[ANIM_STATUS(animIdx, isActive) + animInfosOffset].startKeyframeIdx);
            model.anim.keyframeIdx = (animInfos)[ANIM_STATUS(animIdx, isActive + animInfosOffset)].startKeyframeIdx;
        }
    }

    /** @brief Updates a model anim if `model->stateStep` is 0.
     *
     * @param model Model to update.
     * @param animIdx Anim index to set.
     * @param isActive Active status to set.
     */
    inline void Model_AnimStatusSet(s_Model* model, int animIdx, bool isActive)
    {
        if (model->stateStep == 0)
        {
            model->anim.status = ANIM_STATUS(animIdx, isActive);
            model->stateStep++;
        }
    }

    /** @brief Increments the anim status of a model anim.
     *
     * @param anim Anim to update.
     */
    inline void ModelAnim_StatusIncrement(s_ModelAnim* anim)
    {
        anim->status++;
    }

    /** @brief Decrements the anim status of a model anim.
     *
     * @param anim Anim to update.
     */
    inline void ModelAnim_StatusDecrement(s_ModelAnim* anim)
    {
        anim->status--;
    }
}
