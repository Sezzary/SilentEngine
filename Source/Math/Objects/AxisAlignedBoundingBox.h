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
        // ==========
        // Constants
        // ==========

        static constexpr uint CORNER_COUNT = 8;
        static constexpr uint EDGE_COUNT   = 12;
        static constexpr uint FACE_COUNT   = 6;

        // =======
        // Fields
        // =======

        Vector3 Center  = Vector3::Zero;
        Vector3 Extents = Vector3::One;

        // =============
        // Constructors
        // =============
    
        /** @brief Constructs a default `AxisAlignedBoundingBox`. */
        constexpr AxisAlignedBoundingBox() = default;

        /** @brief Constructs an `AxisAlignedBoundingBox` from a center and extents.
         *
         * @param center Box center.
         * @param extents Box extents.
         */
        constexpr AxisAlignedBoundingBox(const Vector3& center, const Vector3& extents) : Center(center), Extents(extents) {}

        /** @brief Constructs an `AxisAlignedBoundingBox` from a collection of points.
         *
         * @param points Points to encompass with the AABB.
         */
        AxisAlignedBoundingBox(const std::span<const Vector3>& points);

        // ========
        // Getters
        // ========

        /** @brief Gets the X-axis width of the AABB.
         *
         * @return Width.
         */
        float GetWidth() const;

        /** @brief Gets the Y-axis height of the AABB.
         *
         * @return Height.
         */
        float GetHeight() const;

        /** @brief Gets the Z-axis depth of the AABB.
         *
         * @return Depth.
         */
        float GetDepth() const;

        /** @brief Gets the surface area of the AABB.
         *
         * @return Surface area.
         */
        float GetSurfaceArea() const;

        /** @brief Gets the volume of the AABB.
         *
         * @return Volume.
         */
        float GetVolume() const;

        /** @brief Gets the minumum corner point of the AABB.
         *
         * @return Minimum corner point.
         */
        Vector3 GetMin() const;

        /** @brief Gets the maximum corner point of the AABB.
         *
         * @return Maximum corner point.
         */
        Vector3 GetMax() const;

        /** @brief Gets the 8 corner points of the AABB.
         *
         * @return Corner points.
         */
        std::vector<Vector3> GetCorners() const;

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if the AABB intersects with a point.
         *
         * @param point Point to test against.
         * @return `true` if the intersection is valid, `false` otherwise.
         */
        bool Intersects(const Vector3& point) const;
        bool Intersects(const BoundingSphere& sphere) const;
        bool Intersects(const AxisAlignedBoundingBox& aabb) const;
        bool Intersects(const OrientedBoundingBox& obb) const;

        ContainmentType Contains(const Vector3& point) const;
        ContainmentType Contains(const BoundingSphere& sphere) const;
        ContainmentType Contains(const AxisAlignedBoundingBox& aabb) const;
        ContainmentType Contains(const OrientedBoundingBox& obb) const;

        // ==========
        // Utilities
        // ==========

        /** @brief Combines two AABBs.
         *
         * @param aabb0 First AABB.
         * @param aabb1 Second AABB.
         * @return Merged AABB.
         */
        static AxisAlignedBoundingBox Merge(const AxisAlignedBoundingBox& aabb0, const AxisAlignedBoundingBox& aabb1);

        /** @brief Combines the AABB with another AABB.
         *
         * @param aabb AABB to merge with.
         */
        void Merge(const AxisAlignedBoundingBox& aabb);

        // ===========
        // Converters
        // ===========

        /** @brief Converts the AABB to an OBB.
         *
         * @return AABB converted to an OBB.
         */
        OrientedBoundingBox ToObb() const;

        // ==========
        // Operators
        // ==========

        bool                    operator==(const AxisAlignedBoundingBox& aabb) const;
        bool                    operator!=(const AxisAlignedBoundingBox& aabb) const;
        AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& aabb) = default;
    };
}
