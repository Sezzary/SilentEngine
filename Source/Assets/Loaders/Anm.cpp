#include "Framework.h"
#include "Assets/Loaders/Anm.h"

#include "Application.h"
#include "Utils/Stream.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Assets
{
    using LegacyMatrix = std::array<std::array<q0_7, 3>, 3>;

    /** @brief Converts a scaled translation to model space.
     *
     * @param x Fixed-point X component.
     * @param y Fixed-point Y component.
     * @param z Fixed-point Z component.
     * @param shift Fixed-point shift.
     * @return Converted translation.
     */
    static Vector3 ConvertTranslation(int x, int y, int z, int shift)
    {
        return Vector3(x << shift,
                       y << shift,
                       z << shift) / 128.0f;
    }

    /** @brief Converts a legacy matrix rotation to a quaternion.
     *
     * @param rotMat Legacy matrix rotation.
     * @return Converted rotation.
     */
    static Quaternion ConvertRotation(const LegacyMatrix& rotMat)
    {
        return Matrix(rotMat[0][0] / 128.0f,
                      rotMat[1][0] / 128.0f,
                      rotMat[2][0] / 128.0f,
                      rotMat[0][1] / 128.0f,
                      rotMat[1][1] / 128.0f,
                      rotMat[2][1] / 128.0f,
                      rotMat[0][2] / 128.0f,
                      rotMat[1][2] / 128.0f,
                      rotMat[2][2] / 128.0f).ToQuaternion();
    }

    /** @brief Parses the keyframes from an ANM file.
     *
     * @param stream Output file stream.
     * @param keyframeCount Keyframe count.
     * @param translationCount Translations per keyframe.
     * @param rotCount Rotations per keyframe.
     * @param translationShift Left shift for translation values.
     * @return ANM keyframes.
     */
    static std::vector<AnmKeyframe> ParseKeyframes(Stream& stream,
                                                   int keyframeCount, int translationCount, int rotCount,
                                                   int translationShift)
    {
        // Read keyframes.
        auto keyframes = std::vector<AnmKeyframe>{};
        keyframes.reserve(keyframeCount);
        for (int i = 0; i < keyframeCount; i++)
        {
            auto keyframe = AnmKeyframe{};

            // Read bone translations.
            keyframe.BoneTranslations.reserve(translationCount);
            for (int j = 0; j < translationCount; j++)
            {
                q0_7 x = stream.ReadInt8();
                q0_7 y = stream.ReadInt8();
                q0_7 z = stream.ReadInt8();

                // Collect bone translation.
                auto translation = ConvertTranslation(x, y, z, translationShift);
                keyframe.BoneTranslations.push_back(translation);
            }

            // Read bone rotations.
            keyframe.BoneRotation.reserve(rotCount);
            for (int j = 0; j < rotCount; j++)
            {
                auto rotMat = LegacyMatrix{};
                stream.ReadArray(ToSpan(rotMat));

                // Collect bone rotation.
                auto rot = ConvertRotation(rotMat);
                keyframe.BoneRotation.push_back(rot);
            }

            // Collect keyframe.
            keyframes.push_back(std::move(keyframe));
        }

        return keyframes;
    }

    std::shared_ptr<void> ParseAnm(const stdfs::path& filename)
    {
        constexpr int KEYFRAME_SIZE           = 156;
        constexpr int HARRY_TRANSLATION_COUNT = 1;
        constexpr int HARRY_ROT_COUNT         = 17;
        constexpr int HARRY_TRANSLATION_SHIFT = 2;

        const auto& fs = g_App.GetFilesystem();

        // Read file.
        auto stream = Stream(filename, true, false);
        if (!stream.IsOpen())
        {
            throw std::runtime_error(Fmt("Failed to open ANM `{}`.",
                                         stdfs::relative(filename, fs.GetAssetsDirectory()).string()));
        }

        // Parse ANM containing only map-specific keyframes for Harry.
        auto name = filename.filename().string();
        if (name.starts_with("HB_M"))
        {
            // Read keyframes.
            int  keyframeCount = stream.GetSize() / KEYFRAME_SIZE;
            auto keyframes     = ParseKeyframes(stream,
                                                keyframeCount, HARRY_TRANSLATION_COUNT, HARRY_ROT_COUNT,
                                                HARRY_TRANSLATION_SHIFT);

            return std::make_shared<AnmAsset>(AnmAsset
            {
                .Keyframes = std::move(keyframes),
            });
        }

        // Read header.
        int16  keyframesOffset  = stream.ReadInt16();
        uint8  rotCount         = stream.ReadUint8();
        uint8  translationCount = stream.ReadUint8();
        int16  keyframeSize     = stream.ReadInt16();
        int16  boneCount        = stream.ReadInt16();
        int32  activeBoneFlags  = stream.ReadInt32();
        int32  endOffset        = stream.ReadInt32();
        uint16 keyframeCount    = stream.ReadUint16();
        uint8  translationShift = stream.ReadUint8();

        stream.Skip(1);

        int translationsSize = translationCount * Vector3i::AXIS_COUNT;
        int rotsSize         = rotCount * sizeof(LegacyMatrix);
        Debug::Assert((translationsSize + rotsSize) == keyframeSize,
                      Fmt("Attempted to parse ANM `{}` with incongruent number of translations and rotations.",
                          stdfs::relative(filename, fs.GetAssetsDirectory()).string()));

        // Read bones.
        auto bones = std::vector<AnmBone>{};
        bones.reserve(boneCount);
        for (int i = 0; i < boneCount; i++)
        {
            int8 parentBoneIdx    = stream.ReadInt8();
            int8 rotIdx           = stream.ReadInt8();
            int8 translationIdx   = stream.ReadInt8();
            q0_7 bindTranslationX = stream.ReadInt8();
            q0_7 bindTranslationY = stream.ReadInt8();
            q0_7 bindTranslationZ = stream.ReadInt8();

            // Collect bone.
            bones.push_back(AnmBone
            {
                .ParentBoneIdx   = parentBoneIdx,
                .TranslationIdx  = translationIdx,
                .RotationIdx     = rotIdx,
                .BindTranslation = ConvertTranslation(bindTranslationX, bindTranslationY, bindTranslationZ,
                                                      translationShift)
            });
        }

        // Set stream position to keyframes.
        stream.SetPosition(keyframesOffset);

        // Read keyframes.
        auto keyframes = ParseKeyframes(stream, keyframeCount, translationCount, rotCount, translationShift);

        return std::make_shared<AnmAsset>(AnmAsset
        {
            .Bones           = std::move(bones),
            .Keyframes       = std::move(keyframes),
            .ActiveBoneFlags = activeBoneFlags
        });
    }
}
