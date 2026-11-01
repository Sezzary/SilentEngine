#pragma once

#include "Assets/Loaders/Utils/LinearMesh.h"
#include "Renderer/Common/Resources/Layouts/Buffers.h"
#include "Utils/Stream.h"

using namespace Silent::Renderer;
using namespace Silent::Utils;

namespace Silent::Assets
{
    /** @brief LM indexed vertex. */
    struct LmVertex
    {
        int PositionIdx = 0;
        int NormalIdx   = 0;
        int UvIdx       = 0;
        int PaletteIdx  = NO_VALUE;

        bool operator==(const LmVertex& vert) const
        {
            return PositionIdx == vert.PositionIdx &&
                   NormalIdx   == vert.NormalIdx   &&
                   UvIdx       == vert.UvIdx       &&
                   PaletteIdx  == vert.PaletteIdx;
        }
    };

    /** @brief LM primitive. */
    struct LmPrimitive
    {
        std::vector<LmVertex> Vertices = {};
    };

    /** @brief LM native format mesh. */
    struct LmNativeMesh
    {
        std::vector<LmPrimitive> Primitives = {};
        std::vector<Vector3i>    Positions  = {}; /** Q24.7 */
        std::vector<Vector3i>    Normals    = {}; /** Q24.7 */
        std::vector<Vector2i>    Uvs        = {}; /** Q23.8 */
    };

    /** @brief LM mesh. */
    struct LmMesh
    {
        std::string  Name   = {};
        LmNativeMesh Native = {};
        LinearMesh   Linear = {};
    };

    /** @brief LM chunk. */
    struct LmChunk
    {
        std::string         Name        = {};
        std::string         TextureName = {};
        std::vector<LmMesh> Meshes      = {};
        std::vector<int>    MeshIds     = {};
    };

    /** @brief Parses the LM chunk from an ILM, PLM, or IPD asset file.
     *
     * @note UVs remain as pixel coordinates and must be normalized manually according to the preferred heuristics of
     * the given parent format.
     *
     * @param filename Absolute asset file path.
     * @param baseOffset Base stream offset.
     * @param stream Output file stream.
     * @param meshes Output meshes.
     * @param meshIDs Output mesh IDs.
     * @return LM chunk name.
     */
    std::string ParseLmChunk(const stdfs::path& filename, int baseOffset,
                             Stream& stream, std::vector<LmMesh>& meshes, std::vector<int>& meshIds);

    /** @brief Normalizes the UVs of meshes according to an LM model's associated texture resolution.
     *
     * @note For some models, the texture resolution cannot be deduced correctly from LM data alone and is therefore
     * hardcoded.
     *
     * @param meshes Output model meshes with UVs to normalize.
     * @param name Model name.
     * @param texResStep Texture resolution rounding step for automatic resolution deduction.
     * @param texResMap Hardcoded name-resolution associations where resolutions cannot be deduced automatically.
     */
    void NormalizeUvs(std::vector<LmMesh>& meshes, const std::string& name,
                      int texResStep, const std::unordered_map<std::string, Vector2i>& texResMap);

    /** @brief Gets an LM model's corresponding texture asset name.
     *
     * @note For some models, the texture name cannot be deduced correctly from the model name alone and is therefore
     * hardcoded.
     *
     * @param name Model name.
     * @param parentDirName Parent directory name.
     * @param lmTimMap Hardcoded LM-TIM associations where texture asset names cannot be deduced automatically.
     * @return Texture asset name.
     */
    std::string GetTextureAssetName(const std::string& name, const std::string& parentDirName,
                                    const std::unordered_map<std::string, std::string>& lmTimMap);
}

namespace std 
{
    template<> struct hash<Silent::Assets::LmVertex> 
    {
        size_t operator()(const Silent::Assets::LmVertex& vert) const noexcept 
        {
            size_t hash = 0;
            auto CombineHash = [&](int val)
            {
                hash ^= std::hash<int>{}(val) + Silent::Math::GOLDEN_RATIO + (hash << 6) + (hash >> 2);
            };

            CombineHash(vert.PositionIdx);
            CombineHash(vert.NormalIdx);
            CombineHash(vert.UvIdx);
            return hash;
        }
    };
}
