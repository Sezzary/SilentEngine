#pragma once

#include "Engine/Renderer/Common/Objects/Vertex2d.h"

namespace Silent::Renderer
{
    constexpr uint                                       DEPTH_MAX             = 1024;
    constexpr uint                                       TRIANGLE_VERTEX_COUNT = 3;
    constexpr uint                                       QUAD_VERTEX_COUNT     = 4;
    constexpr std::array<int, TRIANGLE_VERTEX_COUNT * 2> QUAD_TRIANGLE_IDXS    = { 0, 1, 2, 0, 2, 3 };

    struct Primitive2d
    {
        std::vector<Vertex2d> Vertices  = {};
        int                   Depth     = 0;

        /** @brief Constructs a 2D line primitive with a width corresponding to the pixel scale at the retro resolution (320x240).
         *
         * @param from Start position in screen percent.
         * @param to End position in screen percent.
         * @param colorFrom Start color.
         * @param colorTo End color.
         * @param depth Render priority.
         * @return 2D line primitive
         */
        static Primitive2d CreateLine(const Vector2& from, const Vector2& to, const Color& colorFrom, const Color& colorTo, int depth);

        /** @brief Constructs a 2D triangle primitive.
         *
         * @param vert0 First vertex position in screen percent.
         * @param vert1 Second vertex position in screen percent.
         * @param vert2 Third vertex position in screen percent.
         * @param color0 First vertex color.
         * @param color1 Second vertex color.
         * @param color2 Third vertex color.
         * @param depth Render priority.
         */
        static Primitive2d CreateTriangle(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2,
                                          const Color& color0, const Color& color1, const Color& color2, int depth);

        /** @brief Constructs a 2D quad primitive.
         *
         * @param vert0 First vertex position in screen percent.
         * @param vert1 Second vertex position in screen percent.
         * @param vert2 Third vertex position in screen percent.
         * @param vert3 Fourth vertex position in screen percent.
         * @param color0 First vertex color.
         * @param color1 Second vertex color.
         * @param color2 Third vertex color.
         * @param color3 Fourth vertex color.
         * @param depth Render priority.
         */
        static Primitive2d CreateQuad(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2, const Vector2& vert3,
                                      const Color& color0, const Color& color1, const Color& color2, const Color& color3, int depth);
    };
}
