#pragma once

#include "Engine/Renderer/Common/Enums.h"
#include "Engine/Renderer/Common/Objects/Primitive/Vertex3d.h"

namespace Silent::Renderer
{
    /** @brief 3D world space primitive representing a line or triangle. */
    struct Primitive3d
    {
        std::vector<Vertex3d> Vertices = {};
        BlendMode             BlendM   = BlendMode::Alpha;

        /** @brief Constructs a 3D debug line primitive with additive blending.
         *
         * @param from Start vertex.
         * @param to End vertex.
         * @param color Color.
         * @return 3D debug line primitive.
         */
        static Primitive3d CreateDebugLine(const Vector3& from, const Vector3& to, const Color& color);

        /** @brief Constructs a 3D debug triangle primitive with additive blending.
         *
         * @param vert0 First vertex.
         * @param vert1 Second vertex.
         * @param vert2 Third vertex.
         * @param color Color.
         * @return 3D debug triangle primitive.
         */
        static Primitive3d CreateDebugTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2, const Color& color);
    };
}
