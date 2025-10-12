#pragma once

#include "Engine/Renderer/Common/Enums.h"
#include "Engine/Renderer/Common/Objects/Vertex2d.h"

namespace Silent::Renderer
{
    class Sprite2d
    {
        std::array<Vertex2d, QUAD_VERTEX_COUNT> Vertices  = {};
        Color                                   Col       = Color::Black;
        int                                     Depth     = 0;
        BlendMode                               BlendMode = BlendMode::Opaque;
    };
}
