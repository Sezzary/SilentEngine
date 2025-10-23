#pragma once

#include "Math/Objects/Vector3.h"

namespace Silent::Math
{
    class EulerAngles;
    class Matrix;
    class Quaternion;

    class AxisAngle
    {
    public:
        // Fields

        Vector3 Axis  = Vector3::UnitZ; // Normalized.
        float   Angle = 0.0f;           // Radians.

        // Presets

        static const AxisAngle Identity;

        // Constructors

        constexpr AxisAngle() = default;
        constexpr AxisAngle(const Vector3& axis, float angle) : Axis(axis), Angle(angle) {};

        AxisAngle(const Vector3& dir);

        // Utilities

        static AxisAngle Lerp(const AxisAngle& from, const AxisAngle& to, float alpha);
        void             Lerp(const AxisAngle& to, float alpha);
        static AxisAngle Slerp(const AxisAngle& from, const AxisAngle& to, float alpha);
        void             Slerp(const AxisAngle& to, float alpha);

        // Converters

        Vector3     ToDirection() const;
        EulerAngles ToEulerAngles() const;
        Quaternion  ToQuaternion() const;
        Matrix      ToRotationMatrix() const;

        // Operators

        bool       operator==(const AxisAngle& axisAngle) const;
        bool       operator!=(const AxisAngle& axisAngle) const;
        AxisAngle& operator=(const AxisAngle& axisAngle);
        AxisAngle& operator*=(const AxisAngle& axisAngle);
        AxisAngle  operator*(const AxisAngle& axisAngle) const;
    };
}
