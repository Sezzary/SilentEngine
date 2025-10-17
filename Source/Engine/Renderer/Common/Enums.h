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
        Multiply
    };

    enum class AlignMode
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

    enum class ScaleMode
    {
        Fit,
        Fill,
        Stretch
    };
}
