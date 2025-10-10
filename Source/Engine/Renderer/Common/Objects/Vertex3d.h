#pragma once

namespace Silent::Renderer
{
    struct Vertex3d
    {
        Vector3 Position = Vector3::Zero;
        Vector3 Normal   = Vector3::Zero;
        Color   Col      = Color::Black;
        Vector2 Uv       = Vector2::Zero;
    };
}
