#pragma once

#include "Engine/Renderer/Common/Enums.h"
#include "Engine/Renderer/Common/Objects/Primitive2d.h"

namespace Silent::Renderer
{
    // @todo Can use primitive internally.
    struct Sprite2d
    {
        Primitive2d Primitive = {};
        int         AssetIdx  = 0;

        static Sprite2d CreateSprite2d(int assetIdx, const Vector2& pos, float rot, const Vector2& scale, const Color& color,
                                       int depth, BlendMode blendMode, ScaleMode scaleMode);

        static Sprite2d CreateSprite2d(const Vector2i& pos);
    };
}
