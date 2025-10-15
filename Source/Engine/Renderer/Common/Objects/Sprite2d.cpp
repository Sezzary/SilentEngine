#include "Framework.h"
#include "Engine/Renderer/Common/Objects/Sprite2d.h"

#include "Engine/Renderer/Common/Constants.h"
#include "Engine/Renderer/Common/Enums.h"
#include "Engine/Renderer/Common/Objects/Primitive2d.h"
#include "Engine/Renderer/Common/Utils.h"

namespace Silent::Renderer
{
    
    Sprite2d Sprite2d::CreateSprite2d(int assetIdx, const Vector2& pos, float rot, const Vector2& scale, const Color& color,
                                      uint depth, AlignMode alignMode, ScaleMode scaleMode, BlendMode blendMode)
    {
        return Sprite2d
        {
            .AssetIdx = assetIdx,
            .Position = pos,
            .Rotation = rot,
            .Scale    = scale,
            .Col      = color,
            .Depth    = depth,
            .AlignM   = alignMode,
            .ScaleM   = scaleMode,
            .BlendM   = blendMode,
        };
    }
}
