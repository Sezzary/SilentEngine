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
        // =======
        // Fields
        // =======

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

        /** @brief Checks if a point intersects the sphere.
         *
         * @param point Point to test against.
         * @return `true` if the intersection is valid, `false` otherwise.
         */
        bool Intersects(const Vector3& point) const;

        /** @brief Checks if a sphere intersects the sphere.
         *
         * @param sphere Sphere to test against.
         * @return `true` if the intersection is valid, `false` otherwise.
         */
        bool Intersects(const BoundingSphere& sphere) const;

        /** @brief Checks if an AABB intersects the sphere.
         *
         * @param aabb AABB to test against.
         * @return `true` if the intersection is valid, `false` otherwise.
         */
        bool Intersects(const AxisAlignedBoundingBox& aabb) const;

        /** @brief Checks if an OBB intersects the sphere.
         *
         * @param obb OBB to test against.
         * @return `true` if the intersection is valid, `false` otherwise.
         */
        bool Intersects(const OrientedBoundingBox& obb) const;

        /** @brief Checks if a point is contained by the sphere.
         *
         * @param point Point to test for containment.
         * @return Containment type.
         */
        ContainmentType Contains(const Vector3& point) const;

        /** @brief Checks if a sphere is contained by the sphere.
         *
         * @param sphere Sphere to test for containment.
         * @return Containment type.
         */
        ContainmentType Contains(const BoundingSphere& sphere) const;

        /** @brief Checks if an AABB is contained by the sphere.
         *
         * @param aabb AABB to test for containment.
         * @return Containment type.
         */
        ContainmentType Contains(const AxisAlignedBoundingBox& aabb) const;

        /** @brief Checks if an OBB is contained by the sphere.
         *
         * @param obb OBB to test for containment.
         * @return Containment type.
         */
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
