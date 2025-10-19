#pragma once

namespace Silent::Renderer
{
    /** @brief 2D screen space vertex. */
    struct Vertex2d
    {
        Vector2 Position = Vector2::Zero;
        Color   Col      = Color::Black;
        Vector2 Uv       = Vector2::Zero;
    };
}
