#pragma once

namespace Silent::Game
{
    enum   e_CharaId;
    struct BoneTransform;
    struct s_AnimInfo;
    struct s_AnmHeader;

    /** @brief Character model animation data. */
    struct s_CharaAnimData
    {
        int         activeCharaId; /** `e_CharaId` */
        std::string AnmAssetName = {};
    };

    /** @brief Loaded NPC types in memory and their `g_CharaModelAnimsData` indices. Written to by `Fs_CharaAnimDataUpdate`. */
    extern int g_CharaAnimDataIdxs[Chara_Count];

    /** @brief Animation data for loaded character models. */
    extern s_CharaAnimData g_CharaModelAnimsData[CHARA_GROUP_COUNT];

    /** @brief Player load screen run anim info. */
    extern s_AnimInfo g_PlayerLoadScreenAnimInfo;

    /** @brief Allocates memory for character animation data and bone coords.
     *
     * Handles three critical scenarios:
     * 1. If the data is already at the slot, it ensures the memory layout hasn't drifted.
     * 2. If the data is loaded in a different slot, it copies the data over to reuse it.
     * 3. If the data is new, it calls `Fs_QueueStartReadAnm` to read data from the disk.
     *
     * As a final step, it performs an overlap sweep and zeroes out any existing slots that the new allocation collided
     * with.
     *
     * @param animDataIdx Index of the animation data to allocate.
     * @param charaId ID of the character associated with the animation.
     */
    void Fs_CharaAnimDataAlloc(int animDataIdx, e_CharaId charaId);

    /** @brief Dynamically allocates where the next character animation data bone coords should be stored in
     * `g_SysWork.npcBoneCoordBuffer` based on the space consumed by the current character.
     */
    void Fs_CharaAnimBoneInfoSet(void);
}
