#include "Framework.h"
#include "Engine/Renderer/Common/Objects/Primitive2d.h"

#include "Engine/Renderer/Common/Objects/Vertex2d.h"

namespace Silent::Renderer
{
    Primitive2d Primitive2d::CreateLine2d(const Vector2& pos0, const Vector2& pos1, const Color& color0, const Color& color1, float depth)
    {
        // Create primitive.
        auto prim  = Primitive2d();
        prim.Depth = depth;

        // Define vertices.
        prim.Vertices =
        {
            Vertex2d
            {
                pos0,
                color0
            },
            Vertex2d
            {
                pos1,
                color1,
            }
        };

        return prim;
    }

    Primitive2d Primitive2d::CreateLine2d(const Vector2& pos0, const Vector2& pos1, const Color& color, float depth)
    {
        return CreateLine2d(pos0, pos1, color, color, depth);
    }

    Primitive2d Primitive2d::CreateTriangle2d(const Vector2& pos, const Vector2& size, const Color& color0, const Color& color1, const Color& color2,
                                              float depth, const std::string& texId, const Vector4& uvs)
    {
        // Create primitive.
        auto prim      = Primitive2d();
        prim.Depth     = depth;
        //prim.TextureId = texId;

        // Define positions.
        auto halfSize = size / 2.0f;
        auto pos0     = pos - halfSize;
        auto pos1     = pos + Vector2(halfSize.x, -halfSize.y);
        auto pos2     = pos + halfSize;

        // Define UVs.
        auto uv0 = Vector2(uvs.x, uvs.y);
        auto uv1 = Vector2(uvs.x + uvs.z, uvs.y);
        auto uv2 = Vector2(uvs.x + uvs.z, uvs.y + uvs.w);

        // Define vertices.
        prim.Vertices =
        {
            Vertex2d
            {
                pos0,
                color0,
                uv0
            },
            Vertex2d
            {
                pos1,
                color1,
                uv1
            },
            Vertex2d
            {
                pos2,
                color2,
                uv2
            }
        };

        return prim;
    }

    Primitive2d Primitive2d::CreateTriangle2d(const Vector2& pos, const Vector2& size, const Color& color,
                                              float depth, const std::string& texId, const Vector4& uvs)
    {
        return CreateTriangle2d(pos, size, color, color, color, depth, texId, uvs);
    }

    Primitive2d Primitive2d::CreateQuad2d(const Vector2& pos, const Vector2& size, const Color& color0, const Color& color1, const Color& color2, const Color& color3,
                                          float depth, const std::string& texId, const Vector4& uvs)
    {
        // Create primitive.
        auto prim      = Primitive2d();
        prim.Depth     = depth;
        //prim.TextureId = texId;

        // Define positions.
        auto halfSize = size / 2.0f;
        auto pos0     = pos - halfSize;
        auto pos1     = pos + Vector2(halfSize.x, -halfSize.y);
        auto pos2     = pos + halfSize;
        auto pos3     = pos + Vector2(-halfSize.x, halfSize.y);

        // Define UVs.
        auto  uv0 = Vector2(uvs.x, uvs.y);
        auto  uv1 = Vector2(uvs.x + uvs.z, uvs.y);
        auto  uv2 = Vector2(uvs.x + uvs.z, uvs.y + uvs.w);
        auto  uv3 = Vector2(uvs.x, uvs.y + uvs.w);

        // Define vertices.
        prim.Vertices =
        {
            Vertex2d
            {
                pos0,
                color0,
                uv0
            },
            Vertex2d
            {
                pos1,
                color1,
                uv1
            },
            Vertex2d
            {
                pos2,
                color2,
                uv2
            },
            Vertex2d
            {
                pos3,
                color3,
                uv3
            }
        };

        return prim;
    }

    Primitive2d Primitive2d::CreateQuad2d(const Vector2& pos, const Vector2& size, const Color& color,
                                          float depth, const std::string& texId, const Vector4& uvs)
    {
        return CreateQuad2d(pos, size, color, color, color, color, depth, texId, uvs);
    }
}
