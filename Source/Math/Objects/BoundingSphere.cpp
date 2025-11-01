#include "Framework.h"
#include "Math/Objects/BoundingSphere.h"

#include "Math/Constants.h"
#include "Math/Objects/AxisAlignedBoundingBox.h"
#include "Math/Objects/OrientedBoundingBox.h"
#include "Math/Objects/Vector3.h"

namespace Silent::Math
{
    BoundingSphere::BoundingSphere(const std::span<const Vector3>& points)
    {
        // Check if points exist.
        if (points.empty())
        {
            Center = Vector3::Zero;
            Radius = 0.0f;
            return;
        }

        // Compute centroid.
        auto pointSum = Vector3::Zero;
        for (const auto& point : points)
        {
            pointSum += point;
        }
        Center = pointSum / (float)points.size();

        // Compute max distance from centroid.
        float radiusSqr = 0.0f;
        for (const auto& point : points)
        {
            float distSqr = Vector3::DistanceSquared(Center, point);
            if (distSqr > radiusSqr)
            {
                radiusSqr = distSqr;
            }
        }
        Radius = glm::sqrt(radiusSqr);
    }

    float BoundingSphere::GetSurfaceArea() const
    {
        return SQUARE(Radius) * PI_MUL_4;
    }

    float BoundingSphere::GetVolume() const
    {
        return CUBE(Radius) * (PI * (4.0f / 3.0f));
    }

    bool BoundingSphere::Intersects(const Vector3& point) const
    {
        return Vector3::DistanceSquared(Center, point) <= SQUARE(Radius);
    }

    bool BoundingSphere::Intersects(const BoundingSphere& sphere) const
    {
        return Vector3::DistanceSquared(Center, sphere.Center) <= SQUARE(Radius + sphere.Radius);
    }

    bool BoundingSphere::Intersects(const AxisAlignedBoundingBox& aabb) const
    {
        auto closestPoint = Vector3::Clamp(Center, aabb.Center - aabb.Extents, aabb.Center + aabb.Extents);
        return Vector3::DistanceSquared(closestPoint, Center) <= SQUARE(Radius);
    }

    bool BoundingSphere::Intersects(const OrientedBoundingBox& obb) const
    {
        auto rotMat       = obb.Rotation.ToRotationMatrix();
        auto centerDelta  = obb.Center - Center;
        auto closestPoint = obb.Center;

        // Project sphere center onto each OBB axis.
        for (int i = 0; i < Vector3::AXIS_COUNT; i++)
        {
            auto axis = Vector3(rotMat[i].x, rotMat[i].y, rotMat[i].z);

            float proj = Vector3::Dot(centerDelta, axis);
            proj       = glm::clamp(proj, -obb.Extents[i], obb.Extents[i]);

            closestPoint += proj * axis;
        }

        float distSqr = Vector3::DistanceSquared(Center, closestPoint);
        return distSqr <= SQUARE(Radius);
    }
    
    ContainmentType BoundingSphere::Contains(const Vector3& point) const
    {
        return Intersects(point) ? ContainmentType::Contains : ContainmentType::None;
    }

    ContainmentType BoundingSphere::Contains(const BoundingSphere& sphere) const
    {
        float distSqr = Vector3::DistanceSquared(Center, sphere.Center);
        if (distSqr <= SQUARE(Radius - sphere.Radius))
        {
            return ContainmentType::Contains;
        }
        
        if (distSqr <= SQUARE(Radius + sphere.Radius))
        {
            return ContainmentType::Intersects;
        }

        return ContainmentType::None;
    }

    ContainmentType BoundingSphere::Contains(const AxisAlignedBoundingBox& aabb) const
    {
        auto aabbMin = aabb.GetMin();
        auto aabbMax = aabb.GetMax();
    
        auto  closestPoint = Vector3::Clamp(Center, aabbMin, aabbMax);
        float distSqr      = Vector3::DistanceSquared(closestPoint, Center);
        if (distSqr > SQUARE(Radius))
        {
            return ContainmentType::None;
        }

        auto sphereMin = Center - Vector3(Radius);
        auto sphereMax = Center + Vector3(Radius);
        if (sphereMin.x >= aabbMin.x && sphereMax.x <= aabbMax.x &&
            sphereMin.y >= aabbMin.y && sphereMax.y <= aabbMax.y &&
            sphereMin.z >= aabbMin.z && sphereMax.z <= aabbMax.z)
        {
            return ContainmentType::Contains;
        }

        return ContainmentType::Intersects;
    }

    ContainmentType BoundingSphere::Contains(const OrientedBoundingBox& obb) const
    {
        bool allInside = true;
        for (const auto& corner : obb.GetCorners())
        {
            auto centerDelta = corner - Center;
            float distSqr    = Vector3::Dot(centerDelta, centerDelta);
            if (distSqr > SQUARE(Radius))
            {
                allInside = false;
                break;
            }
        }

        if (allInside)
        {
            return ContainmentType::Contains;
        }

        if (obb.Intersects(Center))
        {
            return ContainmentType::Intersects;
        }

        return ContainmentType::None;
    }
    
    AxisAlignedBoundingBox BoundingSphere::ToAabb() const
    {
        return AxisAlignedBoundingBox(Center, Vector3(Radius));
    }

    bool BoundingSphere::operator==(const BoundingSphere& sphere) const
    {
        return Center == sphere.Center && Radius == sphere.Radius;
    }

    bool BoundingSphere::operator!=(const BoundingSphere& sphere) const
    {
        return !(*this == sphere);
    }
}
