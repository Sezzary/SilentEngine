#pragma once

#include "Engine/Renderer/Common/Objects/Vertex2d.h"

namespace Silent::Renderer
{
    struct Primitive2d
    {
        std::vector<Vertex2d> Vertices  = {};
        int                   Depth     = 0;

        static Primitive2d CreateLine(const Vector2& from, const Vector2& to, const Color& color0, const Color& color1, int depth);
        static Primitive2d CreateTriangle(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2,
                                          const Color& color0, const Color& color1, const Color& color2, int depth);
        static Primitive2d CreateQuad(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2, const Vector2& vert3,
                                      const Color& color0, const Color& color1, const Color& color2, const Color& color3, int depth);
    };
}
