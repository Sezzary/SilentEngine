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
    /** @brief Normalizes the UVs of meshes according to an ILM model's associated texture resolution.
     *
     * @note For some models, the texture resolution cannot be deduced correctly from ILM data alone and is therefore
     * hardcoded.
     *
     * @param name Model name.
     * @param meshes Model meshes with UVs to normalize.
     */
    static void NormalizeUvs(const std::string& name, std::vector<LmMesh>& meshes)
    {
        constexpr int RES_STEP = 64;

        static const auto ILM_TEX_RES_MAP = std::unordered_map<std::string, Vector2i>
        {
            { /*CHARA/*/"HERO"/*.ILM*/, Vector2i(256, 192) }
        };

        auto texRes = Vector2i::Zero;

        // Get texture resolution.
        const auto* obtuseTexRes = Find(ILM_TEX_RES_MAP, name);
        if (obtuseTexRes != nullptr)
        {
            texRes = *obtuseTexRes;
        }
        else
        {
            // Derive resolution from pixel UV layout.
            for (const auto& mesh : meshes)
            {
                for (const auto& vert : mesh.Linear.Vertices)
                {
                    texRes = Vector2i(std::max(texRes.x, (int)CeilToStep(vert.Uv.x, RES_STEP)),
                                      std::max(texRes.y, (int)CeilToStep(vert.Uv.y, RES_STEP)));
                }
            }
        }

        // Normalize UVs.
        for (auto& mesh : meshes)
        {
            for (auto& vert : mesh.Linear.Vertices)
            {
                vert.Uv /= texRes.ToVector2();
            }
        }
    }

    /** @brief Gets an ILM model's corresponding texture asset name.
     *
     * @note For some models, the texture name cannot be deduced correctly from the model name alone and is therefore
     * hardcoded.
     *
     * @param name Model name.
     * @return Texture asset name.
     */
    static std::string GetTextureAssetName(const std::string& name)
    {
        static const auto ILM_TIM_MAP = std::unordered_map<std::string, std::string>
        {
            { /*CHARA/*/"EI"/*.ILM*/,   "TEST/EI.TIM"      },
            { /*CHARA/*/"BIRD"/*.ILM*/, "CHARA/REBIRD.TIM" },
            { /*CHARA/*/"MAN"/*.ILM*/,  "TEST/DEV.TIM"     }, // `TEST/NINGEN.TIM` is identical.
            { /*CHARA/*/"MTH"/*.ILM*/,  "CHARA/MOTH.TIM"   },
            { /*CHARA/*/"WRM"/*.ILM*/,  "CHARA/WORM.TIM"   }
        };

        // Retrieve obtusely corresponding TIM.
        const auto* obtuseTexName = Find(ILM_TIM_MAP, name);
        if (obtuseTexName != nullptr)
        {
            return *obtuseTexName;
        }

        // Retrieve directly corresponding TIM.
        auto texName = "CHARA/" + name + ".TIM";
        return texName; 
    }

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

        // Normalize ILM UVs.
        NormalizeUvs(name, meshes);

        return std::make_shared<IlmAsset>(IlmAsset
        {
            .Lm = LmChunk
            {
                .Name        = name,
                .TextureName = GetTextureAssetName(name),
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
