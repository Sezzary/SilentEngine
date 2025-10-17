#pragma once

namespace Silent::Renderer
{
    /** @brief 2D or 3D triangle. */
    struct Triangle
    {
        constexpr uint VERTEX_COUNT = 3;

        std::array<uint, VERTEX_COUNT> VertexIdxs = {};
    };
}
