#include "Framework.h"
#include "Math/Utils.h"

#include "Math/Constants.h"
#include "Math/Objects/Vector3.h"

namespace Silent::Math
{
    float FloorToStep(float x, float step)
    {
        return floor(x / step) * step;
    }

    float CeilToStep(float x, float step)
    {
        return ceil(x / step) * step;
    }

    float RoundToStep(float x, float step)
    {
        return round(x / step) * step;
    }

    float Remap(float x, float min0, float max0, float min1, float max1)
    {
        float alpha = (x - min0) / (max0 - min0);
        return std::lerp(min1, max1, alpha);
    }

    bool IsApproxEqual(float a, float b, float epsilon)
    {
        return (std::max(a, b) - std::min(a, b)) <= epsilon;
    }

    short GetSurfaceSlopeAngle(const Vector3& normal, const Vector3& axis)
    {
        if (normal == -axis)
        {
            return 0;
        }

        float rad = glm::acos(Vector3::Dot(normal, -axis));
        return FP_ANGLE_FROM_RAD(rad);
    }

    short GetSurfaceAspectAngle(const Vector3& normal, const Vector3& axis)
    {
        if (normal == -axis)
        {
            return 0;
        }

        // Project normal onto plane defined by `axis`.
        auto axisNorm   = Vector3::Normalize(axis);
        auto projNormal = normal - (axisNorm * Vector3::Dot(normal, axisNorm));

        // Calculate angle in 2D plane formed by projected normal.
        float rad = glm::atan2(projNormal.x, projNormal.z);
        float dot = Vector3::Dot(projNormal, axisNorm);
        if (dot < 0.0f)
        {
            rad += PI;
        }

        return FP_ANGLE_FROM_RAD(rad);
    }

    float GetDistanceToLine(const Vector3& from, const Vector3& lineStart, const Vector3& lineEnd)
    {
        auto to = GetClosestPointOnLine(from, lineStart, lineEnd);
        return Vector3::Distance(from, to);
    }

    Vector3 GetClosestPointOnLine(const Vector3& from, const Vector3& lineStart, const Vector3& lineEnd)
    {
        if (lineStart == lineEnd)
        {
            return lineStart;
        }

        auto  line  = lineEnd - lineStart;
        float alpha = Vector3::Dot(line, from - lineStart) / Vector3::Dot(line, line);

        // If closest point is out of range, return line start or end point.
        if (alpha <= 0.0f)
        {
            return lineStart;
        }
        else if (alpha >= 1.0f)
        {
            return lineEnd;
        }

        // Return closest point on line.
        return Vector3::Translate(lineStart, line, alpha);
    }

    Vector3 GetClosestPointOnLinePerp(const Vector3& from, const Vector3& lineStart, const Vector3& lineEnd, const Vector3& axis)
    {
        if (lineStart == lineEnd)
        {
            return lineStart;
        }

        // Ensure axis is normalized.
        auto axisNorm = Vector3::Normalize(axis);

        // Compute `line`.
        auto line = lineEnd - lineStart;

        // Project `line` and `from` onto plane perpendicular to `axis`.
        auto linePerp = line - (axisNorm * Vector3::Dot(line, axisNorm));
        auto fromPerp = from - (axisNorm * Vector3::Dot(from, axisNorm));

        // Compute `alpha` from line projection.
        float alpha = Vector3::Dot(linePerp, fromPerp - lineStart) / Vector3::Dot(linePerp, linePerp);

        // If closest point is out of range, return `lineStart` or `lineEnd`.
        if (alpha <= 0.0f)
        {
            return lineStart;
        }
        else if (alpha >= 1.0f)
        {
            return lineEnd;
        }

        // Return closest point on line perpendicular to `axis`.
        return Vector3::Translate(lineStart, line, alpha);
    }

    bool IsPointInFront(const Vector3& from, const Vector3& to, const Vector3& ref, const Vector3& axis)
    {
        if (from == to)
        {
            return true;
        }

        auto refDir = ref - from;

        // Project `refDir` onto plane defined by `axis`.
        auto axisNorm   = Vector3::Normalize(axis);
        auto projRefDir = refDir - (axisNorm * Vector3::Dot(refDir, axisNorm));
    
        // Project target direction onto same plane.
        auto targetDir     = to - from;
        auto projTargetDir = targetDir - (axisNorm * Vector3::Dot(targetDir, axisNorm));
    
        float dot = Vector3::Dot(projRefDir, projTargetDir);
        return dot > 0.0f;
    }

    bool IsPointOnLeft(const Vector3& from, const Vector3& to, const Vector3& ref, const Vector3& axis)
    {
        if (from == to)
        {
            return true;
        }

        auto refDir = ref - from;

        // Project `refDir` onto plane orthogonal to `axis`.
        auto axisNorm   = Vector3::Normalize(axis);
        auto projRefDir = refDir - (axisNorm * Vector3::Dot(refDir, axisNorm));

        // Normal vector to projected reference direction in 2D.
        auto headingNormal = Vector3(projRefDir.z, 0.0f, -projRefDir.x);

        // Project `targetDir` onto same plane.
        auto targetDir     = to - from;
        auto projTargetDir = targetDir - (axisNorm * Vector3::Dot(targetDir, axisNorm));

        float dot = Vector3::Dot(headingNormal, projTargetDir);
        return dot > 0.0f;
    }
}
