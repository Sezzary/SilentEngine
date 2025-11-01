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
        // =======
        // Fields
        // =======

        Vector3 Axis  = Vector3::UnitZ; /** Normalized axis. */
        float   Angle = 0.0f;           /** Angle in radians. */

        // ========
        // Presets
        // ========

        static const AxisAngle Identity;

        // =============
        // Constructors
        // =============

        /** @brief Constructs a default `AxisAngle`. */
        constexpr AxisAngle() = default;

        /** @brief Constructs an `AxisAngle` from axis and angle in radians.
         *
         * @param axis Normalized axis.
         * @param angle Angle in radians.
         */
        constexpr AxisAngle(const Vector3& axis, float angle) : Axis(axis), Angle(angle) {};

        /** @brief Constructs an `AxisAngle` from a direction vector.
         *
         * @param dir Direction vector. Normalized internally.
         */
        AxisAngle(const Vector3& dir);

        // ==========
        // Utilities
        // ==========

        /** @brief Linearly interpolates bewtween two axis angles.
         *
         * @param from Start axis angle.
         * @param to End axis angle.
         * @param alpha Interpolation alpha, range `[0.0f, 1.0f]`.
         * @return Linearly interpolated axis angle.
         */
        static AxisAngle Lerp(const AxisAngle& from, const AxisAngle& to, float alpha);

        /** @brief Linearly interpolates between the axis angle and another axis angle.
         *
         * @param to End matrix.
         * @param alpha Interpolation alpha, range `[0.0f, 1.0f]`.
         */
        void Lerp(const AxisAngle& to, float alpha);

        /** @brief Spherically interpolates bewtween two axis angles.
         *
         * @param from Start axis angle.
         * @param to End axis angle.
         * @param alpha Interpolation alpha, range `[0.0f, 1.0f]`.
         * @return Spherically interpolated axis angle.
         */
        static AxisAngle Slerp(const AxisAngle& from, const AxisAngle& to, float alpha);

        /** @brief Spherically interpolates between the axis angle and another axis angle.
         *
         * @param to End matrix.
         * @param alpha Interpolation alpha, range `[0.0f, 1.0f]`.
         */
        void Slerp(const AxisAngle& to, float alpha);

        // ===========
        // Converters
        // ===========

        /** @brief Converts to a direction vector.
         *
         * @return Direction vector.
         */
        Vector3 ToDirection() const;

        /** @brief Converts to Euler angles.
         *
         * @return Euler angles.
         */
        EulerAngles ToEulerAngles() const;

        /** @brief Converts to a Quaternion.
         *
         * @return Quaternion.
         */
        Quaternion ToQuaternion() const;

        /** @brief Converts to a rotation matrix.
         *
         * @return Rotation matrix.
         */
        Matrix ToRotationMatrix() const;

        // ==========
        // Operators
        // ==========

        bool       operator==(const AxisAngle& axisAngle) const;
        bool       operator!=(const AxisAngle& axisAngle) const;
        AxisAngle& operator=(const AxisAngle& axisAngle);
        AxisAngle& operator*=(const AxisAngle& axisAngle);
        AxisAngle  operator*(const AxisAngle& axisAngle) const;
    };
}
