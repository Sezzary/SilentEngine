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

        // Load FreeType and HarfBuzz data.
        if (FT_New_Face(fontLib, path.string().c_str(), 0, &_ftFace))
        {
            Debug::Log("Failed to initialize font face.", Debug::LogLevel::Error);
            return;
        }
        _hbFont = hb_ft_font_create_referenced(_ftFace);

        if (FT_Set_Pixel_Sizes(_ftFace, 0, std::min<int>(pointSize, ATLAS_SIZE / 4)))
        {
            Debug::Log("Failed to set font point size.", Debug::LogLevel::Error);
            return;
        }

        // Add first atlas.
        AddAtlas();

        // Precache glyphs.
        auto codePoints = GetCodePoints(precacheGlyphs);
        for (char32 codePoint : codePoints)
        {
            CacheGlyph(codePoint);
        }

        _isLoaded = true;

        // Debug.
        //stbi_write_png((g_App.GetFilesystem().GetAppDirectory() / (_name + "_Atlas.png")).string().c_str(), ATLAS_SIZE, ATLAS_SIZE, 1, _atlases.front().data(), ATLAS_SIZE);
    }

    Font::~Font()
    {
        hb_font_destroy(_hbFont);
        FT_Done_Face(_ftFace);
    }

    const std::vector<std::vector<byte>>& Font::GetAtlases()
    {
        return _atlases;
    }

    std::vector<ShapedGlyph> Font::GetShapedGlyphs(const std::string& msg)
    {
        // Cache new glyphs.
        auto codePoints = GetCodePoints(msg);
        for (char32 codePoint : codePoints)
        {
            auto it = _glyphs.find(codePoint);
            if (it == _glyphs.end())
            {
                CacheGlyph(codePoint);
            }
        }

        // Add string to buffer.
        auto* buffer = hb_buffer_create();
        hb_buffer_add_utf8(buffer, msg.c_str(), msg.size(), 0, msg.size());

        // @todo Extend this later.
        // Set text direction and script.
        hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);                       // Left-to-right text.
        hb_buffer_set_script(buffer, HB_SCRIPT_LATIN);                           // Latin script.
        hb_buffer_set_language(buffer, hb_language_from_string("en", NO_VALUE)); // English language.

        // Retrieve shaped glyphs and positions.
        hb_shape(_hbFont, buffer, nullptr, 0);
        uint  glyphCount     = 0;
        auto* glyphInfos     = hb_buffer_get_glyph_infos(buffer, &glyphCount);
        auto* glyphPositions = hb_buffer_get_glyph_positions(buffer, &glyphCount);

        // Collect shaped glyphs.
        auto shapedGlyphs = std::vector<ShapedGlyph>{};
        shapedGlyphs.reserve(codePoints.size());
        for (int i = 0; i < glyphCount; i++)
        {
            const auto& glyphInfo = glyphInfos[i];
            const auto& glyphPos  = glyphPositions[i];
            const auto& glyph     = _glyphs[glyphInfo.codepoint];

            shapedGlyphs.push_back(ShapedGlyph
            {
                .Metadata = glyph,
                .Offset   = Vector2i(glyphPos.x_offset, glyphPos.y_offset)
            });
        }

        // Free resources and return shaped glyphs.
        hb_buffer_destroy(buffer);
        return shapedGlyphs;
    }

    bool Font::IsLoaded() const
    {
        return _isLoaded;
    }

    std::vector<char32> Font::GetCodePoints(const std::string& str) const
    {
        // Reserve minimum size.
        auto codePoints = std::vector<char32>{};
        codePoints.reserve((str.size() / 4) + 1);

        // Collect code points.
        utf8::utf8to32(str.begin(), str.end(), std::back_inserter(codePoints));
        return codePoints;
    }

    void Font::CacheGlyph(char32 codePoint)
    {
        // Load glyph.
        FT_Load_Glyph(_ftFace, FT_Get_Char_Index(_ftFace, codePoint), FT_LOAD_DEFAULT);
        const auto& metrics = _ftFace->glyph->metrics;

        // Pack glyph rectangle.
        auto size = Vector2i(FP_FROM(metrics.width, Q6_SHIFT), FP_FROM(metrics.height, Q6_SHIFT)) + Vector2i(GLYPH_PADDING * 2);
        auto rect = _rectPacks.back().insert(rectpack2D::rect_wh(size.x, size.y));
        if (!rect.has_value())
        {
            Debug::Log("Active atlas " + std::to_string(_activeAtlasIdx) + " for font `" + _name + "` is full. Creating new atlas.",
                       Debug::LogLevel::Info, Debug::LogMode::Debug);

            // Start new atlas.
            AddAtlas();
            rect = _rectPacks.back().insert(rectpack2D::rect_wh(size.x, size.y));
        }

        // Register new glyph.
        _glyphs[codePoint] = GlyphMetadata
        {
            .CodePoint = codePoint,
            .AtlasIdx  = _activeAtlasIdx,
            .Position  = Vector2i(rect->x, rect->y) + Vector2i(GLYPH_PADDING),
            .Size      = size,
            .Bearing   = Vector2i(FP_FROM(metrics.horiBearingX, Q6_SHIFT), FP_FROM(metrics.horiBearingY, Q6_SHIFT)),
            .Advance   = (int)metrics.horiAdvance
        };
        const auto& glyph = _glyphs[codePoint];

        // Rasterize.
        FT_Render_Glyph(_ftFace->glyph, FT_RENDER_MODE_NORMAL);
        const auto& bitmap     = _ftFace->glyph->bitmap;
        byte*       pixelsTo   = &_atlases.back()[(glyph.Position.y * ATLAS_SIZE) + glyph.Position.x];
        byte*       pixelsFrom = (byte*)bitmap.buffer;

        // Copy pixels to atlas.
        for (int y = 0; y < bitmap.rows; y++)
        {
            for (int x = 0; x < bitmap.width; x++)
            {
                pixelsTo[(ATLAS_SIZE * y) + x] = pixelsFrom[(bitmap.width * y) + x];
            }
        }
    }

    void Font::AddAtlas()
    {
        _rectPacks.push_back(PackedRects({ ATLAS_SIZE, ATLAS_SIZE }));
        _atlases.push_back(std::vector<byte>(ATLAS_SIZE * ATLAS_SIZE));
        _activeAtlasIdx++;
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
