#pragma once

#include "Game/Bodyprog/Chara/Chara.h"

namespace Silent::Game
{
    struct s_AnimInfo;

    // @todo For now, still required in some cases where `g_CharaAnimDataIdxs` is used to access map spawns.
    /** @brief Loaded NPC types in memory and their `g_CharaModelAnimsData` indices. Written to by `Fs_CharaAnimDataAlloc`. */
    extern int g_CharaAnimDataIdxs[Chara_Count];

    /** @brief Player load screen run anim info. */
    extern s_AnimInfo g_PlayerLoadScreenAnimInfo;

    /** @brief Allocates memory for character animation data.
     *
     * @param animDataIdx Index of the animation data to allocate.
     * @param charaId ID of the character associated with the animation.
     */
    void Fs_CharaAnimDataAlloc(int animDataIdx, int charaId);
}
