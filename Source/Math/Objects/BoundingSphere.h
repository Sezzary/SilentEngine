#pragma once

#include "Math/Objects/Vector3.h"

namespace Silent::Math
{
    class      AxisAlignedBoundingBox;
    class      OrientedBoundingBox;
    enum class ContainmentType;

    class BoundingSphere
    {
    public:
        // Fields

        Vector3 Center = Vector3::Zero;
        float   Radius = 1.0f;

        // Constructors

        constexpr BoundingSphere() = default;
        constexpr BoundingSphere(const Vector3& center, float radius) : Center(center), Radius(radius) {}

        // Getters

        float GetSurfaceArea() const;
        float GetVolume() const;

        // Inquirers

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

        bool            operator==(const BoundingSphere& sphere) const;
        bool            operator!=(const BoundingSphere& sphere) const;
        BoundingSphere& operator=(const BoundingSphere& sphere) = default;
    };
}
