#pragma once

#include "Game/Bodyprog/Chara/Model.h"
#include "Game/Bodyprog/Formats/Model.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Game
{
    constexpr int WORLD_OBJECT_COUNT_MAX = 29;

    /** @brief World object metadata. */
    struct s_WorldObjectMetadata
    {
        u_Filename name;
        s8         field_8;
        s8         lmIdx; /** Set to 2 when found in `g_MapTerrain.globalLm.lmHdr` and 3-6 if found in `g_MapTerrain.activeChunks[i] (i + 3)`. */
    };

    /** @brief World object model. TODO: Rename to "static object"? Conceptually it's what this is in modern terms. */
    struct s_WorldObjectModel
    {
        s_ModelInfo           modelInfo;
        s_WorldObjectMetadata metadata;
    };

    /** @brief Geometry-space world object to draw. */
    struct s_WorldObject
    {
        s_WorldObjectModel* model;
        s32                 positionX  : 18;
        s32                 positionY  : 14;
        s32                 positionZ  : 18;
        s32                 __pad_8_18 : 14;
        s32                 rotationX  : 10;
        s32                 rotationY  : 12;
        s32                 rotationZ  : 10;
    };

    /** @brief Hand-held player item. */
    struct s_HeldItem
    {
        s32           itemId; /** `e_InvItemId` */
        s32           queueIdx;
        char*         textureName;
        s_FsImageDesc imageDesc;
        s_LmHeader*   lmHdr;
        s_Bone        bone;
    };
}
