#pragma once

namespace Silent::Renderer
{
    class ShaderProgram;

    class View
    {
    public:
        // =======
        // Fields
        // =======

        Vector3 Position  = Vector3::Zero;
        Vector3 Direction = -Vector3::UnitZ;
        Vector3 Up        = Vector3::UnitY;

        Vector2i Size = Vector2i::Zero;  
        float    Fov  = 45.0f;

        float Speed = 0.02f;

        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `View`. */
        View() = default;

        /** @brief  */
        View(const Vector3& pos, const Vector2i& size);

        // ==========
        // Utilities
        // ==========

        /** @brief Gets the combined view and projection matrix.
         *
         * @param fov Field of view in radians.
         * @param aspect Aspect ratio.
         * @param nearPlane Near plane.
         * @param farPlane Far plane.
         */
        Matrix GetMatrix(float fov, float aspect, float nearPlane, float farPlane);

        void Move();
    };
}
