#include "Framework.h"
#include "Renderer/Common/Objects/Primitive3d.h"

#include "Renderer/Common/Constants.h"
#include "Renderer/Common/Enums.h"
#include "Renderer/Common/Objects/Primitive/Vertex3d.h"

namespace Silent::Renderer
{
    Primitive3d Primitive3d::CreateDebugLine(const Vector3& from, const Vector3& to, const Color& color)
    {
        return Primitive3d
        {
            .Vertices =
            {
                {
                    .Position = from,
                    .Col      = color
                },
                {
                    .Position = to,
                    .Col      = color
                }
            },
            .BlendM = BlendMode::Add
        };
    }

    Primitive3d Primitive3d::CreateDebugTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2, const Color& color)
    {
        return Primitive3d
        {
            .Vertices =
            {
                {
                    .Position = vert0,
                    .Col      = color
                },
                {
                    .Position = vert1,
                    .Col      = color
                },
                {
                    .Position = vert2,
                    .Col      = color
                }
            },
            .BlendM = BlendMode::Add
        };
    }
}
