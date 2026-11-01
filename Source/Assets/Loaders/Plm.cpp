#include "Framework.h"
#include "Assets/Loaders/Plm.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Assets/Loaders/Utils/Lm.h"
#include "Utils/Stream.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Assets
{
    constexpr int     TEXTURE_RESOLUTION_STEP = 32;
    static const auto TEXTURE_RESOLUTION_MAP  = std::unordered_map<std::string, Vector2i>
    {
        // @todo
    };

    constexpr const char PARENT_TIM_DIR[] = "ITEM";
    static const auto    TIM_MAP          = std::unordered_map<std::string, std::string>
    {
        // @todo
    };

    std::shared_ptr<void> ParsePlm(const stdfs::path& filename)
    {
        const auto& fs = g_App.GetFilesystem();

        // Read file.
        auto stream = Stream(filename, true, false);
        if (!stream.IsOpen())
        {
            throw std::runtime_error(Fmt("Failed to open PLM `{}`.",
                                         stdfs::relative(filename, fs.GetAssetsDirectory()).string()));
        }

        // Parse LM chunk.
        auto meshes  = std::vector<LmMesh>{};
        auto meshIds = std::vector<int>{};
        auto name    = ParseLmChunk(filename, 0, stream, meshes, meshIds);

        // Normalize PLM mesh UVs.
        NormalizeUvs(meshes, name, TEXTURE_RESOLUTION_STEP, TEXTURE_RESOLUTION_MAP);

        return std::make_shared<PlmAsset>(PlmAsset
        {
            .Lm = LmChunk
            {
                .Name        = name,
                .TextureName = GetTextureAssetName(name, PARENT_TIM_DIR, TIM_MAP),
                .Meshes      = std::move(meshes),
                .MeshIds     = std::move(meshIds)
            }
        });
    }

    void QueuePlmGpuUpload(const Asset& asset)
    {
        auto& renderer = g_App.GetRenderer();

        renderer.QueueMeshUpload(asset.Name);
    }

    void QueuePlmGpuRelease(const Asset& asset)
    {

        auto& renderer = g_App.GetRenderer();

        renderer.QueueMeshRelease(asset.Name);
    }
}
