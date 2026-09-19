#pragma once

#include "Renderer/Common/Resources/Scene/Sprite2d.h"
#include "Renderer/Common/Enums.h"
#include "Utils/Font.h"

using namespace Silent::Utils;

namespace Silent::Renderer
{
    constexpr int  GLYPH_2D_COUNT_MAX = 1024;
    constexpr auto GLYPH_SCALE_MODE   = ScaleMode::VerticalEdge;

    /** @brief Text embellishment style flags. */
    enum class TextStyleFlags
    {
        None       = 0,
        Gradient   = 1 << 0,
        Shadow     = 1 << 1,
        HalfHeight = 1 << 2
    };

    /** @brief 2D screen glyph. */
    struct Glyph2d
    {
        std::string AtlasName = {};
        Vector2     UvMin     = Vector2::Zero;
        Vector2     UvMax     = Vector2::Zero;
        Vector2     Position  = Vector2::Zero;
        float       Rotation  = 0.0f;
        Vector2     Scale     = Vector2::Zero;
        Color       Col       = Color::Clear;
        int         Depth     = 0;

        bool  HasGradient    = false;
        float GradientUvMinY = 0.0f;
        float GradientUvMaxY = 0.0f;

        /** @brief Creates a 2D screen glyph.
         *
         * @param shadedGlyph Shaped glyph with spacing parameters.
         * @param hasGradient Has vertical center gradient.
         * @param atlasName Name of the texture atlas containing the rasterized glyph. @todo For now, always the first one.
         * @param uvMin Lower texture atlas UV bound.
         * @param uvMax Upper texture atlas UV bound.
         * @param pos Position in screen percent.
         * @param rot Rotation in radians.
         * @param scale Scale.
         * @param color Tint color.
         * @param depth Glyph layer render priority.
         * @return 2D glyph.
         */
        static Glyph2d CreateGlyph2d(const ShapedGlyph& shapedGlyph, bool hasGradient,
                                     const std::string& atlasName, const Vector2& uvMin, const Vector2& uvMax,
                                     const Vector2& pos, float rot, const Vector2& scale, const Color& color,
                                     int depth = 0);
    };

    /** @brief 2D screen text. */
    struct Text2d
    {
        ShapedText  Shape      = {};
        std::string Message    = {};
        Font*       Font       = nullptr;
        Vector2     Position   = Vector2::Zero;
        float       Rotation   = 0.0f;
        float       Scale      = 0.0f;
        Color       Col        = Color::White;
        int         StyleFlags = (int)TextStyleFlags::None;
        int         Depth      = 0;
        AlignMode   AlignMd    = AlignMode::Center;

        /** @brief Creates a shaped 2D text message.
         *
         * @param msg Message string.
         * @param fontName Font chain name to use for glyphs.
         * @param pos Screen position in percent.
         * @param rot Rotation in radians.
         * @param scale Scale relative to the screen height.
         * @param color Glyph tint color.
         * @param styleFlags Style flags.
         * @param depth Glyph layer render priority.
         * @param alignMode Alignment mode.
         * @return Shaped 2D text.
         */
        static Text2d CreateText2d(const std::string& msg, const std::string& fontName,
                                   const Vector2& pos, float rot, float scale,
                                   const Color& color, int styleFlags,
                                   int depth = 0, AlignMode alignMode = AlignMode::Center);
    };
}
