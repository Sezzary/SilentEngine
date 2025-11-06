#include "Framework.h"
#include "Utils/Font.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include "Utils/Utils.h"

namespace Silent::Utils
{
    Font::Font(FT_Library& fontLib, const std::string& name, const std::filesystem::path& path, int pointSize, const std::string& precacheGlyphs)
    {
        _name = name;

        if (FT_New_Face(fontLib, path.string().c_str(), 0, &_face))
        {
            Debug::Log("Failed to initialize font face.", Debug::LogLevel::Error);
            return;
        }

        if (FT_Set_Pixel_Sizes(_face, 0, pointSize))
        {
            Debug::Log("Failed to set font point size.", Debug::LogLevel::Error);
            return;
        }

        // Set atlas size.
        _atlas.resize(ATLAS_SIZE * ATLAS_SIZE);

        // Cache precache glyphs.
        auto codePoints = GetCodePoints(precacheGlyphs);
        for (char32 codePoint : codePoints)
        {
            CacheGlyph(codePoint);
        }

        _isLoaded = true;

        // Debug.
        stbi_write_png((g_App.GetFilesystem().GetAppDirectory() / (_name + "_Atlas.png")).string().c_str(), ATLAS_SIZE, ATLAS_SIZE, 1, _atlas.data(), ATLAS_SIZE);
    }

    Font::~Font()
    {
        FT_Done_Face(_face);
    }

    const std::vector<byte>& Font::GetAtlas()
    {
        return _atlas;
    }

    std::vector<Glyph*> Font::GetGlyphs(const std::string& msg)
    {
        // Get code points.
        auto codePoints = GetCodePoints(msg);

        // Collect glyphs.
        auto glyphs = std::vector<Glyph*>{};
        glyphs.reserve(codePoints.size());
        for (char32 codePoint : codePoints)
        {
            // Check if glyph is cached.
            auto it = _glyphs.find(codePoint);
            if (it == _glyphs.end())
            {
                if (!CacheGlyph(codePoint))
                {
                    continue;
                }
            }

            // Add glyph.
            auto& [keyCodePoint, glyph] = *it;
            glyphs.push_back(&glyph);
        }

        return glyphs;
    }

    bool Font::IsLoaded() const
    {
        return _isLoaded;
    }

    std::vector<char32> Font::GetCodePoints(const std::string& str) const
    {
        auto codePoints = std::vector<char32>{};
        utf8::utf8to32(str.begin(), str.end(), std::back_inserter(codePoints));
        return codePoints;
    }

    bool Font::CacheGlyph(char32 codePoint)
    {
        // Load glyph.
        FT_Load_Glyph(_face, FT_Get_Char_Index(_face, codePoint), FT_LOAD_DEFAULT);
        const auto& metrics = _face->glyph->metrics;

        // Pack glyph rectangle.
        auto size = Vector2i(FP_FROM(metrics.width, Q6_SHIFT), FP_FROM(metrics.height, Q6_SHIFT)) + Vector2i(GLYPH_PADDING * 2);
        auto rect = _glyphRects.insert(rectpack2D::rect_wh(size.x, size.y));
        if (!rect.has_value())
        {
            Debug::Log("Resetting `" + _name + "` font glyph atlas.");

            // Clear atlas.
            _glyphs.clear();
            _glyphRects = PackedRects({ ATLAS_SIZE, ATLAS_SIZE });
            _atlas.resize(ATLAS_SIZE * ATLAS_SIZE, 0);

            // Retry packing glyph rectangle.
            auto rect   = _glyphRects.insert(rectpack2D::rect_wh(size.x, size.y));
            if (!rect.has_value())
            {
                Debug::Log("Failed to register glyph with code point " + std::to_string(codePoint) + ".", Debug::LogLevel::Error);
                return false;
            }
        }

        // Register new glyph.
        _glyphs[codePoint] = Glyph
        {
            .CodePoint = codePoint,
            .Position  = Vector2i(rect->x, rect->y) + Vector2i(GLYPH_PADDING),
            .Size      = size,
            .Bearing   = Vector2i(FP_FROM(metrics.horiBearingX, Q6_SHIFT), FP_FROM(metrics.horiBearingY, Q6_SHIFT)),
            .Advance   = (int)metrics.horiAdvance
        };

        const auto& glyph = _glyphs[codePoint];

        // Rasterize.
        FT_Render_Glyph(_face->glyph, FT_RENDER_MODE_NORMAL);
        const auto& bitmap     = _face->glyph->bitmap;
        byte*       pixelsTo   = &_atlas[(glyph.Position.y * ATLAS_SIZE) + glyph.Position.x];
        byte*       pixelsFrom = (byte*)bitmap.buffer;

        // Copy pixels to atlas.
        for (int y = 0; y < bitmap.rows; y++)
        {
            for (int x = 0; x < bitmap.width; x++)
            {
                pixelsTo[(ATLAS_SIZE * y) + x] = pixelsFrom[(bitmap.width * y) + x];
            }
        }

        return true;
    }

    FontManager::FontManager()
    {
        if (FT_Init_FreeType(&_library))
        {
            Debug::Log("Failed to initialize font library.", Debug::LogLevel::Error);
        }
    }

    FontManager::~FontManager()
    {
        _fonts.clear();
        FT_Done_FreeType(_library);
    }

    Font* FontManager::GetFont(const std::string& fontName)
    {
        // Check if font exists.
        auto it = _fonts.find(fontName);
        if (it == _fonts.end())
        {
            Debug::Log("Attempted to get missing font `"+ fontName + "`.", Debug::LogLevel::Warning);
            return nullptr;
        }

        auto& [keyFontName, font] = *it;
        return &font;
    }

    void FontManager::LoadFont(const std::filesystem::path& fontPath, int pointSize, const std::string& glyphPrecache)
    {
        // Check if font is already loaded.
        auto fontName = fontPath.filename().string();
        auto it       = _fonts.find(fontName);
        if (it != _fonts.end())
        {
            Debug::Log("Attempted to load loaded font `"+ fontName + "`.", Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return;
        }

        // Handle load.
        _fonts[fontName] = Font(_library, fontName, fontPath, pointSize, glyphPrecache);
        if (!_fonts[fontName].IsLoaded())
        {
            Debug::Log("Failed to load font `" + fontName + "`.", Debug::LogLevel::Error);
            _fonts.erase(fontName);
            return;
        }

        Debug::Log("Loaded font `" + fontName + "` at point size " + std::to_string(pointSize) + ".");
    }
}
