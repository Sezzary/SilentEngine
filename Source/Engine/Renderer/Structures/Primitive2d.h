#pragma once

#include "Engine/Renderer/Structures/Vertex2d.h"

namespace Silent::Renderer
{
    class Primitive2d
    {
    public:
        // Fields

        std::vector<Vertex2d> Vertices  = {};
        float                 Depth     = 0.0f;
        std::string           TextureId = {};

        // Constructors

        Primitive2d() = default;

        static Primitive2d CreateLine2d(const Vector2& pos0, const Vector2& pos1, const Color& color0, const Color& color1, float depth);
        static Primitive2d CreateLine2d(const Vector2& pos0, const Vector2& pos1, const Color& color, float depth);
        static Primitive2d CreateTriangle2d(const Vector2& pos, const Vector2& size, const Color& color0, const Color& color1, const Color& color2,
                                            float depth, const std::string& texId, const Vector4& atlasRegion = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        static Primitive2d CreateTriangle2d(const Vector2& pos, const Vector2& size, const Color& color,
                                            float depth, const std::string& texId, const Vector4& atlasRegion = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        static Primitive2d CreateQuad2d(const Vector2& pos, const Vector2& size, const Color& color0, const Color& color1, const Color& color2, const Color& color3,
                                        float depth, const std::string& texId, const Vector4& atlasRegion = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        static Primitive2d CreateQuad2d(const Vector2& pos, const Vector2& size, const Color& color,
                                        float depth, const std::string& texId, const Vector4& atlasRegion = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
    };
}
