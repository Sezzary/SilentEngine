#pragma once

#include "Engine/Renderer/Common/Enums.h"

namespace Silent::Renderer
{
    /** @brief 2D screen sprite. */
    struct Sprite2d
    {
        int       AssetIdx = 0;
        Vector2   Position = Vector2::Zero;
        float     Rotation = 0.0f;
        Vector2   Scale    = Vector2::Zero;
        Color     Col      = Color::Black;
        uint      Depth    = 0;
        AlignMode AlignM   = AlignMode::Center;
        ScaleMode ScaleM   = ScaleMode::Fit;
        BlendMode BlendM   = BlendMode::Opaque;

        static Sprite2d CreateSprite2d(int assetIdx, const Vector2& pos, float rot, const Vector2& scale, const Color& color,
                                       uint depth = 0, AlignMode alignMode = AlignMode::Center, ScaleMode scaleMode = ScaleMode::Fit, BlendMode blendMode = BlendMode::Alpha);
    };
}
