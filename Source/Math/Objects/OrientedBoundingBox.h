#pragma once

#include "Math/Objects/Quaternion.h"
#include "Math/Objects/Vector3.h"

namespace Silent::Math
{
    class      AxisAlignedBoundingBox;
    class      BoundingSphere;
    class      Matrix;
    enum class ContainmentType;

    class OrientedBoundingBox
    {
    public:
        // Constants

        static constexpr uint CORNER_COUNT = 8;
        static constexpr uint EDGE_COUNT   = 12;
        static constexpr uint FACE_COUNT   = 6;

        // Fields

        Vector3    Center   = Vector3::Zero;
        Vector3    Extents  = Vector3::One;
        Quaternion Rotation = Quaternion::Identity;

        // Constructors

       constexpr OrientedBoundingBox() = default;
       constexpr OrientedBoundingBox(const Vector3& center, const Vector3& extents, const Quaternion rot) : Center(center), Extents(extents), Rotation(rot) {}

       // Getters

       float                GetWidth() const;
       float                GetHeight() const;
       float                GetDepth() const;
       float                GetSurfaceArea() const;
       float                GetVolume() const;
       std::vector<Vector3> GetCorners() const;
       Matrix               GetTransformMatrix() const;

       // Utilities

       bool Intersects(const Vector3& point) const;
       bool Intersects(const BoundingSphere& sphere) const;
       bool Intersects(const AxisAlignedBoundingBox& aabb) const;
       bool Intersects(const OrientedBoundingBox& obb) const;

       ContainmentType Contains(const Vector3& point) const;
       ContainmentType Contains(const BoundingSphere& sphere) const;
       ContainmentType Contains(const AxisAlignedBoundingBox& aabb) const;
       ContainmentType Contains(const OrientedBoundingBox& obb) const;

       // Converters

       AxisAlignedBoundingBox ToAabb() const;

       // Operators

       bool                 operator==(const OrientedBoundingBox& obb) const;
       bool                 operator!=(const OrientedBoundingBox& obb) const;
       OrientedBoundingBox& operator=(const OrientedBoundingBox& obb) = default;
    };
}
