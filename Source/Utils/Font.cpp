#include "Framework.h"
#include "Utils/Font.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include "Utils/Utils.h"

namespace Silent::Utils
{
    Font::Font(FT_Library& fontLib, const std::filesystem::path& path, int pointSize, const std::string& precacheGlyphs)
    {
        constexpr int POINT_SIZE_MAX = ATLAS_SIZE / 8;

        _name = path.filename().string();

        // Load FreeType and HarfBuzz data.
        if (FT_New_Face(fontLib, path.string().c_str(), 0, &_ftFace))
        {
            throw std::runtime_error("Failed to initialize FreeFont font face.");
        }
        _hbFont = hb_ft_font_create(_ftFace, nullptr);

        // Clamp point size.
        if (pointSize > POINT_SIZE_MAX)
        {
            Debug::Log(fmt::format("Attempted to initialize font `{}` with invalid point size {}. Max is {}.", _name, pointSize, POINT_SIZE_MAX),
                       Debug::LogLevel::Warning);

            pointSize = std::min<int>(pointSize, POINT_SIZE_MAX);
        }

        // Set point size.
        if (FT_Set_Pixel_Sizes(_ftFace, 0, pointSize))
        {
            throw std::runtime_error("Failed to set font point size.");
        }

        // Set scale factor.
        _scaleFactor = (float)pointSize / (float)_ftFace->size->metrics.x_ppem;

        // Add first atlas.
        AddAtlas();

        // Precache glyphs.
        auto codePoints = GetCodePoints(precacheGlyphs);
        for (char32 codePoint : codePoints)
        {
            CacheGlyph(codePoint);
        }

        // Debug.
        stbi_write_png((g_App.GetFilesystem().GetAppDirectory() / (_name + "_Atlas.png")).string().c_str(), ATLAS_SIZE, ATLAS_SIZE, 1, _atlases.front().data(), ATLAS_SIZE);
    }

    Font::~Font()
    {
        hb_font_destroy(_hbFont);
        FT_Done_Face(_ftFace);
    }

    int Font::GetPointSize() const
    {
        return _pointSize;
    }

    const std::vector<std::vector<byte>>& Font::GetAtlases() const
    {
        return _atlases;
    }

    ShapedText Font::GetShapedText(const std::string& msg)
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

        // Add text to buffer.
        auto* buffer = hb_buffer_create();
        if (!hb_buffer_allocation_successful(buffer))
        {
            Debug::Log(fmt::format("Failed to get shaped glyphs for message `{}", msg), Debug::LogLevel::Error);
            return {};
        }
        hb_buffer_add_utf8(buffer, msg.c_str(), msg.size(), 0, msg.size());

        // @todo Extend this later to support right-to-left scripts.
        // Set text direction and script.
        hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);                       // Left-to-right text.
        hb_buffer_set_script(buffer, HB_SCRIPT_LATIN);                           // Latin script.
        hb_buffer_set_language(buffer, hb_language_from_string("en", NO_VALUE)); // English language.

        // Retrieve shaped glyphs and positions.
        hb_shape(_hbFont, buffer, nullptr, 0);
        uint  glyphCount     = 0;
        auto* glyphInfos     = hb_buffer_get_glyph_infos(buffer, &glyphCount);
        auto* glyphPositions = hb_buffer_get_glyph_positions(buffer, &glyphCount);

        // Build shaped text.
        auto shapedText = ShapedText{};
        shapedText.Glyphs.reserve(glyphCount);
        for (int i = 0; i < glyphCount; i++)
        {
            const auto& glyphInfo = glyphInfos[i];
            const auto& glyphPos  = glyphPositions[i];
            const auto& glyph     = _glyphs[glyphInfo.codepoint];

            shapedText.Glyphs.push_back(ShapedGlyph
            {
                .Metadata = glyph,
                .Advance  = Vector2i(glyphPos.x_advance, glyphPos.y_advance) * _scaleFactor,
                .Offset   = Vector2i(glyphPos.x_offset,  glyphPos.y_offset)  * _scaleFactor
            });
            shapedText.Width += shapedText.Glyphs.back().Advance.x;
        }

        // Free resources and return shaped text.
        hb_buffer_destroy(buffer);
        return shapedText;
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
            Debug::Log(fmt::format("Active atlas {} for font `{}` is full. Creating new atlas.", _activeAtlasIdx, _name), Debug::LogLevel::Info);

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
            .Size      = size
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
        _rectPacks.push_back(PackedRects(rectpack2D::rect_wh(ATLAS_SIZE, ATLAS_SIZE)));
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
            Debug::Log(fmt::format("Attempted to get missing font `{}`.", fontName), Debug::LogLevel::Warning);
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
            return;
        }

        // Handle load.
        try
        {
            _fonts[fontName] = Font(_library, fontPath, pointSize, glyphPrecache);

            Debug::Log(fmt::format("Loaded font `{}` at point size {}.", fontName, pointSize));
        }
        catch (const std::runtime_error& ex)
        {
            Debug::Log(fmt::format("Failed to load font `{}`: {}", fontName, ex.what()), Debug::LogLevel::Error);
        }
    }
}
