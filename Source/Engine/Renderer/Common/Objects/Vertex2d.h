#pragma once

namespace Silent::Renderer
{
    struct Vertex2d
    {
        Vector2 Position = Vector2::Zero;
        Color   Col      = Color::Black;
        Vector2 Uv       = Vector2::Zero;
    };
}
