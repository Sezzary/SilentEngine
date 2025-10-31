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

        // =============
        // Constructors
        // =============

        /** @brief Constructs a default `BoundingSphere`. */
        constexpr BoundingSphere() = default;

        /** @brief Constructs a `BoundingSphere` from a center and radius.
         *
         * @param center Sphere center.
         * @param radius Sphere radius.
         */
        constexpr BoundingSphere(const Vector3& center, float radius) : Center(center), Radius(radius) {}

        /** @brief Constructs a `BoundingSphere` from a collection of points.
         *
         * @param points Points to encompass with the sphere.
         */
        BoundingSphere(const std::span<const Vector3>& points);

        // ========
        // Getters
        // ========

        /** @brief Gets the surface area of the sphere.
         *
         * @return Surface area.
         */
        float GetSurfaceArea() const;

        /** @brief Gets the volume of the sphere.
         *
         * @return Volume.
         */
        float GetVolume() const;

        // ==========
        // Inquirers
        // ==========

        bool Intersects(const Vector3& point) const;
        bool Intersects(const BoundingSphere& sphere) const;
        bool Intersects(const AxisAlignedBoundingBox& aabb) const;
        bool Intersects(const OrientedBoundingBox& obb) const;

        ContainmentType Contains(const Vector3& point) const;
        ContainmentType Contains(const BoundingSphere& sphere) const;
        ContainmentType Contains(const AxisAlignedBoundingBox& aabb) const;
        ContainmentType Contains(const OrientedBoundingBox& obb) const;

        // ===========
        // Converters
        // ===========

        /** Converts the sphere to an AABB.
         *
         * @return AABB encompassing the sphere.
         */
        AxisAlignedBoundingBox ToAabb() const;

        // ==========
        // Operators
        // ==========

        bool            operator==(const BoundingSphere& sphere) const;
        bool            operator!=(const BoundingSphere& sphere) const;
        BoundingSphere& operator=(const BoundingSphere& sphere) = default;
    };
}
