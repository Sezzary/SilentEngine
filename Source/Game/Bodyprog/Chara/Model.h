#pragma once

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
}
