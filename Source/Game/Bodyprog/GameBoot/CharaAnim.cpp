#include "Framework.h"
#include "Game/Bodyprog/GameBoot/CharaAnim.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Game/Bodyprog/Chara/Anim.h"
#include "Game/Bodyprog/Chara/Chara.h"
#include "Game/Bodyprog/Sys/CharaFileInfos.h"
#include "Game/Main/FsQueue.h"
#include "Game/Maps/Characters/Harry.h"

using namespace Silent::Assets;

// @todo ????????????????????

namespace Silent::Game
{
    int g_CharaAnimDataIdxs[Chara_Count] =
    {
        NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE,
        NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE,
        NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE,
        NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE,
        NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE,
        NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE, NO_VALUE
    };

    s_CharaAnimData g_CharaModelAnimsData[CHARA_GROUP_COUNT] =
    {
        s_CharaAnimData
        {
            .activeCharaId = Chara_Harry,
            .AnmAssetName  = {}
        }
    };

    s_AnimInfo g_PlayerLoadScreenAnimInfo =
    {
        .playbackFunc        = Anim_PlayLoop,
        .status              = ANIM_STATUS(HarryAnim_Still, false),
        .hasVariableDuration = false,
        .linkStatus          = ANIM_STATUS(HarryAnim_Still, false),
        .duration            = { Q12(8.0f) },
        .startKeyframeIdx    = 26, // } `HarryAnim_RunForward` keyframe range.
        .endKeyframeIdx      = 44  // }
    };

    /** @brief Finds the character animation data slot index.
     * If not found, it returns index 0.
     *
     * @param charaId ID of the character for which to find the animation data.
     * @return Animation data index.
     */
    static s32 Fs_CharaAnimDataIdxGet(e_CharaId charaId)
    {
        // Find existing character data.
        for (int i = 1; (i < CHARA_GROUP_COUNT); i++)
        {
            if (g_CharaModelAnimsData[i].activeCharaId == charaId)
            {
                return i;
            }
        }

        return 0;
    }

    /** @brief Assigns character animation data to a free slot and initializes character model bones.
     * Called by `Fs_QueuePostLoadAnm`.
     *
     * @param animDataIdx Index of the animation data to update.
     * @param charaId ID of the character associated with the animation.
     */
    static void Fs_CharaAnimDataUpdate(s32 animDataIdx, e_CharaId charaId)
    {
        auto& assets = g_App.GetAssets();

        const auto& charaFileInfo = CHARA_FILE_INFOS[(int)charaId];
        const auto* anmAsset      = assets[assets.GetName(charaFileInfo.animFileIdx)];
        const auto  anmData       = anmAsset->GetData<AnmAsset>();

        // Assign ANM asset to slot.
        auto& newAnimData            = g_CharaModelAnimsData[animDataIdx];
        newAnimData->activeCharaId   = charaId;
        newAnimData->AnmAssetName    = anmAsset->Name;
        g_CharaAnimDataIdxs[charaId] = animDataIdx;

        // ???
        //Anim_BonesInit(animFile, localBoneCoords);
    }

    void Fs_CharaAnimDataAlloc(s32 animDataIdx, e_CharaId charaId)
    {
        if (charaId == Chara_None)
        {
            return;
        }

        int foundAnimDataIdx = Fs_CharaAnimDataIdxGet(charaId);
        if (foundAnimDataIdx == 0)
        {
            // @todo Refactor the system to make loads asynchronous again.
            Fs_QueueStartReadAnm(animDataIdx, charaId).wait();
        }
        Fs_CharaAnimDataUpdate(animDataIdx, charaId);
    }

    void Fs_CharaAnimBoneInfoSet(void) // 0x8003569C
    {
        // Run through character group.
        for (int i = 1; i < (CHARA_GROUP_COUNT - 1); i++)
        {
            // Skip if no valid character model.
            if (g_MapOverlayHdr.charaGroupIds[i] == Chara_None)
            {
                continue;
            }

            // ???
            //Anim_BonesInit(anmHdr, boneCoords);
        }
    }
}
