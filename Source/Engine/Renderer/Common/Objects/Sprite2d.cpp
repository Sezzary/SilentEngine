#include "Framework.h"
#include "Engine/Renderer/Common/Objects/Sprite2d.h"

#include "Engine/Renderer/Common/Enums.h"
#include "Engine/Renderer/Common/Objects/Vertex2d.h"

namespace Silent::Renderer
{
    Sprite2d Sprite2d::CreateSprite2d(int assetIdx, const Vector2& pos, float rot, const Vector2& scale, const Color& color,
                                             int depth, BlendMode blendMode, ScaleMode scaleMode)
    {
        return Sprite2d
        {
            .AssetIdx = assetIdx,
            .Vertices =
            {

            },
            .Rotation = rot,
            .Scale    = scale,
            .Col      = color,
            .Depth    = depth,
            .BlendM   = blendMode,
            .ScaleM   = scaleMode
        };
    }

    /*Sprite2d Sprite2d::CreateSprite2d(const Vector2i& pos)
    {
        return CreateSprite2d(pos );
    }*/
}
