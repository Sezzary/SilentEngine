#pragma once

#include "Assets/Loaders/Utils/Lm.h"

namespace Silent::Assets
{
    struct Asset;

    /** @brief IPD model info. */
    struct IpdModelInfo
    {
        uint8       IsGlobalPlm       = false;
        std::string Name              = {};
        uint32      ModelHeaderOffset = 0;
    };

    /** @brief IPD model buffer. */
    struct IpdModelBuffer
    {

    };

    // TODO: Move to Collision.h when ready. This replaces fields originally modified in loaded IPDs themselves.
    struct ShapePass
    {
        int ShapePassCounter;          /** Active shape pass. Stamps below this value, i.e. from an older pass, are "unvisited". */
        int ShapePassStampBuffer[256]; /** Tracks visited shapes per query pass. */
    };

    /** @brief IPD collision surface. */
    struct IpdSurface
    {
        Vector3i Offset          = Vector3i::Zero; /** Q7.8 */
        int      FloorType       = 0;              /** `e_FloorType` */
        bool     DisableHeight   = false;
        int      SurfaceCacheIdx = 0;              /** Index into `s_CollisionState::shape.surfaceCache`. */
        int      TriggerFlagIdx  = 0;              /** `e_CollisionTriggerFlags` index. */
        q7_8     SlopeGradientX  = Q8(0.0f);
        q7_8     SlopeGradientZ  = Q8(0.0f);
    };

    /** @brief IPD collision polygon shape composed of up to 2 surfaces split by a line. */
    struct IpdPolygon
    {
        Vector3i Offset          = Vector3i::Zero; /** Q7.8 */
        int      TriggerFlagIdx  = 0;              /** `e_CollisionTriggerFlags` index. */
        int      SplitVertexIdx0 = 0;              /** `s_IpdCollisionData::polygonSplitVertices` index. */
        int      SplitVertexIdx1 = 0;              /** `s_IpdCollisionData::polygonSplitVertices` index. */
        int      SurfaceIdx0     = 0;              /** `s_IpdCollisionData::polygonSurfaces` index. */
        int      SurfaceIdx1     = 0;              /** `s_IpdCollisionData::polygonSurfaces` index. */
    };

    /** @brief IPD collision cylinder shape. */
    struct IpdCylinder
    {
        int      FloorType      = 0;              /** `e_FloorType` */
        bool     DisableHeight  = false;
        int      TriggerFlagIdx = 0;              /** `e_CollisionTriggerFlags` index. */
        Vector3i Offset         = Vector3i::Zero; /** Q7.8 */
        q7_8     Radius         = Q8(0.0f);
    };

    /** @brief IPD collision shapes allocation for a cell (represented by pair). */
    struct IpdShapeAlloc
    {
        int ShapeLutIdx   = 0; /** Index into `IpdCollision::ShapeLut`. */
        int SurfaceLutIdx = 0; /** Index into `IpdCollision::SurfaceLut`. */
    };

    /** @brief IPD collision. */
    struct IpdCollision
    {
        q23_8                      PositionX            = Q8(0.0f);
        q23_8                      PositionZ            = Q8(0.0f);
        q23_8                      CellSize             = Q8(0.0f); /** `Q8(2.0f)` in most IPD files, sometimes `Q8(0.0f)`. */
        int                        CellCountX           = 0;        /** 20 in most IPD files, sometimes 0. */
        int                        CellCountZ           = 0;        /** 20 in most IPD files, sometimes 0. */
        std::vector<Vector3i>      PolygonSplitVertices = {};       /** Q7.8 | Pairs define a polygon line split. */
        std::vector<IpdSurface>    PolygonSurfaces      = {};       /** Unique collision surfaces. */
        std::vector<IpdPolygon>    Polygons             = {};       /** Unique collision polygons. */
        std::vector<IpdCylinder>   Cylinders            = {};       /** Unique collision cylinders. */
        std::vector<IpdShapeAlloc> ShapeAllocs          = {};       /** Shape allocations per cell. Pairs of elements define index ranges. */
        std::vector<int>           ShapeLut             = {};       /** Indices into `polygons` + `cylinders`. */
        std::vector<int>           SurfaceLut           = {};       /** Indices into `polygonSurfaces`. */
    };

    /** @brief IPD asset data. */
    struct IpdAsset
    {
        LmChunk        Lm           = {};
        IpdModelInfo   ModelInfos   = {};
        IpdModelBuffer ModelBuffers = {};
        IpdCollision   Collision    = {};
    };

    /** @brief Parses an IPD asset file.
     *
     * @param filename Absolute asset file path.
     * @return Parsed IPD asset data as a `void` pointer.
     */
    std::shared_ptr<void> ParseIpd(const stdfs::path& filename);

    /** @brief Queues an IPD asset for upload to the GPU as meshes.
     *
     * @param asset IPD asset.
     */
    void QueueIpdGpuUpload(const Asset& asset);

    /** @brief Queues a IPD asset to release from the GPU as meshes.
     *
     * @param asset IPD asset.
     */
    void QueueIpdGpuRelease(const Asset& asset);
}
