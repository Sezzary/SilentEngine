#pragma once

namespace Silent::Math
{
    class AxisAngle;
    class Matrix;
    class Quaternion;
    class Vector3;

    // NOTE: Angle components in fixed-point Q1.15 format.
    class EulerAngles
    {
    private:
        // Constants

        static constexpr short DEFAULT_EPSILON = 3;

    public:
        // Fields

        short x = 0; // Pitch.
        short y = 0; // Yaw.
        short z = 0; // Roll.

        // Presets

        static const EulerAngles Identity;

        // Constructors

        constexpr EulerAngles() = default;
        constexpr EulerAngles(short x, short y, short z) : x(x), y(y), z(z) {}

        EulerAngles(const Vector3& dir);

        // Utilities

        static EulerAngles InterpConstant(const EulerAngles& from, const EulerAngles& eulerTo, short angularVel);
        void               InterpConstant(const EulerAngles& to, short angularVel);
        static EulerAngles Lerp(const EulerAngles& from, const EulerAngles& to, float alpha, short epsilon = DEFAULT_EPSILON);
        void               Lerp(const EulerAngles& to, float alpha, short epsilon = DEFAULT_EPSILON);
        static EulerAngles Slerp(const EulerAngles& from, const EulerAngles& to, float alpha);
        void               Slerp(const EulerAngles& to, float alpha);

        static bool Compare(const EulerAngles& eulerAngles0, const EulerAngles& eulerAngles1, short epsilon = DEFAULT_EPSILON);

        // Converters

        Vector3    ToDirection() const;
        Quaternion ToQuaternion() const;
        AxisAngle  ToAxisAngle() const;
        Matrix     ToRotationMatrix() const;

        glm::vec3 ToGlmEulerAngles() const;

        // Operators

        bool         operator==(const EulerAngles& eulerAngles) const;
        bool         operator!=(const EulerAngles& eulerAngles) const;
        EulerAngles& operator=(const EulerAngles& eulerAngles);
        EulerAngles& operator+=(const EulerAngles& eulerAngles);
        EulerAngles& operator-=(const EulerAngles& eulerAngles);
        EulerAngles& operator*=(const EulerAngles& eulerAngles);
        EulerAngles& operator*=(float scalar);
        EulerAngles& operator/=(float scalar);
        EulerAngles  operator+(const EulerAngles& eulerAngles) const;
        EulerAngles  operator-(const EulerAngles& eulerAngles) const;
        EulerAngles  operator*(const EulerAngles& eulerAngles) const;
        EulerAngles  operator*(float scalar) const;
        EulerAngles  operator/(float scalar) const;

    private:
        // Helpers

        static short InterpConstant(short from, short to, short angularVel);
        static short Lerp(short from, short to, float alpha, short epsilon = DEFAULT_EPSILON);
        static bool  Compare(short angle0, short angle1, short epsilon = DEFAULT_EPSILON);
    };
}
