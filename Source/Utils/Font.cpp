#include "Framework.h"
#include "Utils/Font.h"

#include "Application.h"
#include "Utils/Utils.h"

namespace Silent::Utils
{
    Font::Font(FT_Library& fontLib, const FontMetadata& metadata, const stdfs::path& path,
               const std::string& precacheGlyphs)
    {
        constexpr int POINT_SIZE_MAX = ATLAS_SIZE / 8;

        // @todo Check if counts are equal for fonts and trackings.

        _name               = metadata.Name;
        _trackings          = metadata.Trackings;
        _enableAntialiasing = metadata.EnableAntialiasing;

        // Clamp point size.
        _pointSize = metadata.PointSize;
        if (_pointSize > POINT_SIZE_MAX)
        {
            Debug::Log(Fmt("Attempted to initialize font chain `{}` with invalid point size {}. Max is {}.",
                           _name, _pointSize, POINT_SIZE_MAX),
                       Debug::LogLevel::Warning);

            _pointSize = std::min<int>(_pointSize, POINT_SIZE_MAX);
        }

        // Add chained fonts to library.
        _ftFonts.reserve(metadata.Filenames.size());
        for (const auto& filename : metadata.Filenames)
        {
            auto ftFont = FT_Face{};
            if (FT_New_Face(fontLib, (path / filename).string().c_str(), 0, &ftFont))
            {
                throw std::runtime_error("Failed to initialize FreeType font.");
            }
            _ftFonts.push_back(ftFont);

            // Set point size.
            if (FT_Set_Pixel_Sizes(ftFont, 0, _pointSize))
            {
                throw std::runtime_error("Failed to set font point size.");
            }
        }

        // Add first atlas.
        AddAtlas();

        // Precache glyphs.
        auto codePoints = GetUtf8CodePoints(precacheGlyphs);
        for (char32 codePoint : codePoints)
        {
            if (Find(_glyphs, codePoint) != nullptr)
            {
                Debug::Log(Fmt("Attempted to precache existing glyph U+{:X} for font chain `{}`. Check for duplicates.",
                               (int)codePoint, _name),
                           Debug::LogLevel::Warning);
                continue;
            }

            CacheGlyph(codePoint);
        }

        // @debug
        /*for (int i = 0; i < _textureAtlases.size(); i++)
        {
            stbi_write_png((g_App.GetFilesystem().GetAppDirectory() / (_name + Fmt("_Atlas{}.png", i))).string().c_str(),
                           ATLAS_SIZE, ATLAS_SIZE, RGBA_COMP_COUNT, _textureAtlases[i].data(), ATLAS_SIZE * 4);
            break;
        }*/
    }

    Font::~Font()
    {
        for (auto* rectAtlas : _rectAtlases)
        {
            sma_atlas_destroy(rectAtlas);
        }

        for (auto& ftFont : _ftFonts)
        {
            FT_Done_Face(ftFont);
        }
    }

    const std::string& Font::GetName() const
    {
        return _name;
    }

    int Font::GetPointSize() const
    {
        return _pointSize;
    }

    const std::vector<std::vector<byte>>& Font::GetTextureAtlases() const
    {
        return _textureAtlases;
    }

    ShapedText Font::GetShapedText(const std::string& msg)
    {
        // Cache new glyphs.
        auto codePoints = GetUtf8CodePoints(msg);
        for (char32 codePoint : codePoints)
        {
            if (Find(_glyphs, codePoint) != nullptr)
            {
                continue;
            }

            CacheGlyph(codePoint);
        }

        auto shapedText = ShapedText{};
        shapedText.Glyphs.reserve(codePoints.size());

        // Build shaped text.
        for (int i = 0; i < codePoints.size(); i++)
        {
            char32 codePoint = codePoints[i];

            // Run through font chain.
            for (int j = 0; j < _ftFonts.size(); j++)
            {
                auto* ftFont = _ftFonts[j];

                // Check if glyph is valid.
                int charIdx = FT_Get_Char_Index(ftFont, codePoint);
                if (charIdx == 0)
                {
                    // If no valid glyphs exist, use first font's invalid glyph.
                    if (j < (_ftFonts.size() - 1))
                    {
                        continue;
                    }
                    else
                    {
                        j = 0;
                    }
                }

                // Compute spacing.
                float spacing = _glyphs[codePoint].Spacing;
                if (FT_HAS_KERNING(ftFont) && i < (codePoints.size() - 1))
                {
                    char32 nextCodePoint = codePoints[i + 1];

                    int charIdx0 = FT_Get_Char_Index(ftFont, codePoint);
                    int charIdx1 = FT_Get_Char_Index(ftFont, nextCodePoint);

                    auto kerningDelta = FT_Vector{};
                    FT_Get_Kerning(ftFont, charIdx0, charIdx1, FT_KERNING_DEFAULT, &kerningDelta);
                    spacing += Q6_TO_FLT(kerningDelta.x) + (_pointSize * _trackings[j]);
                }

                // Add shaped glyph.
                shapedText.Glyphs.push_back(ShapedGlyph
                {
                    .Attribs = _glyphs[codePoint],
                    .Spacing = spacing
                });
                shapedText.Width += spacing;
                break;
            }
        }

        return shapedText;
    }

    const std::set<int>& Font::GetDirtyGpuAtlasIdxs() const
    {
        return _dirtyGpuAtlasIdxs;
    }

    void Font::ClearDirtyGpuAtlasIdxs()
    {
        _dirtyGpuAtlasIdxs.clear();
    }

    void Font::CacheGlyph(char32 codePoint)
    {
        // Load valid glyph from font chain.
        FT_Face ftFont  = nullptr;
        int     fontIdx = 0;
        for (fontIdx = 0; fontIdx < _ftFonts.size(); fontIdx++)
        {
            // Check if glyph is valid.
            uint charIdx = FT_Get_Char_Index(_ftFonts[fontIdx], codePoint);
            if (charIdx == 0)
            {
                // If no valid glyphs exist, use first font's invalid glyph.
                if (fontIdx < (_ftFonts.size() - 1))
                {
                    continue;
                }
                else
                {
                    fontIdx = 0;
                }
            }

            FT_Load_Glyph(_ftFonts[fontIdx], charIdx, _enableAntialiasing ? FT_LOAD_DEFAULT : FT_LOAD_NO_HINTING);
            ftFont = _ftFonts[fontIdx];
            break;
        }
        Debug::Assert(ftFont != nullptr, Fmt("Failed to cache glyph U+{:X} for font chain `{}`.",
                                             (int)codePoint, _name));

        // Get metrics.
        const auto& metrics = ftFont->glyph->metrics;
        auto        size    = Vector2i(Q6_TO_FLT(metrics.width), Q6_TO_FLT(metrics.height)) +
                              Vector2i(GLYPH_PADDING * 2);

        // Insert new rectangle.
        const auto& rect = InsertGlyphRect(size, codePoint);

        // Get FT glyph.
        auto ftGlyph = FT_Glyph{};
        FT_Get_Glyph(ftFont->glyph, &ftGlyph);

        // Get FT glyph box.
        auto ftBox = FT_BBox{};
        FT_Glyph_Get_CBox(ftGlyph, FT_GLYPH_BBOX_SUBPIXELS, &ftBox);

        // Register new glyph attributes.
        _glyphs[codePoint] = GlyphAttribs
        {
            .CodePoint     = codePoint,
            .AtlasIdx      = _activeAtlasIdx,
            .AtlasPosition = Vector2i(sma_item_x(&rect), sma_item_y(&rect)) + Vector2i(GLYPH_PADDING),
            .AtlasSize     = size - Vector2i(GLYPH_PADDING * 2),
            .Bearing       = Vector2(Q6_TO_FLT(metrics.horiBearingX), Q6_TO_FLT(metrics.horiBearingY)),
            .Spacing       = Q6_TO_FLT(metrics.horiAdvance) + (_pointSize * _trackings[fontIdx]),
            .Ascender      = Q6_TO_FLT(ftFont->size->metrics.ascender),
            .Descender     = Q6_TO_FLT(ftFont->size->metrics.descender),
            .MinY          = Q6_TO_FLT(ftBox.yMin),
            .MaxY          = Q6_TO_FLT(ftBox.yMax)
        };
        const auto& glyph = _glyphs[codePoint];

        // Rasterize in texture atlas.
        RasterizeGlyph(ftFont, glyph);
        //Debug::Log(Fmt("Cached glyph U+{:X} for font chain `{}`.", (int)codePoint, _name),
        //           Debug::LogLevel::Info, Debug::LogMode::Debug);
    }

    smol_atlas_item_t& Font::InsertGlyphRect(const Vector2i& size, char32 codePoint)
    {
        // Attempt insertion into current active atlas.
        auto* rect = sma_item_add(_rectAtlases[_activeAtlasIdx], size.x, size.y);
        if (rect == nullptr)
        {
            Debug::Log(Fmt("Active atlas {} for font chain `{}` is full. Creating new atlas.", _activeAtlasIdx, _name));
            AddAtlas();

            // Attempt insertion into new active atlas.
            rect = sma_item_add(_rectAtlases[_activeAtlasIdx], size.x, size.y);
            if (rect == nullptr)
            {
                throw std::runtime_error(Fmt("Failed to insert glyph U+{:X} rectangle for font chain `{}`.",
                                             (int)codePoint, _name));
            }
        }

        return *rect;
    }

    void Font::RasterizeGlyph(const FT_Face& ftFont, const GlyphAttribs& attribs)
    {
        FT_Render_Glyph(ftFont->glyph, FT_RENDER_MODE_NORMAL); // @todo Try SDF generator.
        const auto& bitmap     = ftFont->glyph->bitmap;
        byte*       pixelsFrom = (byte*)bitmap.buffer;
        byte*       pixelsTo   = &_textureAtlases.back()[(((attribs.AtlasPosition.y) * ATLAS_SIZE) * RGBA_COMP_COUNT) +
                                                         ((attribs.AtlasPosition.x) * RGBA_COMP_COUNT)];

        // Copy pixels to atlas.
        for (int y = 0; y < bitmap.rows; y++)
        {
            for (int x = 0; x < bitmap.width; x++)
            {
                byte pixel = pixelsFrom[(bitmap.width * y) + x];
                pixel      = _enableAntialiasing ? pixel :
                                                   Q8_COLOR(((uchar)pixel >= Q8_COLOR(0.5f)) ? 1.0f : 0.0f);

                int pixelBaseIdx           = ((ATLAS_SIZE * y) * RGBA_COMP_COUNT) + (x * RGBA_COMP_COUNT);
                pixelsTo[pixelBaseIdx + 0] =
                pixelsTo[pixelBaseIdx + 1] =
                pixelsTo[pixelBaseIdx + 2] =
                pixelsTo[pixelBaseIdx + 3] = pixel;
            }
        }

        // Mark relevant GPU atlas texture as dirty.
        _dirtyGpuAtlasIdxs.insert(_activeAtlasIdx);
    }

    void Font::AddAtlas()
    {
        _rectAtlases.push_back(sma_atlas_create(ATLAS_SIZE, ATLAS_SIZE));
        _textureAtlases.emplace_back(std::vector<byte>((ATLAS_SIZE * ATLAS_SIZE) * RGBA_COMP_COUNT));
        _activeAtlasIdx = _rectAtlases.size() - 1;
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
            Debug::Log(Fmt("Attempted to get missing font chain `{}`.", name), Debug::LogLevel::Warning);
        }

        return font;
    }

    void FontManager::LoadFont(const FontMetadata& metadata, const stdfs::path& path,
                               const std::string& glyphPrecache)
    {
        if (metadata.Filenames.empty())
        {
            Debug::Log(Fmt("Attempted to load font chain `{}` without font files.", metadata.Name),
                       Debug::LogLevel::Warning);
            return;
        }

        // Handle load.
        try
        {
            _fonts.try_emplace(metadata.Name, _library, metadata, path, glyphPrecache);

            //Debug::Log(Fmt("Loaded font chain `{}` at point size {}.", metadata.Name, metadata.PointSize));
        }
        catch (const std::runtime_error& ex)
        {
            Debug::Log(Fmt("Failed to load font chain `{}`: {}", metadata.Name, ex.what()), Debug::LogLevel::Error);
        }
    }
}
