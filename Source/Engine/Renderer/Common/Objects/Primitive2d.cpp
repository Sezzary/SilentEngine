#include "Framework.h"
#include "Engine/Renderer/Common/Objects/Primitive2d.h"

#include "Engine/Renderer/Common/Objects/Vertex2d.h"
#include "Engine/Renderer/Renderer.h"

namespace Silent::Renderer
{
    Primitive2d Primitive2d::CreateLine(const Vector2& from, const Vector2& to, const Color& colorFrom, const Color& colorTo, int depth)
    {
        constexpr float WIDTH = SCREEN_SPACE_RES.y / RETRO_SCREEN_SPACE_RES.y;

        float rot    = glm::atan2(to.y - from.y, to.x - from.x);
        auto  dir    = Vector2::Normalize(to - from);
        auto  offset = Vector2(-dir.y, dir.x) * WIDTH;

        return Primitive2d
        {
            .Vertices =
            {
                Vertex2d{ from,          colorFrom },
                Vertex2d{ to,            colorTo   },
                Vertex2d{ from + offset, colorFrom },
                Vertex2d{ to   + offset, colorTo   }
            },
            .Depth = depth
        };
    }

    Primitive2d Primitive2d::CreateTriangle(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2,
                                            const Color& color0, const Color& color1, const Color& color2, int depth)
    {
        return Primitive2d
        {
            .Vertices =
            {
                Vertex2d{ vert0, color0 },
                Vertex2d{ vert1, color1 },
                Vertex2d{ vert2, color2 }
            },
            .Depth = depth
        };
    }

    Primitive2d Primitive2d::CreateQuad(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2, const Vector2& vert3,
                                        const Color& color0, const Color& color1, const Color& color2, const Color& color3, int depth)
    {
        return Primitive2d
        {
            .Vertices =
            {
                Vertex2d{ vert0, color0 },
                Vertex2d{ vert1, color1 },
                Vertex2d{ vert2, color2 },
                Vertex2d{ vert3, color3 }
            },
            .Depth = depth
        };
    }
}
