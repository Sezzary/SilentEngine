#pragma once

namespace Silent::Math
{
    class EulerAngles;
    class Quaternion;
    class Vector3;

    class Matrix : public glm::mat4
    {
    public:
        // Presets

        static const Matrix Identity;

        // Constructors

        constexpr Matrix() : glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                                       0.0f, 1.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 1.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f, 1.0f) {}
        constexpr Matrix(const glm::mat4& mat) : glm::mat4(mat) {}
        constexpr Matrix(float m)              : glm::mat4(m) {}
        constexpr Matrix(float m00, float m01, float m02, float m03,
                         float m10, float m11, float m12, float m13,
                         float m20, float m21, float m22, float m23,
                         float m30, float m31, float m32, float m33) : glm::mat4(m00, m01, m02, m03,
                                                                                 m10, m11, m12, m13,
                                                                                 m20, m21, m22, m23,
                                                                                 m30, m31, m32, m33) {}

        static Matrix CreateTranslation(const Vector3& translation);
        static Matrix CreateRotationX(float rad);
        static Matrix CreateRotationY(float rad);
        static Matrix CreateRotationZ(float rad);
        static Matrix CreateScale(const Vector3& scale);
        static Matrix CreateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
        static Matrix CreatePerspective(float fov, float aspect, float nearPlane, float farPlane);
        static Matrix CreateLookAt(const Vector3& pos, const Vector3& target, const Vector3& up);

        // Utilities

        static Matrix Inverse(const Matrix& mat);
        void          Inverse();
        static Matrix Lerp(const Matrix& from, const Matrix& to, float alpha);
        void          Lerp(const Matrix& to, float alpha);
        static Matrix Translate(const Matrix& mat, const Vector3& translation);
        void          Translate(const Vector3& translation);
        static Matrix Rotate(const Matrix& mat, float rad, const Vector3& axis);
        void          Rotate(float rad, const Vector3& axis);
        static Matrix Scale(const Matrix& mat, const Vector3& scale);
        void          Scale(const Vector3& scale);

        // @todo
        // LookAt
        // Transpose
        // Determinant

        // Converters

        Vector3     ToTranslation() const;
        Vector3     ToDirection() const;
        EulerAngles ToEulerAngles() const;
        Quaternion  ToQuaternion() const;
        AxisAngle   ToAxisAngle() const;
        Vector3     ToScale() const;

        const glm::mat4& ToGlmMat4() const;
        glm::mat4&       ToGlmMat4();

        // Operators

        bool    operator==(const Matrix& mat) const;
        bool    operator!=(const Matrix& mat) const;
        Matrix& operator=(const Matrix& mat) = default;
        Matrix& operator+=(const Matrix& mat);
        Matrix& operator-=(const Matrix& mat);
        Matrix& operator*=(const Matrix& mat);
        Matrix& operator*=(float scalar);
        Matrix& operator/=(const Matrix& mat);
        Matrix& operator/=(float scalar);
    };
}
