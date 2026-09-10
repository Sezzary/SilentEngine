#include "Framework.h"
#include "Renderer/Common/Resources/Scene/Text2d.h"

#include "Application.h"
#include "Renderer/Common/Enums.h"
#include "Utils/Font.h"

using namespace Silent::Utils;

namespace Silent::Renderer
{
    Glyph2d Glyph2d::CreateGlyph2d(const ShapedGlyph& shapedGlyph, bool hasGradient,
                                   const std::string& atlasName, const Vector2& uvMin, const Vector2& uvMax,
                                   const Vector2& pos, float rot, const Vector2& scale, const Color& color,
                                   int depth)
    {
        return Glyph2d
        {
            .AtlasName      = atlasName,
            .UvMin          = uvMin,
            .UvMax          = uvMax,
            .Position       = pos,
            .Rotation       = rot,
            .Scale          = scale,
            .Col            = color,
            .Depth          = depth,
            .HasGradient    = hasGradient,
            .GradientUvMinY = Remap(shapedGlyph.Attribs.Ascender,
                                    shapedGlyph.Attribs.MinY, shapedGlyph.Attribs.MaxY,
                                    uvMax.y, uvMin.y),
            .GradientUvMaxY = Remap(0.0f,
                                    shapedGlyph.Attribs.MinY, shapedGlyph.Attribs.MaxY,
                                    uvMax.y, uvMin.y)
        };
    }

    Text2d Text2d::CreateText2d(const std::string& msg, const std::string& fontName,
                                const Vector2& pos, float rot, float scale,
                                const Color& color, int styleFlags,
                                int depth, AlignMode alignMode)
    {
        auto& fonts = g_App.GetFonts();

        // Get font.
        auto* font = fonts.GetFont(fontName);
        if (font == nullptr)
        {
            Debug::Log(Fmt("Attempted to create 2D text using missing font `{}`.", fontName),
                       Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return {};
        }

        // Get shaped text glyphs.
        return Text2d
        {
            .Shape      = font->GetShapedText(msg),
            .Message    = msg,
            .Font       = font,
            .Position   = pos,
            .Rotation   = rot,
            .Scale      = scale,
            .Col        = color,
            .StyleFlags = styleFlags,
            .Depth      = depth,
            .AlignMd    = alignMode
        };
    }
}
