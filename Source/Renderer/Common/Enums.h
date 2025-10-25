#pragma once

namespace Silent::Renderer
{
    /** @brief Render stages representing pipelines or shader programs depending on the renderer backend. */
    enum class RenderStage
    {
        /** Material */

        Primitive2d,
        Primitive2dTextured,

        /** Post-process */

        Vignette,

        Count
    };

    /** @brief Render stage blending modes. */
    enum class BlendMode
    {
        Opaque,
        Alpha,
        FastAlpha,
        Multiply,
        Add,
        Subtract,
        Wireframe,

        Count
    };

    /** @brief 2D sprite and shape align modes. */
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

    /** @brief 2D sprite and shape scale modes relative to the display aspect ratio. */
    enum class ScaleMode
    {
        Fit,
        Fill,
        Stretch
    };
}
