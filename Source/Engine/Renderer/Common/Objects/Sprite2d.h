#pragma once

#include "Engine/Renderer/Common/Enums.h"
#include "Engine/Renderer/Common/Objects/Vertex2d.h"

namespace Silent::Renderer
{
    // @todo Can use primitive internally.
    struct Sprite2d
    {
        int                                     AssetIdx = 0;
        std::array<Vertex2d, QUAD_VERTEX_COUNT> Vertices = {};
        float                                   Rotation = 0.0f;
        Vector2                                 Scale    = Vector3::Zero;
        Color                                   Col      = Color::Black;
        int                                     Depth    = 0;
        BlendMode                               BlendM   = BlendMode::Opaque;
        ScaleMode                               ScaleM   = ScaleMode::Fit;

        static Sprite2d CreateSprite2d(int assetIdx, const Vector2& pos, float rot, const Vector2& scale, const Color& color,
                                       int depth, BlendMode blendMode, ScaleMode scaleMode);

        static Sprite2d CreateSprite2d(const Vector2i& pos);
    };
}
