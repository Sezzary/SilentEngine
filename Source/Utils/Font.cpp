#include "Framework.h"
#include "Utils/Font.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include "Utils/Utils.h"

namespace Silent::Utils
{
    Font::Font(const std::string& name, const std::filesystem::path& path, int pointSize, FT_Library& fontLib)
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

        _isLoaded = true;
    }

    Font::~Font()
    {
        FT_Done_Face(_face);
    }

    std::vector<Glyph*> Font::GetGlyphs(const std::string& msg)
    {
        // Get rune IDs.
        auto runeIds = GetRuneIds(msg);

        // Collect glyphs.
        auto glyphs = std::vector<Glyph*>{};
        glyphs.reserve(runeIds.size());
        for (char32 runeId : runeIds)
        {
            // Check if glyph exists in atlas.
            auto it = _glyphs.find(runeId);
            if (it == _glyphs.end())
            {
                CacheGlyph(runeId);
            }

            // Add glyph.
            auto& [keyRuneId, glyph] = *it;
            glyphs.push_back(&glyph);
        }

        return glyphs;
    }

    bool Font::IsLoaded() const
    {
        return _isLoaded;
    }

    void Font::Initialize(const std::string& glyphPrecache)
    {
        // Collect unpositioned glyphs.
        auto runeIds = GetRuneIds(glyphPrecache);
        for (char32 runeId : runeIds)
        {
            FT_Load_Glyph(_face, runeId, FT_LOAD_DEFAULT);
            const auto& metrics = _face->glyph->metrics;

            _glyphs[runeId] = Glyph
            {
                .RuneId  = runeId,
                .Size    = Vector2i(metrics.width,        metrics.height),
                .Bearing = Vector2i(metrics.horiBearingX, metrics.horiBearingY),
                .Advance = (int)metrics.horiAdvance
            };
        }

        // @todo
        // Pack glyph rectangles.
        /*_glyphRects.reserve(_glyphs.size());
        auto resultSize = rectpack2D::find_best_packing_dont_sort<rectpack2D::space_rect>(
            _glyphRects,
            rectpack2D::make_finder_input(
                0,//max_side,
                0,//discard_step,
                report_successful,
                report_unsuccessful,
                rectpack2D::flipping_option::DISABLED,//runtime_flipping_mode
            )
        );*/

        // Set glyph atlas positions and rasterize.
        for (int i = 0; i < _glyphs.size(); i++)
        {
            auto&       glyph = _glyphs[i];
            const auto& rect  = _glyphRects[i];

            glyph.Position = Vector2i(rect.x, rect.y);
            RasterizeGlyph(glyph.RuneId);
        }
    }

    std::vector<char32> Font::GetRuneIds(const std::string& str) const
    {
        auto runeIds = std::vector<char32>{};
        utf8::utf8to32(str.begin(), str.end(), std::back_inserter(runeIds));
        return runeIds;
    }

    void Font::CacheGlyph(char32 runeId)
    {
        // Get data from font.
        FT_Load_Glyph(_face, runeId, FT_LOAD_DEFAULT);
        const auto& metrics = _face->glyph->metrics;

        // Register new glyph.
        _glyphs[runeId] = Glyph
        {
            .RuneId  = runeId,
            .Size    = Vector2i(metrics.width,        metrics.height),
            .Bearing = Vector2i(metrics.horiBearingX, metrics.horiBearingY),
            .Advance = (int)metrics.horiAdvance
        };
        _glyphRects.push_back(rectpack2D::space_rect(0, 0, metrics.width, metrics.height));

        /*using rect_type = rectpack2D::output_rect_t<rectpack2D::space_rect>;

        auto report_successful = [](rectpack2D::space_rect&) {
            return rectpack2D::callback_result::CONTINUE_PACKING;
        };

        auto report_unsuccessful = [](rectpack2D::space_rect&) {
            return rectpack2D::callback_result::ABORT_PACKING;
        };*/

        // @todo
        /*auto result_size = rectpack2D::find_best_packing_dont_sort<rect_type>(
            _glyphRects,
            rectpack2D::make_finder_input(
                0,//max_side,
                0,//discard_step,
                report_successful,
                report_unsuccessful,
                rectpack2D::flipping_option::DISABLED,//runtime_flipping_mode
            )
        );*/

        // Set position and rasterize.
        _glyphs[runeId].Position = Vector2i(_glyphRects.back().x, _glyphRects.back().y);
        RasterizeGlyph(runeId);
    }

    void Font::RasterizeGlyph(char32 runeId)
    {
        const auto& glyph = _glyphs[runeId];

        // Rasterize.
        FT_Render_Glyph(_face->glyph, FT_RENDER_MODE_NORMAL);
        const auto& bitmap     = _face->glyph->bitmap;
        byte*       pixelsTo   = &_atlas[(glyph.Position.y * DEFAULT_ATLAS_SIZE) + glyph.Position.x];
        byte*       pixelsFrom = (byte*)bitmap.buffer;

        // Copy pixels to atlas. @todo Needs padding?
        for (int i = 0; i < bitmap.rows; i++)
        {
            for (int x = 0; x < bitmap.width; x++)
            {
                pixelsTo[x] = (pixelsFrom[x] << 24) | 0xFFFFFF;
            }

            // Advance pointers.
            pixelsTo   += DEFAULT_ATLAS_SIZE;
            pixelsFrom += bitmap.width;
        }
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
        if (it == _fonts.end())
        {
            Debug::Log("Attempted to load loaded font `"+ fontName + "`.", Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return;
        }

        // Handle load.
        _fonts[fontName] = Font(fontName, fontPath, pointSize, _library);
        if (!_fonts[fontName].IsLoaded())
        {
            Debug::Log("Failed to load font `" + fontName + "`.", Debug::LogLevel::Error);
            _fonts.erase(fontName);
            return;
        }

        // Precache glyphs.
        _fonts[fontName].Initialize(glyphPrecache);

        Debug::Log("Loaded font `" + fontName + "` at point size " + std::to_string(pointSize) + ".");
    }
}
