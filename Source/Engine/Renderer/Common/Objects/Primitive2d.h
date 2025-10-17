#pragma once

#include "Engine/Renderer/Common/Enums.h"
#include "Engine/Renderer/Common/Objects/Primitive/Vertex2d.h"

namespace Silent::Renderer
{
    constexpr uint                                       DEPTH_MAX          = 1024;
    constexpr std::array<int, TRIANGLE_VERTEX_COUNT * 2> QUAD_TRIANGLE_IDXS = { 0, 1, 2, 0, 2, 3 };

    /** @brief 2D screen space primitive representing a line, triangle, or quad. */
    struct Primitive2d
    {
        std::vector<Vertex2d> Vertices = {};
        uint                  Depth    = 0;
        ScaleMode             ScaleM   = ScaleMode::Fit;
        BlendMode             BlendM   = BlendMode::Alpha;

        /** @brief Constructs a 2D line primitive with a width at the retro resolution pixel scale (320x240) using screen positions in percent.
         *
         * @param from Start position in screen percent.
         * @param to End position in screen percent.
         * @param colorFrom Start color.
         * @param colorTo End color.
         * @param depth Render priority.
         * @return 2D line primitive.
         */
        static Primitive2d CreateLine(const Vector2& from, const Vector2& to,
                                      const Color& colorFrom, const Color& colorTo,
                                      uint depth = 0, ScaleMode scaleMode = ScaleMode::Fit, BlendMode blendMode = BlendMode::Alpha);

        /** @brief Constructs a 2D line primitive with a width at the retro resolution pixel scale (320x240) using retro screen coordinates.
         *
         * @param from Start position in retro screen coordinates (320x240).
         * @param to End position in retro screen coordinates (320x240).
         * @param colorFrom Start color.
         * @param colorTo End color.
         * @param depth Render priority.
         * @return 2D line primitive.
         */
        static Primitive2d CreateLine(const Vector2i& from, const Vector2i& to,
                                      const Color& colorFrom, const Color& colorTo,
                                      uint depth = 0, ScaleMode scaleMode = ScaleMode::Fit, BlendMode blendMode = BlendMode::Alpha);

        /** @brief Constructs a 2D triangle primitive using screen positions in percent.
         *
         * @param vert0 First vertex position in screen percent.
         * @param vert1 Second vertex position in screen percent.
         * @param vert2 Third vertex position in screen percent.
         * @param color0 First vertex color.
         * @param color1 Second vertex color.
         * @param color2 Third vertex color.
         * @param depth Render priority.
         * @return 2D triangle primitive.
         */
        static Primitive2d CreateTriangle(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2,
                                          const Color& color0, const Color& color1, const Color& color2,
                                          uint depth = 0, ScaleMode scaleMode = ScaleMode::Fit, BlendMode blendMode = BlendMode::Alpha);

        /** @brief Constructs a 2D triangle primitive using retro screen coordinates.
         *
         * @param vert0 First vertex position in retro screen coordinates (320x240).
         * @param vert1 Second vertex position in retro screen coordinates (320x240).
         * @param vert2 Third vertex position in retro screen coordinates (320x240).
         * @param color0 First vertex color.
         * @param color1 Second vertex color.
         * @param color2 Third vertex color.
         * @param depth Render priority.
         * @return 2D triangle primitive.
         */
        static Primitive2d CreateTriangle(const Vector2i& vert0, const Vector2i& vert1, const Vector2i& vert2,
                                          const Color& color0, const Color& color1, const Color& color2,
                                          uint depth = 0, ScaleMode scaleMode = ScaleMode::Fit, BlendMode blendMode = BlendMode::Alpha);

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
         * @return 2D quad primitive.
         */
        static Primitive2d CreateQuad(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2, const Vector2& vert3,
                                      const Color& color0, const Color& color1, const Color& color2, const Color& color3,
                                      uint depth = 0, ScaleMode scaleMode = ScaleMode::Fit, BlendMode blendMode = BlendMode::Alpha);

        /** @brief Constructs a 2D quad primitive.
         *
         * @param vert0 First vertex position in retro screen coordinates (320x240).
         * @param vert1 Second vertex position in retro screen coordinates (320x240).
         * @param vert2 Third vertex position in retro screen coordinates (320x240).
         * @param vert3 Fourth vertex position in retro screen coordinates (320x240).
         * @param color0 First vertex color.
         * @param color1 Second vertex color.
         * @param color2 Third vertex color.
         * @param color3 Fourth vertex color.
         * @param depth Render priority.
         * @return 2D quad primitive.
         */
        static Primitive2d CreateQuad(const Vector2i& vert0, const Vector2i& vert1, const Vector2i& vert2, const Vector2i& vert3,
                                      const Color& color0, const Color& color1, const Color& color2, const Color& color3,
                                      uint depth = 0, ScaleMode scaleMode = ScaleMode::Fit, BlendMode blendMode = BlendMode::Alpha);
    };
}
