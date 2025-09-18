#pragma once

#include "Math/Objects/Vector3.h"

namespace Silent::Math
{
    class      BoundingSphere;
    class      OrientedBoundingBox;
    enum class ContainmentType;

    class AxisAlignedBoundingBox
    {
    public:
        // Constants

        static constexpr uint CORNER_COUNT = 8;
        static constexpr uint EDGE_COUNT   = 12;
        static constexpr uint FACE_COUNT   = 6;

        // Fields

        Vector3 Center  = Vector3::Zero;
        Vector3 Extents = Vector3::One;

        // Constructors
    
        constexpr AxisAlignedBoundingBox() = default;
        constexpr AxisAlignedBoundingBox(const Vector3& center, const Vector3& extents) : Center(center), Extents(extents) {}

        AxisAlignedBoundingBox(const std::span<const Vector3>& points);

        // Getters

        float                GetWidth() const;
        float                GetHeight() const;
        float                GetDepth() const;
        float                GetSurfaceArea() const;
        float                GetVolume() const;
        Vector3              GetMin() const;
        Vector3              GetMax() const;
        std::vector<Vector3> GetCorners() const;

        // Inquirers

        bool Intersects(const Vector3& point) const;
        bool Intersects(const BoundingSphere& sphere) const;
        bool Intersects(const AxisAlignedBoundingBox& aabb) const;
        bool Intersects(const OrientedBoundingBox& obb) const;

        ContainmentType Contains(const Vector3& point) const;
        ContainmentType Contains(const BoundingSphere& sphere) const;
        ContainmentType Contains(const AxisAlignedBoundingBox& aabb) const;
        ContainmentType Contains(const OrientedBoundingBox& obb) const;

        // Utilities

        static AxisAlignedBoundingBox Merge(const AxisAlignedBoundingBox& aabb0, const AxisAlignedBoundingBox& aabb1);
        void                          Merge(const AxisAlignedBoundingBox& aabb);

        // Converters

        OrientedBoundingBox ToObb() const;

        // Operators

        bool                    operator==(const AxisAlignedBoundingBox& aabb) const;
        bool                    operator!=(const AxisAlignedBoundingBox& aabb) const;
        AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& aabb) = default;
    };
}
