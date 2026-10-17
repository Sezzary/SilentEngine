#pragma once

namespace Silent::Assets
{
    /** @brief ANM bone. */
    struct AnmBone
    {
        int     ParentBoneIdx   = 0;
        int     TranslationIdx  = 0;
        int     RotationIdx     = 0;
        Vector3 BindTranslation = Vector3::Zero;
    };

    /** @brief ANM keyframe. */
    struct AnmKeyframe
    {
        std::vector<Vector3>    BoneTranslations = {};
        std::vector<Quaternion> BoneRotation     = {};
    };

    /** @brief ANM asset data. */
    struct AnmAsset
    {
        std::vector<AnmBone>     Bones           = {};
        std::vector<AnmKeyframe> Keyframes       = {};
        int                      ActiveBoneFlags = 0;
    };

    /** @brief Parses an ANM asset file.
     *
     * @param filename Absolute asset file path.
     * @return Parsed ANM asset data as a `void` pointer.
     */
    std::shared_ptr<void> ParseAnm(const stdfs::path& filename);
}
