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

    void Fs_CharaAnimDataAlloc(s32 animDataIdx, int charaId)
    {
        auto& assets = g_App.GetAssets();

        if (charaId == Chara_None)
        {
            return;
        }

        // Load ANM asset. @todo Refactor the system to make loads asynchronous again.
        int assetIdx = CHARA_FILE_INFOS[charaId].animFileIdx;
        assets.Load(assetIdx).wait();

        // Assign legacy slot.
        g_CharaAnimDataIdxs[charaId] = animDataIdx;
    }
}
