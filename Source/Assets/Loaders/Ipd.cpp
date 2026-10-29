#include "Framework.h"
#include "Assets/Loaders/Ipd.h"

#include "Application.h"
#include "Assets/Loaders/Utils/Lm.h"
#include "Utils/Stream.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Assets
{
    /** @brief Parses the collision chunk from an IPD asset file.
     *
     * @param stream Output file stream.
     * @return Collision data.
     */
    static IpdCollision ParseCollisionChunk(Stream& stream)
    {
        // Read position.
        q23_8 posX = stream.ReadInt32();
        q23_8 posZ = stream.ReadInt32();

        // Read array counts.
        uint8 polySplitVertCount = stream.ReadUint8();
        uint8 polySurfaceCount   = stream.ReadUint8();
        uint8 polyCount          = stream.ReadUint8();
        uint8 cylinderCount      = stream.ReadUint8();

        // Read array offsets.
        uint32 polySplitVertsOffset = stream.ReadUint32();
        uint32 polySurfacesOffset   = stream.ReadUint32();
        uint32 polysOffset          = stream.ReadUint32();
        uint32 cylindersOffset      = stream.ReadUint32();

        // Read cell grid attributes.
        uint16 cellSize   = stream.ReadInt16();
        uint8  cellCountX = stream.ReadUint8();
        uint8  cellCountZ = stream.ReadUint8();

        // Read LUT parameters.
        uint32 shapeAllocsOffset = stream.ReadUint32();
        uint16 shapeLutSize      = stream.ReadUint16();
        uint16 surfaceLutSize    = stream.ReadUint16();
        uint32 shapeLutOffset    = stream.ReadUint32();
        uint32 surfaceLutOffset  = stream.ReadUint32();

        // Read shape pass counter.
        uint8 shapePassCounter = stream.ReadUint8(); // Unused.

        stream.Skip(3);

        // Read shape pass stamp buffer.
        auto shapePassStampBuffer = std::array<uint8, 256>{};
        stream.ReadArray(ToSpan(shapePassStampBuffer)); // Unused.

        // Set stream position to polygon split vertices.
        stream.SetPosition(polySplitVertsOffset);

        // Read polygon split vertices.
        auto polySplitVerts = std::vector<Vector3i>{};
        polySplitVerts.reserve(polySplitVertCount);
        for (int i = 0; i < polySplitVertCount; i++)
        {
            // Read components.
            q7_8 x = stream.ReadInt16();
            q7_8 y = stream.ReadInt16();
            q7_8 z = stream.ReadInt16();

            // Collect vertex.
            polySplitVerts.push_back(Vector3i(x, y, z));
        }

        // Set stream position to polygon surfaces.
        stream.SetPosition(polySurfacesOffset);

        // Read polygon surfaces.
        auto polySurfaces = std::vector<IpdSurface>{};
        polySurfaces.reserve(polySurfaceCount);
        for (int i = 0; i < polySurfaceCount; i++)
        {
            // Read offset.
            q7_8 offsetX = stream.ReadInt16();
            q7_8 offsetY = stream.ReadInt16();
            q7_8 offsetZ = stream.ReadInt16();

            // Read packed attributes.
            uint16 attribsPack = stream.ReadInt16();

            // Read gradient.
            q7_8 slopeGradientX = stream.ReadInt16();
            q7_8 slopeGradientZ = stream.ReadInt16();

            // Collect surface.
            polySurfaces.push_back(IpdSurface
            {
                .Offset          = Vector3i(offsetX, offsetY, offsetZ),
                .FloorType       = GetPackedValue(attribsPack, 0, 5),
                .DisableHeight   = GetPackedValue(attribsPack, 5, 3),
                .SurfaceCacheIdx = GetPackedValue(attribsPack, 8, 3),
                .TriggerFlagIdx  = GetPackedValue(attribsPack, 11, 4),
                .SlopeGradientX  = slopeGradientX,
                .SlopeGradientZ  = slopeGradientZ
            });
        }

        // Set stream position to polygons.
        stream.SetPosition(polysOffset);

        // Read polygons.
        auto polys = std::vector<IpdPolygon>{};
        polys.reserve(polyCount);
        for (int i = 0; i < polyCount; i++)
        {
            // Read packed offset and trigger flag index.
            uint16 dataPack = stream.ReadUint16();
            q7_8   offsetZ  = stream.ReadInt16();

            // Read split vertex indices.
            uint8 splitVertIdx0 = stream.ReadUint8();
            uint8 splitVertIdxz = stream.ReadUint8();

            // Read surface indices.
            uint8 surfaceIdx0 = stream.ReadUint8();
            uint8 surfaceIdxz = stream.ReadUint8();

            stream.Skip(2);

            // Collect polygon.
            polys.push_back(IpdPolygon
            {
                .Offset          = Vector3i(GetPackedValue(dataPack, 0, 14), GetPackedValue(dataPack, 16, 14), offsetZ),
                .TriggerFlagIdx  = (GetPackedValue(dataPack, 14, 2) << 2) | GetPackedValue(dataPack, 30, 2),
                .SplitVertexIdx0 = splitVertIdx0,
                .SplitVertexIdx1 = splitVertIdxz,
                .SurfaceIdx0     = surfaceIdx0,
                .SurfaceIdx1     = surfaceIdxz
            });
        }

        // Set stream position to cylinders.
        stream.SetPosition(cylindersOffset);

        // Read cylinders.
        auto cylinders = std::vector<IpdCylinder>{};
        cylinders.reserve(cylinderCount);
        for (int i = 0; i < cylinderCount; i++)
        {
            // Read packed attributes
            uint16 attribsPack = stream.ReadUint16();

            // Read offset components.
            q7_8 x = stream.ReadInt16();
            q7_8 y = stream.ReadInt16();
            q7_8 z = stream.ReadInt16();

            // Read radius.
            q7_8 radius = stream.ReadInt16();

            // Collect cylinder.
            cylinders.push_back(IpdCylinder
            {
                .FloorType      = GetPackedValue(attribsPack, 0, 5),
                .DisableHeight  = GetPackedValue(attribsPack, 5, 3),
                .TriggerFlagIdx = GetPackedValue(attribsPack, 8, 4),
                .Offset         = Vector3i(x, y, z),
                .Radius         = radius
            });
        }

        // Set stream position to shape allocations.
        stream.SetPosition(shapeAllocsOffset);

        // Read shape allocations.
        auto shapeAllocs = std::vector<IpdShapeAlloc>{};
        shapeAllocs.reserve(cellCountX * cellCountZ);
        for (int i = 0; i < (cellCountX * cellCountZ); i++)
        {
            // Read indices.
            int16 shapeLutIdx   = stream.ReadInt16();
            int16 surfaceLutIdx = stream.ReadInt16();

            // Collect allocation.
            shapeAllocs.push_back(IpdShapeAlloc
            {
                .ShapeLutIdx   = shapeLutIdx,
                .SurfaceLutIdx = surfaceLutIdx
            });
        }

        // Set stream position to shape LUT.
        stream.SetPosition(shapeLutOffset);

        // Read shape LUT entries.
        auto shapeLut = std::vector<int>{};
        shapeLut.reserve(shapeLutSize);
        for (int i = 0; i < shapeLutSize; i++)
        {
            // Collect shape LUT entry.
            shapeLut.push_back(stream.ReadInt16());
        }

        // Set stream position to polygon surface LUT.
        stream.SetPosition(surfaceLutOffset);

        // Read polygon surface LUT entries.
        auto surfaceLut = std::vector<int>{};
        surfaceLut.reserve(surfaceLutSize);
        for (int i = 0; i < surfaceLutSize; i++)
        {
            // Collect polygon surface LUT entry.
            surfaceLut.push_back(stream.ReadInt16());
        }

        return IpdCollision
        {
            .PositionX            = posX,
            .PositionZ            = posZ,
            .CellSize             = cellSize,
            .CellCountX           = cellCountX,
            .CellCountZ           = cellCountZ,
            .PolygonSplitVertices = std::move(polySplitVerts),
            .PolygonSurfaces      = std::move(polySurfaces),
            .Polygons             = std::move(polys),
            .Cylinders            = std::move(cylinders),
            .ShapeAllocs          = std::move(shapeAllocs),
            .ShapeLut             = std::move(shapeLut),
            .SurfaceLut           = std::move(surfaceLut)
        };
    }

    /** @brief Normalizes the UVs of meshes according to an IPD model's associated texture resolution.
     *
     * @param name Model name.
     * @param meshes Model meshes with UVs to normalize.
     */
    static void NormalizeUvs(const std::string& name, std::vector<LmMesh>& meshes)
    {
        constexpr int RES_STEP = 128;

        static const auto IPD_TEX_RES_MAP = std::unordered_map<std::string, Vector2i>
        {
            // @todo Account for some exceptions. Most textures are either 128x256 or 256x256, but some are thin strips
            // or the UVs don't span far enough.
        };

        auto texRes = Vector2i::Zero;

        // Get texture resolution.
        const auto* obtuseTexRes = Find(IPD_TEX_RES_MAP, name);
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

    std::shared_ptr<void> ParseIpd(const stdfs::path& filename)
    {
        constexpr uint8 MAGIC            = 0x14;
        constexpr int   COLLISION_OFFSET = 0x54;

        const auto& fs = g_App.GetFilesystem();

        auto asset = IpdAsset{};

        // Read file.
        auto stream = Stream(filename, true, false);
        if (!stream.IsOpen())
        {
            throw std::runtime_error(Fmt("Failed to open IPD `{}`.",
                                         stdfs::relative(filename, fs.GetAssetsDirectory()).string()));
        }

        // Read header magic.
        uint8 magic = stream.ReadUint8();
        if (magic != MAGIC)
        {
            throw std::runtime_error(Fmt("Failed to parse invalid IPD `{}`.",
                                         stdfs::relative(filename, fs.GetAssetsDirectory()).string()));
        }

        // Read header.
        uint8  isInitialized    = stream.ReadUint8();
        int8   chunkX           = stream.ReadInt8();
        int8   chunkZ           = stream.ReadInt8();
        uint32 lmOffset         = stream.ReadUint32();
        uint8  modelCount       = stream.ReadUint8();
        uint8  modelBufferCount = stream.ReadUint8();
        uint8  modelOrderCount  = stream.ReadUint8();

        stream.Skip(9);

        uint32 modelInfosOffset   = stream.ReadUint32();
        uint32 modelBuffersOffset = stream.ReadUint32();
        uint8  textureCount       = stream.ReadUint8();

        stream.Skip(51); // @todo Actual texture data array? Decompilation needs work.

        uint32 modelOrderListOffset = stream.ReadUint32();

        // Set stream position to collision chunk.
        stream.SetPosition(COLLISION_OFFSET);

        // Parse collision chunk.
        auto coll = ParseCollisionChunk(stream);
        
        // Set stream position to LM chunk.
        stream.SetPosition(lmOffset);

        // Read LM chunk.
        auto meshes  = std::vector<LmMesh>{};
        auto meshIds = std::vector<int>{};
        auto name    = ParseLmChunk(filename, stream, meshes, meshIds);

        // Normalize IPD UVs.
        NormalizeUvs(name, meshes);

        // Set LM chunk.
        asset.Lm = LmChunk
        {
            .Name        = name,
            //.TextureName = GetTextureAssetName(name),// @todo
            .Meshes      = std::move(meshes),
            .MeshIds     = std::move(meshIds)
        };

        // Set stream position to model infos.
        stream.SetPosition(modelInfosOffset);

        // Read model infos.
        auto modelInfos = std::vector<IpdModelInfo>{};
        modelInfos.reserve(modelCount);
        for (int i = 0; i < modelCount; i++)
        {
            auto modelInfo        = IpdModelInfo{};
            modelInfo.IsGlobalPlm = stream.ReadUint8();

            stream.Skip(3);

            modelInfo.Name              = stream.ReadCString(8);
            modelInfo.ModelHeaderOffset = stream.ReadUint32();
            modelInfos.push_back(std::move(modelInfo));
        }

        // Set stream position to model buffers.
        stream.SetPosition(modelBuffersOffset);

        // @todo
        // Read model buffers.
        for (int i = 0; i < modelBufferCount; i++)
        {
            uint8 modelInstCount = stream.ReadUint8();
            uint8 field_1        = stream.ReadUint8();

            stream.Skip(1);

            int16  minX                   = stream.ReadInt16();
            int16  maxX                   = stream.ReadInt16();
            int16  minZ                   = stream.ReadInt16();
            int16  maxZ                   = stream.ReadInt16();
            uint32 modelInstsOffset       = stream.ReadUint32();
            uint32 field_10Offset         = stream.ReadUint32();
            uint32 subcellPositionsOffset = stream.ReadUint32();
        }

        return std::make_shared<IpdAsset>(std::move(asset));
    }

    void QueueIpdGpuUpload(const Asset& asset)
    {
        auto& renderer = g_App.GetRenderer();

        renderer.QueueMeshUpload(asset.Name);
    }

    void QueueIpdGpuRelease(const Asset& asset)
    {
        auto& renderer = g_App.GetRenderer();

        renderer.QueueMeshRelease(asset.Name);
    }
}
