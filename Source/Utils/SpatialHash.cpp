#include "Framework.h"
#include "Utils/SpatialHash.h"

#include "Utils/Utils.h"

namespace Silent::Utils
{
    SpatialHash::Cell::Cell(const AxisAlignedBoundingBox& aabb)
    {
        Aabb = aabb;
    }

    SpatialHash::SpatialHash(float cellSize)
    {
        _cellSize        = cellSize;
        _cellAabbExtents = Vector3(cellSize / 2.0f);
    }

    uint SpatialHash::GetSize() const
    {
        return (uint)_cells.size();
    }

    std::set<int> SpatialHash::GetBoundedObjectIds() const
    {
        // Collect object IDs from all cells.
        auto objectIds = std::set<int>{};
        for (const auto& [key, cell] : _cells)
        {
            objectIds.insert(cell.ObjectIds.begin(), cell.ObjectIds.end());
        }

        return objectIds;
    }

    std::set<int> SpatialHash::GetBoundedObjectIds(const Vector3& pos) const
    {
        // Return early if no cells exist.
        if (_cells.empty())
        {
            return {};
        }

        // Get position's cell key.
        auto key = GetCellKey(pos);

        // Check if cell exists.
        auto* cell = Find(_cells, key);
        if (cell == nullptr)
        {
            return {};
        }

        // Return object IDs from cell.
        return cell->ObjectIds;
    }

    std::set<int> SpatialHash::GetBoundedObjectIds(const Ray& ray, float dist) const
    {
        // Return early if no cells exist.
        auto objectIds = std::set<int>{};
        if (_cells.empty())
        {
            return objectIds;
        }

        // Collect object IDs from cells intersecting ray.
        auto keys = GetCellKeys(ray, dist);
        for (const auto& key : keys)
        {
            // Check if cell exists.
            auto* cell = Find(_cells, key);
            if (cell == nullptr)
            {
                continue;
            }

            // Collect object IDs from cell.
            objectIds.insert(cell->ObjectIds.begin(), cell->ObjectIds.end());
        }

        return objectIds;
    }

    std::set<int> SpatialHash::GetBoundedObjectIds(const AxisAlignedBoundingBox& aabb) const
    {
        // Return early if no cells exist.
        auto objectIds = std::set<int>{};
        if (_cells.empty())
        {
            return objectIds;
        }

        // Collect object IDs from cells intersecting AABB.
        auto keys = GetCellKeys(aabb);
        for (const auto& key : keys)
        {
            // Check if cell exists.
            auto* cell = Find(_cells, key);
            if (cell == nullptr)
            {
                continue;
            }

            // Collect object IDs from cell.
            objectIds.insert(cell->ObjectIds.begin(), cell->ObjectIds.end());
        }

        return objectIds;
    }

    std::set<int> SpatialHash::GetBoundedObjectIds(const OrientedBoundingBox& obb) const
    {
        // Return early if no cells exist.
        auto objectIds = std::set<int>{};
        if (_cells.empty())
        {
            return objectIds;
        }

        // Collect object IDs from cells intersecting OBB.
        auto keys = GetCellKeys(obb);
        for (const auto& key : keys)
        {
            // Check if cell exists.
            auto* cell = Find(_cells, key);
            if (cell == nullptr)
            {
                continue;
            }

            // Collect object IDs from cell.
            objectIds.insert(cell->ObjectIds.begin(), cell->ObjectIds.end());
        }

        return objectIds;
    }

    std::set<int> SpatialHash::GetBoundedObjectIds(const BoundingSphere& sphere) const
    {
        // Return early if no cells exist.
        auto objectIds = std::set<int>{};
        if (_cells.empty())
        {
            return objectIds;
        }

        // Collect object IDs from cells intersecting sphere.
        auto keys = GetCellKeys(sphere);
        for (const auto& key : keys)
        {
            // Check if cell exists.
            auto* cell = Find(_cells, key);
            if (cell == nullptr)
            {
                continue;
            }

            // Collect object IDs from cell.
            objectIds.insert(cell->ObjectIds.begin(), cell->ObjectIds.end());
        }

        return objectIds;
    }

    bool SpatialHash::IsEmpty() const
    {
        return _cells.empty();
    }

    void SpatialHash::Insert(int objectId, const AxisAlignedBoundingBox& aabb)
    {
        // Insert object ID into cells intersecting AABB.
        auto keys = GetCellKeys(aabb);
        Insert(objectId, keys);
    }

    void SpatialHash::Insert(int objectId, const OrientedBoundingBox& obb)
    {
        // Insert object ID into cells intersecting OBB.
        auto keys = GetCellKeys(obb);
        Insert(objectId, keys);
    }

    void SpatialHash::Insert(int objectId, const BoundingSphere& sphere)
    {
        // Insert object ID into cells intersecting sphere.
        auto keys = GetCellKeys(sphere);
        Insert(objectId, keys);
    }

    void SpatialHash::Move(int objectId, const AxisAlignedBoundingBox& aabb, const AxisAlignedBoundingBox& prevAabb)
    {
        Remove(objectId, prevAabb);
        Insert(objectId, aabb);
    }

    void SpatialHash::Move(int objectId, const OrientedBoundingBox& obb, OrientedBoundingBox& prevObb)
    {
        Remove(objectId, prevObb);
        Insert(objectId, obb);
    }

    void SpatialHash::Move(int objectId, const BoundingSphere& sphere, const BoundingSphere& prevSphere)
    {
        Remove(objectId, prevSphere);
        Insert(objectId, sphere);
    }

    void SpatialHash::Remove(int objectId, const AxisAlignedBoundingBox& prevAabb)
    {
        // Remove object ID from cells intersecting previous AABB.
        auto keys = GetCellKeys(prevAabb);
        Remove(objectId, keys);
    }

    void SpatialHash::Remove(int objectId, const OrientedBoundingBox& prevObb)
    {
        // Remove object ID from cells intersecting previous OBB.
        auto keys = GetCellKeys(prevObb);
        Remove(objectId, keys);
    }

    void SpatialHash::Remove(int objectId, const BoundingSphere& prevSphere)
    {
        // Remove object ID from cells intersecting previous sphere.
        auto keys = GetCellKeys(prevSphere);
        Remove(objectId, keys);
    }

    void SpatialHash::Debug() const
    {
        constexpr auto BOX_COLOR = Color(1.0f, 1.0f, 1.0f, 0.5f);

        Debug::Message("=== Spatial Hash Debug ===");

        Debug::Message("Cells: %d", _cells.size());
        for (const auto& [key, cell] : _cells)
        {
            Debug::CreateBox(cell.Aabb.ToObb(), BOX_COLOR);
        }
    }

    Vector3i SpatialHash::GetCellKey(const Vector3& pos) const
    {
        // Compute and return key.
        return Vector3i(RoundToStep(pos.x, _cellSize) + (_cellSize * 0.5f),
                        RoundToStep(pos.y, _cellSize) + (_cellSize * 0.5f),
                        RoundToStep(pos.z, _cellSize) + (_cellSize * 0.5f));
    }

    std::vector<Vector3i> SpatialHash::GetCellKeys(const Ray& ray, float dist) const
    {
        // Reserve minimum key vector.
        auto keys = std::vector<Vector3i>{};
        keys.reserve((int)(dist / _cellSize) + 1);

        // Compute cell position.
        auto pos = Vector3(floor(ray.Origin.x / _cellSize) * _cellSize,
                           floor(ray.Origin.y / _cellSize) * _cellSize,
                           floor(ray.Origin.z / _cellSize) * _cellSize);

        // Compute cell position step.
        auto posStep = Vector3((ray.Direction.x > 0.0f) ? _cellSize : -_cellSize,
                               (ray.Direction.y > 0.0f) ? _cellSize : -_cellSize,
                               (ray.Direction.z > 0.0f) ? _cellSize : -_cellSize);

        // Compute next intersection.
        auto nextIntersect = Vector3(((pos.x + ((posStep.x > 0.0f) ? _cellSize : 0.0f)) - ray.Origin.x) / ray.Direction.x,
                                     ((pos.y + ((posStep.y > 0.0f) ? _cellSize : 0.0f)) - ray.Origin.y) / ray.Direction.y,
                                     ((pos.z + ((posStep.z > 0.0f) ? _cellSize : 0.0f)) - ray.Origin.z) / ray.Direction.z);

        // Compute ray step.
        auto rayStep = Vector3(_cellSize / abs(ray.Direction.x),
                               _cellSize / abs(ray.Direction.y),
                               _cellSize / abs(ray.Direction.z));

        // Traverse cells and collect keys.
        float curDist = 0.0f;
        while (curDist <= dist)
        {
            auto key = GetCellKey(pos);
            keys.push_back(key);

            // Determine which axis to step along.
            if (nextIntersect.x < nextIntersect.y)
            {
                if (nextIntersect.x < nextIntersect.z)
                {
                    pos.x           += posStep.x;
                    curDist          = nextIntersect.x;
                    nextIntersect.x += rayStep.x;
                }
                else
                {
                    pos.z           += posStep.z;
                    curDist          = nextIntersect.z;
                    nextIntersect.z += rayStep.z;
                }
            }
            else
            {
                if (nextIntersect.y < nextIntersect.z)
                {
                    pos.y           += posStep.y;
                    curDist          = nextIntersect.y;
                    nextIntersect.y += rayStep.y;
                }
                else
                {
                    pos.z           += posStep.z;
                    curDist          = nextIntersect.z;
                    nextIntersect.z += rayStep.z;
                }
            }
        }

        return keys;
    }

    std::vector<Vector3i> SpatialHash::GetCellKeys(const AxisAlignedBoundingBox& aabb) const
    {
        // Compute cell bounds.
        auto minCell = Vector3(FloorToStep(aabb.Center.x - aabb.Extents.x, _cellSize),
                               FloorToStep(aabb.Center.y - aabb.Extents.y, _cellSize),
                               FloorToStep(aabb.Center.z - aabb.Extents.z, _cellSize));
        auto maxCell = Vector3(FloorToStep(aabb.Center.x + aabb.Extents.x, _cellSize),
                               FloorToStep(aabb.Center.y + aabb.Extents.y, _cellSize),
                               FloorToStep(aabb.Center.z + aabb.Extents.z, _cellSize));

        // Reserve key vector.
        auto keys = std::vector<Vector3i>{};
        keys.reserve(((int)((maxCell.x - minCell.x) / _cellSize) + 1) *
                     ((int)((maxCell.y - minCell.y) / _cellSize) + 1) *
                     ((int)((maxCell.z - minCell.z) / _cellSize) + 1));

        // Collect keys of cells intersecting AABB.
        for (float x = minCell.x; x <= maxCell.x; x += _cellSize)
        {
            for (float y = minCell.y; y <= maxCell.y; y += _cellSize)
            {
                for (float z = minCell.z; z <= maxCell.z; z += _cellSize)
                {
                    auto pos = Vector3(x, y, z);
                    keys.push_back(GetCellKey(pos));
                }
            }
        }

        return keys;
    }

    std::vector<Vector3i> SpatialHash::GetCellKeys(const OrientedBoundingBox& obb) const
    {
        auto aabb = obb.ToAabb();

        // Collect keys of cells intersecting OBB.
        auto keys = GetCellKeys(aabb);
        for (auto it = keys.begin(); it != keys.end();)
        {
            const auto& key = *it;

            // Remove keys of cells not intersecting OBB.
            auto cellAabb = AxisAlignedBoundingBox(key.ToVector3(), _cellAabbExtents);
            if (!obb.Intersects(cellAabb))
            {
                it = keys.erase(it);
                continue;
            }

            it++;
        }

        return keys;
    }

    std::vector<Vector3i> SpatialHash::GetCellKeys(const BoundingSphere& sphere) const
    {
        auto aabb = AxisAlignedBoundingBox(sphere.Center, Vector3(sphere.Radius));

        // Collect keys of cells intersecting sphere.
        auto keys = GetCellKeys(aabb);
        for (auto it = keys.begin(); it != keys.end();)
        {
            const auto& key = *it;

            // Remove keys of cells not intersecting OBB.
            auto cellAabb = AxisAlignedBoundingBox(key.ToVector3(), _cellAabbExtents);
            if (!sphere.Intersects(cellAabb))
            {
                it = keys.erase(it);
                continue;
            }

            it++;
        }

        return keys;
    }

    void SpatialHash::Insert(int objectId, const std::vector<Vector3i>& keys)
    {
        // Insert object ID into cells at keys.
        for (auto& key : keys)
        {
            // Get existing cell or insert new cell.
            auto [it, isInserted] = _cells.try_emplace(key, Cell(AxisAlignedBoundingBox(key.ToVector3(), _cellAabbExtents)));

            // Insert object ID into cell.
            auto& [keyPos, cell] = *it;
            cell.ObjectIds.insert(objectId);
        }
    }

    void SpatialHash::Remove(int objectId, const std::vector<Vector3i>& keys)
    {
        // Remove object ID from cells at keys.
        for (const auto& key : keys)
        {
            // Check if cell exists.
            auto it = _cells.find(key);
            if (it == _cells.end())
            {
                continue;
            }

            // Remove object ID from cell.
            auto& [keyPos, cell] = *it;
            cell.ObjectIds.erase(objectId);

            // Remove cell if empty.
            if (cell.ObjectIds.empty())
            {
                _cells.erase(it);
            }
        }
    }
}
