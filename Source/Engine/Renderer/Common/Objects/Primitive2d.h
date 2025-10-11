#pragma once

#include "Engine/Renderer/Common/Objects/Vertex2d.h"

namespace Silent::Renderer
{
    constexpr uint TRIANGLE_VERTEX_COUNT = 3;
    constexpr uint QUAD_VERTEX_COUNT     = 4;

    struct Primitive2d
    {
        std::vector<Vertex2d> Vertices  = {};
        int                   Depth     = 0;

        /** @brief Constructs a line. */
        static Primitive2d CreateLine(const Vector2& from, const Vector2& to, const Color& color0, const Color& color1, int depth);

        /** @brief Constructs a triangle. */
        static Primitive2d CreateTriangle(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2,
                                          const Color& color0, const Color& color1, const Color& color2, int depth);

        /** @brief Constructs a quad. */
        static Primitive2d CreateQuad(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2, const Vector2& vert3,
                                      const Color& color0, const Color& color1, const Color& color2, const Color& color3, int depth);
    };
}
