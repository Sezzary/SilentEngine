#pragma once

#include "Renderer/Common/Objects/Triangle.h"

namespace Silent::Renderer
{
    /** @brief 3D mesh. @todo Should refactor this to use buckets later. */
    struct Mesh
    {
        std::vector<Triangle> Triangles = {};
        std::vector<Vertex3d> Vertices  = {};
    };
}
