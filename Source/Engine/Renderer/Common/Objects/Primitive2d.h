#pragma once

#include "Engine/Renderer/Common/Objects/Vertex2d.h"

namespace Silent::Renderer
{
    struct Primitive2d
    {
        std::vector<Vertex2d> Vertices  = {};
        int                   Depth     = 0;

        static Primitive2d CreateLine2d(const Vector2& from, const Vector2& to, const Color& color0, const Color& color1, float depth);
        static Primitive2d CreateLine2d(const Vector2& from, const Vector2& to, const Color& color, float depth);
        static Primitive2d CreateTriangle2d(const Vector2& pos, const Vector2& size, const Color& color0, const Color& color1, const Color& color2,
                                            float depth, const std::string& texId, const Vector4& uvs = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        static Primitive2d CreateTriangle2d(const Vector2& pos, const Vector2& size, const Color& color,
                                            float depth, const std::string& texId, const Vector4& uvs = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        static Primitive2d CreateQuad2d(const Vector2& pos, const Vector2& size, const Color& color0, const Color& color1, const Color& color2, const Color& color3,
                                        float depth, const std::string& texId, const Vector4& uvs = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        static Primitive2d CreateQuad2d(const Vector2& pos, const Vector2& size, const Color& color,
                                        float depth, const std::string& texId, const Vector4& uvs = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
    };
}
