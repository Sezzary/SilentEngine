#pragma once

namespace Silent::Renderer
{
    enum class BlendMode
    {
        Opaque,
        Alpha,
        FastAlpha,
        Add,
        Subtract,
        Multiply,
        LumaMultiply // @todo Not a blend mode, just a shader.
    };

    enum class ScreenSpriteAlignMode
    {
        Center,
        CenterTop,
        CenterBottom,
        CenterLeft,
        CenterRight,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    enum class ScreenSpriteScaleMode
    {
        Fit,
        Fill,
        Stretch
    };
}
