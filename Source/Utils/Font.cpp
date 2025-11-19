#include "Framework.h"
#include "Utils/Font.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include "Utils/Utils.h"

namespace Silent::Utils
{
    /** @brief HarfBuzz text shaping data. */
    struct ShapingInfo
    {
        hb_glyph_info_t*     Glyphs    = nullptr;
        hb_glyph_position_t* Positions = nullptr;
        hb_buffer_t*         Buffer    = nullptr;
    };

    Font::Font(FT_Library& fontLib, const std::string& name, const std::vector<std::string>& filenames, const std::filesystem::path& path, int pointSize,
               const std::string& precacheGlyphs)
    {
        constexpr int POINT_SIZE_MAX = ATLAS_SIZE / 8;

        _name      = name;
        _fontCount = filenames.size();

        // Clamp point size.
        if (pointSize > POINT_SIZE_MAX)
        {
            Debug::Log(Fmt("Attempted to initialize font `{}` with invalid point size {}. Max is {}.", _name, pointSize, POINT_SIZE_MAX),
                       Debug::LogLevel::Warning);

            pointSize = std::min<int>(pointSize, POINT_SIZE_MAX);
        }

        // Add chained fonts to library.
        for (const auto& filename : filenames)
        {
            FT_Face ftFont = nullptr;
            if (FT_New_Face(fontLib, (path / filename).string().c_str(), 0, &ftFont))
            {
                throw std::runtime_error("Failed to initialize font.");
            }

            _ftFonts.push_back(ftFont);
            _hbFonts.push_back(hb_ft_font_create(ftFont, nullptr));

            // Set point size.
            if (FT_Set_Pixel_Sizes(ftFont, 0, pointSize))
            {
                throw std::runtime_error("Failed to set font point size.");
            }
        }
        Debug::Assert(_ftFonts.size() == _fontCount && _hbFonts.size() == _fontCount, Fmt("Invalid initialization for font `{}`.", name));

        // Set scale factor.
        _scaleFactor = (float)pointSize / (float)_ftFonts.front()->size->metrics.x_ppem;

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
        for (auto& ftFont : _ftFonts)
        {
            FT_Done_Face(ftFont);
        }

        for (auto& hbFont : _hbFonts)
        {
            hb_font_destroy(hbFont);
        }
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
            if (Find(_glyphs, codePoint) == nullptr)
            {
                CacheGlyph(codePoint);
            }
        }

        auto shapingInfos = std::vector<ShapingInfo>(_fontCount);
        auto shapedText   = ShapedText{};
        shapedText.Glyphs.reserve(codePoints.size());

        // Build shaped text.
        for (int i = 0; i < codePoints.size(); i++)
        {
            // Run through font chain.
            for (int j = 0; j < _fontCount; j++)
            {
                // Check if glyph is valid.
                uint charIdx = FT_Get_Char_Index(_ftFonts[j], codePoints[i]);
                if (charIdx == 0)
                {
                    // If no valid glyphs exist, use first font's invalid glyph.
                    if (j < (_fontCount - 1))
                    {
                        continue;
                    }
                    else
                    {
                        j = 0;
                    }
                }

                // Get shaping info.
                auto& shapingInfo = shapingInfos[j];
                if (shapingInfo.Buffer == nullptr)
                {
                    // Get buffer.
                    shapingInfo.Buffer = GetShapingBuffer(msg);
                    if (shapingInfo.Buffer == nullptr)
                    {
                        return {};
                    }

                    // Fill buffer.
                    hb_shape(_hbFonts[j], shapingInfo.Buffer, nullptr, 0);
                    uint glyphCount       = 0;
                    shapingInfo.Glyphs    = hb_buffer_get_glyph_infos(shapingInfo.Buffer, &glyphCount);
                    shapingInfo.Positions = hb_buffer_get_glyph_positions(shapingInfo.Buffer, &glyphCount);
                }

                // Add shaped text.
                shapedText.Glyphs.push_back(ShapedGlyph
                {
                    .Metadata = _glyphs[codePoints[i]],
                    .Advance  = Vector2i(shapingInfo.Positions[i].x_advance, shapingInfo.Positions[i].y_advance) * _scaleFactor,
                    .Offset   = Vector2i(shapingInfo.Positions[i].x_offset,  shapingInfo.Positions[i].y_offset)  * _scaleFactor
                });
                shapedText.Width += shapedText.Glyphs.back().Advance.x;
                break;
            }
        }

        // Free resources.
        for (auto& shapingInfo : shapingInfos)
        {
            hb_buffer_destroy(shapingInfo.Buffer);
        }

        return shapedText;
    }

    std::vector<char32> Font::GetCodePoints(const std::string& msg) const
    {
        // Reserve minimum size.
        auto codePoints = std::vector<char32>{};
        codePoints.reserve((msg.size() / 4) + 1);

        // Collect code points.
        utf8::utf8to32(msg.begin(), msg.end(), std::back_inserter(codePoints));
        return codePoints;
    }

    hb_buffer_t* Font::GetShapingBuffer(const std::string& msg) const
    {
        // Allocate buffer.
        auto* buffer = hb_buffer_create();
        if (!hb_buffer_allocation_successful(buffer))
        {
            Debug::Log(Fmt("Failed to allocate shaping buffer for message `{}`", msg), Debug::LogLevel::Error);
            return nullptr;
        }

        // Insert characters.
        hb_buffer_add_utf8(buffer, msg.c_str(), msg.size(), 0, msg.size());

        // @todo Extend this later when a language needs it.
        // Set text direction and script.
        hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);                       // Left-to-right text.
        hb_buffer_set_script(buffer, HB_SCRIPT_LATIN);                           // Latin script.
        hb_buffer_set_language(buffer, hb_language_from_string("en", NO_VALUE)); // English language.

        return buffer;
    }

    void Font::CacheGlyph(char32 codePoint)
    {
        // Load valid glyph from font chain.
        FT_Face ftFont = nullptr;
        for (int i = 0; i < _ftFonts.size(); i++)
        {
            // Check if glyph is valid.
            uint charIdx = FT_Get_Char_Index(_ftFonts[i], codePoint);
            if (charIdx == 0)
            {
                // If no valid glyphs exist, use first font's invalid glyph.
                if (i < (_ftFonts.size() - 1))
                {
                    continue;
                }
                else
                {
                    i = 0;
                }
            }

            FT_Load_Glyph(_ftFonts[i], charIdx, FT_LOAD_DEFAULT);
            ftFont = _ftFonts[i];
            break;
        }
        Debug::Assert(ftFont != nullptr, Fmt("Failed to cache glyph U+{:X} for font `{}`.", (int)codePoint, _name));

        const auto& metrics = ftFont->glyph->metrics;

        // Pack glyph rectangle.
        auto size = Vector2i(FP_FROM(metrics.width, Q6_SHIFT), FP_FROM(metrics.height, Q6_SHIFT)) + Vector2i(GLYPH_PADDING * 2);
        auto rect = _rectPacks.back().insert(rectpack2D::rect_wh(size.x, size.y));
        if (!rect.has_value())
        {
            Debug::Log(Fmt("Active atlas {} for font `{}` is full. Creating new atlas.", _activeAtlasIdx, _name), Debug::LogLevel::Info);

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
        FT_Render_Glyph(ftFont->glyph, FT_RENDER_MODE_NORMAL);
        const auto& bitmap     = ftFont->glyph->bitmap;
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

    Font* FontManager::GetFont(const std::string& name)
    {
        // Check if font exists.
        auto* font = Find(_fonts, name);
        if (font == nullptr)
        {
            Debug::Log(Fmt("Attempted to get missing font `{}`.", name), Debug::LogLevel::Warning);
        }

        return font;
    }

    void FontManager::LoadFont(const std::string& name, const std::vector<std::string>& filenames, const std::filesystem::path& path, int pointSize,
                               const std::string& glyphPrecache)
    {
        // Check if font is already loaded.
        if (Find(_fonts, name) != nullptr)
        {
            return;
        }

        // Handle load.
        try
        {
            _fonts[name] = Font(_library, name, filenames, path, pointSize, glyphPrecache);

            Debug::Log(Fmt("Loaded font `{}` at point size {}.", name, pointSize));
        }
        catch (const std::runtime_error& ex)
        {
            Debug::Log(Fmt("Failed to load font `{}`: {}", name, ex.what()), Debug::LogLevel::Error);
        }
    }
}
