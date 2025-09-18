#pragma once

#include "Math/Objects/Vector3.h"

namespace Silent::Math
{
    class AxisAlignedBoundingBox;
    class BoundingSphere;
    class OrientedBoundingBox;

    class Ray
    {
    public:
        // Fields

        Vector3 Origin    = Vector3::Zero;
        Vector3 Direction = Vector3::UnitZ;

        // Constructors

        constexpr Ray() = default;
        constexpr Ray(const Vector3& origin, const Vector3& dir) : Origin(origin), Direction(dir) {}

        // Inquirers

        std::optional<float> Intersects(const BoundingSphere& sphere) const;
        std::optional<float> Intersects(const AxisAlignedBoundingBox& aabb) const;
        std::optional<float> Intersects(const OrientedBoundingBox& obb) const;
        
        // Operators

        bool operator==(const Ray& ray) const;
        bool operator!=(const Ray& ray) const;
        Ray& operator=(const Ray& ray) = default;
    };
}
