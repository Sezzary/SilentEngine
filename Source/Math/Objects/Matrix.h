#pragma once

namespace Silent::Math
{
    class AxisAngle;
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

        /** @brief Constructs a translation `Matrix`.
         *
         * @param translation Translation offset.
         */
        static Matrix CreateTranslation(const Vector3& translation);

        /** @brief Constructs an X-axis rotation `Matrix`.
         *
         * @param rad Rotation in radians.
         */
        static Matrix CreateRotationX(float rad);

        /** @brief Constructs a Y-axis rotation `Matrix`.
         *
         * @param rad Y rotation in radians.
         */
        static Matrix CreateRotationY(float rad);

        /** @brief Constructs a Z-axis rotation `Matrix`.
         *
         * @param rad Z rotation in radians.
         */
        static Matrix CreateRotationZ(float rad);

        /** @brief Constructs a scale `Matrix`.
         *
         * @param scale Scale.
         */
        static Matrix CreateScale(const Vector3& scale);

        /** @brief Constructs an orthographic projection `Matrix`.
         *
         * 
         */
        static Matrix CreateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);

        /** @brief Constructs a perspective projection `Matrix`.
         *
         * 
         */
        static Matrix CreatePerspective(float fov, float aspect, float nearPlane, float farPlane);

        /** @brief Constructs a view look-at `Matrix`.
         *
         * 
         */
        static Matrix CreateLookAt(const Vector3& pos, const Vector3& target, const Vector3& up);

        // ==========
        // Utilities
        // ==========

        /** @brief Inverts a matrix. */
        static Matrix Inverse(const Matrix& mat);

        /** @brief Inverts the matrix. */
        void Inverse();

        /** @brief Linearly interpolates between two matrices.
         *
         * @param from Start matrix.
         * @param to End matrix.
         * @param alpha Interpolation alpha, range `[0.0f, 1.0f]`.
         * @return Linearly interpolated matrix.
         */
        static Matrix Lerp(const Matrix& from, const Matrix& to, float alpha);

        /** @brief Linearly interpolates between the matrix and another matrix.
         *
         * @param to End matrix.
         * @param alpha Interpolation alpha, range `[0.0f, 1.0f]`.
         */
        void Lerp(const Matrix& to, float alpha);

        /** @brief Translates a matrix by a given offset.
         *
         * @param mat Matrix to translate.
         * @param translation Translation offset.
         * @return Translated matrix.
         */
        static Matrix Translate(const Matrix& mat, const Vector3& translation);

        /** @brief Translates the matrix by a given offset.
         *
         * @param translation Translation offset.
         */
        void Translate(const Vector3& translation);

        static Matrix Rotate(const Matrix& mat, float rad, const Vector3& axis);

        void Rotate(float rad, const Vector3& axis);

        /** @brief Scales a matrix by a given scale.
         *
         * @param mat Matrix to scale.
         * @param scale XYZ scale factor.
         * @return Scaled matrix.
         */
        static Matrix Scale(const Matrix& mat, const Vector3& scale);

        /** @brief Scales the matrix by a given scale.
         *
         * @param scale XYZ scale factor.
         */
        void Scale(const Vector3& scale);

        // @todo
        // LookAt
        // Transpose
        // Determinant

        // ===========
        // Converters
        // ===========

        /** @brief Extracts the translation offset from the matrix.
         *
         * @return Translation offset.
         */
        Vector3 ToTranslation() const;

        /** @brief Extracts the rotation from the matrix as a direction vector.
         *
         * @return Direction vector corresponding to the rotation.
         */
        Vector3 ToDirection() const;

        /** @brief Extracts the rotation from the matrix as Euler angles.
         * 
         * @return Euler angles rotation.
         */
        EulerAngles ToEulerAngles() const;

        /** @brief Extracts the rotation from the matrix as a quaternion.
         * 
         * @return Quaternion rotation.
         */
        Quaternion ToQuaternion() const;

        /** @brief Extracts the rotation from the matrix as an axis-angle.
         * 
         * @return Axis-angle rotation.
         */
        AxisAngle ToAxisAngle() const;

        /** @brief Extracts the scale from the matrix as an XYZ scale factor.
         *
         * @return XYZ scale factor.
         */
        Vector3 ToScale() const;

        /** @brief Downcasts the matrix to a read-only `glm::mat4`.
         *
         * @return Read-only `glm::mat4`.
         */
        const glm::mat4& ToGlmMat4() const;

        /** @brief Downcasts the matrix to a writable `glm::mat4`.
         *
         * @return Writable `glm::mat4`.
         */
        glm::mat4& ToGlmMat4();

        // ==========
        // Operators
        // ==========

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
