#include "Framework.h"
#include "Assets/Loaders/Utils/Lm.h"

#include "Application.h"
#include "Assets/Loaders/Utils/LinearMesh.h"
#include "Renderer/Renderer.h"
#include "Renderer/Common/Resources/Layouts/Buffers.h"
#include "Services/Filesystem.h"
#include "Utils/Stream.h"
#include "Utils/Utils.h"

using namespace Silent::Services;
using namespace Silent::Renderer;
using namespace Silent::Utils;

namespace Silent::Assets
{
    std::string ParseLmChunk(const stdfs::path& filename, int baseOffset,
                             Stream& stream, std::vector<LmMesh>& meshes, std::vector<int>& meshIds)
    {
        constexpr int8   MAGIC          = 0x30;
        constexpr uint8  VER            = 6;
        constexpr uint32 NAME_OFFSET    = 20;
        constexpr int    MESH_NAME_SIZE = 8;

        const auto& fs = g_App.GetFilesystem();

        // Read header magic.
        int8 magic = stream.ReadInt8();
        if (magic != MAGIC)
        {
            throw std::runtime_error(Fmt("Failed to parse invalid LM chunk from `{}`.",
                                         stdfs::relative(filename, fs.GetAssetsDirectory()).string()));
        }

        uint8 ver           = stream.ReadUint8(); // Unused.
        uint8 isInitialized = stream.ReadUint8(); // Unused.

        stream.Skip(1);

        // Read name offset.
        uint32 nameOffset = stream.ReadUint32();
        if (nameOffset != NAME_OFFSET)
        {
            throw std::runtime_error(Fmt("Attempted to parse LM chunk from `{}` with incongruent name offset.",
                                         stdfs::relative(filename, fs.GetAssetsDirectory()).string()));
        }

        // Read header attributes.
        uint32 meshCount     = stream.ReadUint32();
        uint32 meshesOffset  = stream.ReadUint32();
        uint32 meshIdsOffset = stream.ReadUint32();
        auto   name          = stream.ReadCString();

        // Set stream position to meshes.
        stream.SetPosition(baseOffset + meshesOffset);

        // Read meshes.
        meshes.reserve(meshCount);
        for (int i = 0; i < meshCount; i++)
        {
            // Create UV index lookup.
            auto uvLookup = std::unordered_map<Vector2i, int>{}; // Key = UV (Q0.8), value = UV index.

            // Read mesh name.
            auto meshName = stream.ReadCString(MESH_NAME_SIZE);

            stream.Skip(1);

            // Create mesh.
            auto mesh = LmMesh
            {
                .Name = meshName
            };

            // Read base vertex indices.
            uint8 posBaseIdx    = stream.ReadUint8();
            uint8 normalBaseIdx = stream.ReadUint8();

            stream.Skip(1);

            // Read body offset.
            uint32 bodyOffset = stream.ReadUint32();

            // Store next mesh offset.
            int nextMeshOffset = stream.GetPosition() - baseOffset;

            // Set stream position to current mesh body.
            stream.SetPosition(baseOffset + bodyOffset);

            // Read mesh component counts.
            uint8 primCount   = stream.ReadUint8();
            uint8 posCount    = stream.ReadUint8();
            uint8 normalCount = stream.ReadUint8();

            stream.Skip(1);

            // Read mesh component offsets.
            uint32 primsOffset    = stream.ReadUint32();
            uint32 posXyOffset    = stream.ReadUint32();
            uint32 posZOffset     = stream.ReadUint32();
            uint32 normalsOffset  = stream.ReadUint32();
            uint32 nextBodyOffset = stream.ReadUint32(); // Unused.

            // Read primitives.
            stream.SetPosition(baseOffset + primsOffset);
            mesh.Native.Primitives.reserve(primCount);
            for (int j = 0; j < primCount; j++)
            {
                // Read UV 0.
                q0_8 uvX0 = stream.ReadUint8();
                q0_8 uvY0 = stream.ReadUint8();

                // Read palette index.
                int16 clutPos    = stream.ReadInt16();
                int   clutPosX   = (clutPos & 0x3F) * 0x10;
                int   clutPosZ   = (clutPos >> 6) & 0x1FF;
                int   paletteIdx = clutPosZ;

                // Read UV 1.
                q0_8 uvX1 = stream.ReadUint8();
                q0_8 uvY1 = stream.ReadUint8();

                // Read texture page.
                int16 tPage = stream.ReadInt16(); // Unused.

                // Read UV 2.
                q0_8 uvX2 = stream.ReadUint8();
                q0_8 uvY2 = stream.ReadUint8();

                // Read UV 3.
                q0_8 uvX3 = stream.ReadUint8();
                q0_8 uvY3 = stream.ReadUint8();

                // Read vertex positions.
                uint8 posIdx0 = stream.ReadUint8();
                uint8 posIdx1 = stream.ReadUint8();
                uint8 posIdx2 = stream.ReadUint8();
                uint8 posIdx3 = stream.ReadUint8();

                // Read vertex normals.
                uint8 normalIdx0 = stream.ReadUint8();
                uint8 normalIdx1 = stream.ReadUint8();
                uint8 normalIdx2 = stream.ReadUint8();
                uint8 normalIdx3 = stream.ReadUint8();

                bool isTri     = posIdx3 == UCHAR_MAX;
                int  vertCount = isTri ? TRI_VERTEX_COUNT : QUAD_VERTEX_COUNT;

                // Collect vertex indices.
                auto posIdxs = std::vector<int>
                {
                    posIdx0 - posBaseIdx,
                    posIdx1 - posBaseIdx,
                    posIdx2 - posBaseIdx
                };
                auto normalIdxs = std::vector<int>
                {
                    normalIdx0 - normalBaseIdx,
                    normalIdx1 - normalBaseIdx,
                    normalIdx2 - normalBaseIdx
                };
                auto uvIdxs = std::vector<int>
                {
                    GetLookupIdx(uvLookup, Vector2i(uvX0, uvY0)),
                    GetLookupIdx(uvLookup, Vector2i(uvX1, uvY1)),
                    GetLookupIdx(uvLookup, Vector2i(uvX2, uvY2))
                };
                if (!isTri)
                {
                    posIdxs.push_back(posIdx3 - posBaseIdx);
                    normalIdxs.push_back(normalIdx3 - normalBaseIdx);
                    uvIdxs.push_back(GetLookupIdx(uvLookup, Vector2i(uvX3, uvY3)));
                }

                // Create primitive.
                auto prim = LmPrimitive{};

                // Collect vertices.
                prim.Vertices.reserve(vertCount);
                for (int k = 0; k < vertCount; k++)
                {
                    prim.Vertices.push_back(LmVertex
                    {
                        .PositionIdx = posIdxs[k],
                        .NormalIdx   = normalIdxs[k],
                        .UvIdx       = uvIdxs[k],
                        .PaletteIdx  = paletteIdx
                    });
                }

                // Collect primitive.
                mesh.Native.Primitives.push_back(std::move(prim));
            }

            mesh.Native.Positions.reserve(posCount);

            // Read XY vertex position components.
            stream.SetPosition(baseOffset + posXyOffset);
            for (int j = 0; j < posCount; j++)
            {
                q8_7 x = stream.ReadInt16();
                q8_7 y = stream.ReadInt16();

                // Collect XZ position components.
                mesh.Native.Positions.push_back(Vector3i(x, y, 0));
            }

            // Read Z vertex position components.
            stream.SetPosition(baseOffset + posZOffset);
            for (int j = 0; j < posCount; j++)
            {
                q8_7 z = stream.ReadInt16();

                // Collect Z position component.
                mesh.Native.Positions[j].z = z;
            }

            // Read vertex normals.
            stream.SetPosition(baseOffset + normalsOffset);
            mesh.Native.Normals.reserve(normalCount);
            for (int j = 0; j < normalCount; j++)
            {
                q1_7  x     = stream.ReadInt8();
                q1_7  y     = stream.ReadInt8();
                q1_7  z     = stream.ReadInt8();
                uint8 count = stream.ReadUint8();

                // Collect normal.
                mesh.Native.Normals.push_back(Vector3i(x, y, z));
            }

            // Collect indexed UVs.
            mesh.Native.Uvs.resize(uvLookup.size());
            for (const auto& [keyUv, uvIdx] : uvLookup)
            {
                mesh.Native.Uvs[uvIdx] = keyUv;
            }

            // Collect mesh.
            meshes.push_back(std::move(mesh));

            // Set stream position to next mesh.
            stream.SetPosition(baseOffset + nextMeshOffset);
        }

        // Set stream position to mesh IDs.
        stream.SetPosition(baseOffset + meshIdsOffset);

        // Read mesh IDs.
        meshIds.reserve(meshCount);
        for (int i = 0; i < meshCount; i++)
        {
            uint8 meshId = stream.ReadUint8();

            // Collect mesh ID.
            meshIds.push_back(meshId);
        }

        // @todo Implement render buckets? Sort primitives by CLUT? Needs materials.
        // Linearize meshes.
        for (auto& mesh : meshes)
        {
            // Run through primitives.
            auto vertLookup = std::unordered_map<LmVertex, int>{};
            for (const auto& prim : mesh.Native.Primitives)
            {
                // Collect vertex indices in lookup table.
                auto vertIdxs = std::vector<uint16>{};
                for (const auto& vert : prim.Vertices)
                {
                    uint16 newVertIdx = GetLookupIdx(vertLookup, vert);
                    vertIdxs.push_back(newVertIdx);
                }

                // Collect vertex indices.
                if (vertIdxs.size() == TRI_VERTEX_COUNT)
                {
                    mesh.Linear.Idxs.insert(mesh.Linear.Idxs.end(),
                    {
                        vertIdxs[0], vertIdxs[1], vertIdxs[2]
                    });
                }
                else if (vertIdxs.size() == QUAD_VERTEX_COUNT)
                {
                    mesh.Linear.Idxs.insert(mesh.Linear.Idxs.end(),
                    {
                        vertIdxs[0], vertIdxs[1], vertIdxs[2],
                        vertIdxs[1], vertIdxs[3], vertIdxs[2]
                    });
                }
            }

            // Collect indexed vertices.
            mesh.Linear.Vertices.resize(vertLookup.size());
            for (const auto& [keyVert, vertIdx] : vertLookup)
            {
                mesh.Linear.Vertices[vertIdx] = BufferVertex3d
                {
                    .Position   = mesh.Native.Positions[keyVert.PositionIdx].ToVector3(Q7_SHIFT),
                    .Normal     = Vector3::Normalize(mesh.Native.Normals[keyVert.NormalIdx].ToVector3()),
                    .Uv         = mesh.Native.Uvs[keyVert.UvIdx].ToVector2(), // @note Unnormalized pixel coordinates.
                    .PaletteIdx = keyVert.PaletteIdx
                };
            }
        }

        return name;
    }

    void NormalizeUvs(std::vector<LmMesh>& meshes, const std::string& name,
                      int texResStep, const std::unordered_map<std::string, Vector2i>& texResMap)
    {
        auto texRes = Vector2i::Zero;

        // Get texture resolution.
        const auto* hardcodedTexRes = Find(texResMap, name);
        if (hardcodedTexRes != nullptr)
        {
            texRes = *hardcodedTexRes;
        }
        else
        {
            // Derive resolution from pixel UV layout.
            for (const auto& mesh : meshes)
            {
                for (const auto& vert : mesh.Linear.Vertices)
                {
                    texRes = Vector2i(std::max(texRes.x, (int)CeilToStep(vert.Uv.x, texResStep)),
                                      std::max(texRes.y, (int)CeilToStep(vert.Uv.y, texResStep)));
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

    std::string GetTextureAssetName(const std::string& name, const std::string& parentDirName,
                                    const std::unordered_map<std::string, std::string>& lmTimMap)
    {
        // Retrieve hardcoded corresponding TIM.
        const auto* hardcodedTexName = Find(lmTimMap, name);
        if (hardcodedTexName != nullptr)
        {
            return *hardcodedTexName;
        }

        // Retrieve plainly corresponding TIM.
        auto texName = parentDirName + "/" + name + ".TIM";
        return texName; 
    }
}
