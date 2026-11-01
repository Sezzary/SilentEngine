#include "Framework.h"
#include "Assets/Loaders/Ipd.h"

#include "Application.h"
#include "Assets/Loaders/Utils/Lm.h"
#include "Utils/Stream.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Assets
{
    constexpr int     TEXTURE_RESOLUTION_STEP = 128;
    static const auto TEXTURE_RESOLUTION_MAP  = std::unordered_map<std::string, Vector2i>
    {
        // @todo Account for some exceptions. Most textures are either 128x256 or 256x256, but some are thin strips
        // or the UVs don't span far enough.
    };

    constexpr const char PARENT_TIM_DIR[] = "BG";
    static const auto    TIM_MAP          = std::unordered_map<std::string, std::string>
    {
        // @todo
    };

    /** @brief IPD transform matrix. */
    struct IpdMatrix
    {
        std::array<std::array<q3_12, 3>, 3> Rotation    = {};
        Vector3i                            Translation = Vector3i::Zero; /** Q19.12 */

        /** @brief Converts the IPD transform matrix to a `Matrix`.
         *
         * @return Converted transformation.
         */
        Matrix ToMatrix();
    };

    Matrix IpdMatrix::ToMatrix()
    {
        auto mat = Matrix(Q12_TO_FLT(Rotation[0][0]),
                          Q12_TO_FLT(Rotation[1][0]),
                          Q12_TO_FLT(Rotation[2][0]),
                          Q12_TO_FLT(Rotation[0][1]),
                          Q12_TO_FLT(Rotation[1][1]),
                          Q12_TO_FLT(Rotation[2][1]),
                          Q12_TO_FLT(Rotation[0][2]),
                          Q12_TO_FLT(Rotation[1][2]),
                          Q12_TO_FLT(Rotation[2][2]));

        auto translation = Vector3(Q12_TO_FLT(Translation.x),
                                   Q12_TO_FLT(Translation.y),
                                   Q12_TO_FLT(Translation.z));
        mat.Translate(translation);

        return mat;
    }

    /** @brief Parses the collision chunk from an IPD asset file.
     *
     * @param stream Output file stream.
     * @param baseOffset Base stream offset.
     * @return Collision data.
     */
    static IpdCollision ParseCollisionChunk(Stream& stream, int baseOffset)
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
        stream.SetPosition(baseOffset + polySplitVertsOffset);

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
        stream.SetPosition(baseOffset + polySurfacesOffset);

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
            uint16 attribsPack     = stream.ReadInt16();
            int    floorType       = GetPackedValue(attribsPack, 0, 5);
            int    disableHeight   = GetPackedValue(attribsPack, 5, 3);
            int    surfaceCacheIdx = GetPackedValue(attribsPack, 8, 3);
            int    triggerFlagIdx  = GetPackedValue(attribsPack, 11, 4);

            // Read gradient.
            q7_8 slopeGradientX = stream.ReadInt16();
            q7_8 slopeGradientZ = stream.ReadInt16();

            // Collect surface.
            polySurfaces.push_back(IpdSurface
            {
                .Offset          = Vector3i(offsetX, offsetY, offsetZ),
                .FloorType       = floorType,
                .DisableHeight   = disableHeight,
                .SurfaceCacheIdx = surfaceCacheIdx,
                .TriggerFlagIdx  = triggerFlagIdx,
                .SlopeGradientX  = slopeGradientX,
                .SlopeGradientZ  = slopeGradientZ
            });
        }

        // Set stream position to polygons.
        stream.SetPosition(baseOffset + polysOffset);

        // Read polygons.
        auto polys = std::vector<IpdPolygon>{};
        polys.reserve(polyCount);
        for (int i = 0; i < polyCount; i++)
        {
            // Read packed XY offset and trigger flag index.
            uint32 dataPack       = stream.ReadUint32();
            q7_8   offsetX        = GetPackedValue(dataPack, 0, 14);
            q7_8   offsetY        = GetPackedValue(dataPack, 16, 14);
            int    triggerFlagIdx = (GetPackedValue(dataPack, 14, 2) << 2) | GetPackedValue(dataPack, 30, 2);

            // Read Z offset.
            q7_8 offsetZ = stream.ReadInt16();

            // Read split vertex indices.
            uint8 splitVertIdx0 = stream.ReadUint8();
            uint8 splitVertIdx1 = stream.ReadUint8();

            // Read surface indices.
            uint8 surfaceIdx0 = stream.ReadUint8();
            uint8 surfaceIdxz = stream.ReadUint8();

            stream.Skip(2);

            // Collect polygon.
            polys.push_back(IpdPolygon
            {
                .Offset          = Vector3i(offsetX, offsetY, offsetZ),
                .TriggerFlagIdx  = triggerFlagIdx,
                .SplitVertexIdx0 = splitVertIdx0,
                .SplitVertexIdx1 = splitVertIdx1,
                .SurfaceIdx0     = surfaceIdx0,
                .SurfaceIdx1     = surfaceIdxz
            });
        }

        // Set stream position to cylinders.
        stream.SetPosition(baseOffset + cylindersOffset);

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
        stream.SetPosition(baseOffset + shapeAllocsOffset);

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
        stream.SetPosition(baseOffset + shapeLutOffset);

        // Read shape LUT entries.
        auto shapeLut = std::vector<int>{};
        shapeLut.reserve(shapeLutSize);
        for (int i = 0; i < shapeLutSize; i++)
        {
            // Collect shape LUT entry.
            shapeLut.push_back(stream.ReadInt16());
        }

        // Set stream position to polygon surface LUT.
        stream.SetPosition(baseOffset + surfaceLutOffset);

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

    std::shared_ptr<void> ParseIpd(const stdfs::path& filename)
    {
        constexpr uint8 MAGIC            = 0x14;
        constexpr int   COLLISION_OFFSET = 0x54;

        const auto& fs = g_App.GetFilesystem();

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
        uint8  modelInfoCount   = stream.ReadUint8();
        uint8  modelBufferCount = stream.ReadUint8();
        uint8  modelOrderSize   = stream.ReadUint8();

        stream.Skip(9);

        uint32 modelInfosOffset   = stream.ReadUint32();
        uint32 modelBuffersOffset = stream.ReadUint32();

        uint8 textureCount = stream.ReadUint8();
        stream.Skip(51); // @todo Actual texture data array? Decompilation needs more work.

        uint32 modelOrderListOffset = stream.ReadUint32();

        // Set stream position to collision chunk.
        stream.SetPosition(COLLISION_OFFSET);

        // Parse collision chunk.
        auto coll = ParseCollisionChunk(stream, COLLISION_OFFSET);
        
        // Set stream position to LM chunk.
        stream.SetPosition(lmOffset);

        // Read LM chunk.
        auto meshes  = std::vector<LmMesh>{};
        auto meshIds = std::vector<int>{};
        auto name    = ParseLmChunk(filename, lmOffset, stream, meshes, meshIds);

        // Normalize IPD mesh UVs.
        NormalizeUvs(meshes, name, TEXTURE_RESOLUTION_STEP, TEXTURE_RESOLUTION_MAP);

        // Set stream position to model infos.
        stream.SetPosition(modelInfosOffset);

        // Read model infos.
        auto modelInfos = std::vector<IpdModelInfo>{};
        modelInfos.reserve(modelInfoCount);
        for (int i = 0; i < modelInfoCount; i++)
        {
            uint8 isGlobalPlm = stream.ReadUint8();

            stream.Skip(3);

            auto   name              = stream.ReadCString(8);
            uint32 modelHeaderOffset = stream.ReadUint32();

            // Collect model info.
            modelInfos.push_back(IpdModelInfo
            {
                .IsGlobalPlm = isGlobalPlm,
                .Name        = name
            });
        }

        // Set stream position to model buffers.
        stream.SetPosition(modelBuffersOffset);

        // Read model buffers.
        auto modelBuffers = std::vector<IpdModelBuffer>{};
        modelBuffers.reserve(modelBufferCount);
        for (int i = 0; i < modelBufferCount; i++)
        {
            // Read array counts.
            uint8 modelInstCount     = stream.ReadUint8();
            uint8 billboardInstCount = stream.ReadUint8();
            uint8 subchunkRangeCount = stream.ReadUint8();

            stream.Skip(1);

            // Read visibility AABB.
            q7_8 minX = stream.ReadInt16();
            q7_8 maxX = stream.ReadInt16();
            q7_8 minZ = stream.ReadInt16();
            q7_8 maxZ = stream.ReadInt16();

            // Read array offsets.
            uint32 modelInstsOffset     = stream.ReadUint32();
            uint32 billboardInstsOffset = stream.ReadUint32();
            uint32 subcellRangesOffset  = stream.ReadUint32();

            // Store next model buffer offset.
            int nextModelBufferOffset = stream.GetPosition();

            // Set stream position to model instances.
            stream.SetPosition(modelInstsOffset);

            // Read model instances.
            auto modelInsts = std::vector<IpdModelInstance>{};
            modelInsts.reserve(modelInstCount);
            for (int j = 0; j < modelInstCount; j++)
            {
                uint32 modelInfoIdx = stream.ReadUint32();

                auto transformMat = IpdMatrix{};

                transformMat.Rotation[0][0] = stream.ReadInt16();
                transformMat.Rotation[0][1] = stream.ReadInt16();
                transformMat.Rotation[0][2] = stream.ReadInt16();
                transformMat.Rotation[1][0] = stream.ReadInt16();
                transformMat.Rotation[1][1] = stream.ReadInt16();
                transformMat.Rotation[1][2] = stream.ReadInt16();
                transformMat.Rotation[2][0] = stream.ReadInt16();
                transformMat.Rotation[2][1] = stream.ReadInt16();
                transformMat.Rotation[2][2] = stream.ReadInt16();

                stream.Skip(2);

                transformMat.Translation.x = stream.ReadInt32();
                transformMat.Translation.y = stream.ReadInt32();
                transformMat.Translation.z = stream.ReadInt32();

                // Collect model instance.
                modelInsts.push_back(IpdModelInstance
                {
                    .ModelInfoIdx = modelInfoIdx,
                    .TransformMat = transformMat.ToMatrix()
                });
            }

            // Set stream position to billboard sprite instances.
            stream.SetPosition(billboardInstsOffset);

            // Read billboard sprite instances.
            auto billboardInsts = std::vector<IpdBillboardInstance>{};
            billboardInsts.reserve(billboardInstCount);
            for (int j = 0; j < billboardInstCount; j++)
            {
                q7_8 offsetX = stream.ReadInt16();
                q7_8 offsetY = stream.ReadInt16();
                q7_8 offsetZ = stream.ReadInt16();
                int8 idx     = stream.ReadInt8();

                stream.Skip(1);

                // Collect billboard sprite instance.
                billboardInsts.push_back(IpdBillboardInstance
                {
                    .Idx    = idx,
                    .Offset = Vector3(Q8_TO_FLT(offsetX), Q8_TO_FLT(offsetY), Q8_TO_FLT(offsetZ))
                });
            }

            // Set stream position to subchunk ranges.
            stream.SetPosition(subcellRangesOffset);

            // Read subchunk ranges.
            auto subchunkRanges = std::vector<IpdSubchunkRange>{};
            subchunkRanges.reserve(subchunkRangeCount);
            for (int j = 0; j < subchunkRangeCount; j++)
            {
                q7_8 minX = stream.ReadInt16();
                q7_8 maxX = stream.ReadInt16();
                q7_8 minZ = stream.ReadInt16();
                q7_8 maxZ = stream.ReadInt16();

                // Collect subchunk range.
                subchunkRanges.push_back(IpdSubchunkRange
                {
                    .MinX = minX,
                    .MaxX = maxX,
                    .MinZ = minZ,
                    .MaxZ = maxZ
                });
            }

            // Collect model buffer.
            modelBuffers.push_back(IpdModelBuffer
            {
                .MinX               = minX,
                .MaxX               = maxX,
                .MinZ               = minZ,
                .MaxZ               = maxZ,
                .ModelInstances     = std::move(modelInsts),
                .BillboardInstances = std::move(billboardInsts),
                .SubchunkInstances  = std::move(subchunkRanges)
            });

            // Set stream position to next model buffer.
            stream.SetPosition(nextModelBufferOffset);
        }

        return std::make_shared<IpdAsset>(IpdAsset
        {
            .Lm = LmChunk
            {
                .Name        = name,
                .TextureName = GetTextureAssetName(name, PARENT_TIM_DIR, TIM_MAP),
                .Meshes      = std::move(meshes),
                .MeshIds     = std::move(meshIds)
            },
            .ModelInfos   = std::move(modelInfos),
            .ModelBuffers = std::move(modelBuffers),
            .Collision    = std::move(coll)
        });
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
