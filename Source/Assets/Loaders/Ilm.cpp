#include "Framework.h"
#include "Assets/Loaders/Ilm.h"

#include "Application.h"
#include "Assets/AssetStreamer.h"
#include "Assets/Loaders/Utils/Lm.h"
#include "Utils/Stream.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Assets
{
    constexpr int     TEXTURE_RESOLUTION_STEP = 64;
    static const auto TEXTURE_RESOLUTION_MAP  = std::unordered_map<std::string, Vector2i>
    {
        { /*CHARA/*/"HERO"/*.ILM*/, Vector2i(256, 192) }
    };

    constexpr const char PARENT_TIM_DIR[] = "CHARA";
    static const auto    TIM_MAP          = std::unordered_map<std::string, std::string>
    {
        { /*CHARA/*/"EI"/*.ILM*/,   "TEST/EI.TIM"      },
        { /*CHARA/*/"BIRD"/*.ILM*/, "CHARA/REBIRD.TIM" },
        { /*CHARA/*/"MAN"/*.ILM*/,  "TEST/DEV.TIM"     }, // @note `TEST/NINGEN.TIM` is identical.
        { /*CHARA/*/"MTH"/*.ILM*/,  "CHARA/MOTH.TIM"   },
        { /*CHARA/*/"WRM"/*.ILM*/,  "CHARA/WORM.TIM"   }
    };

    std::shared_ptr<void> ParseIlm(const stdfs::path& filename)
    {
        const auto& fs = g_App.GetFilesystem();

        // Read file.
        auto stream = Stream(filename, true, false);
        if (!stream.IsOpen())
        {
            throw std::runtime_error(Fmt("Failed to open ILM `{}`.",
                                         stdfs::relative(filename, fs.GetAssetsDirectory()).string()));
        }

        // Parse LM chunk.
        auto meshes  = std::vector<LmMesh>{};
        auto meshIds = std::vector<int>{};
        auto name    = ParseLmChunk(filename, 0, stream, meshes, meshIds);

        // Normalize ILM mesh UVs.
        NormalizeUvs(meshes, name, TEXTURE_RESOLUTION_STEP, TEXTURE_RESOLUTION_MAP);

        return std::make_shared<IlmAsset>(IlmAsset
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

    void QueueIlmGpuUpload(const Asset& asset)
    {
        auto& renderer = g_App.GetRenderer();

        renderer.QueueMeshUpload(asset.Name);
    }

    void QueueIlmGpuRelease(const Asset& asset)
    {
        auto& renderer = g_App.GetRenderer();

        renderer.QueueMeshRelease(asset.Name);
    }
}
