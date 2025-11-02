#include "Framework.h"
#include "Utils/Font.h"

#include "Application.h"
#include "Renderer/Renderer.h"

namespace Silent::Utils
{
    Font::Font(const std::string& name, const std::filesystem::path& path, int pointSize, FT_Library& fontLib)
    {
        _name = name;

        if (FT_Init_FreeType(&fontLib))
        {
            Debug::Log("Failed to initialize font library.", Debug::LogLevel::Error);
            return;
        }

        if (FT_New_Face(fontLib, path.string().c_str(), 0, &_fontFace))
        {
            Debug::Log("Failed to initialize font face.", Debug::LogLevel::Error);
            return;
        }

        if (FT_Set_Pixel_Sizes(_fontFace, 0, pointSize))
        {
            Debug::Log("Failed to set font point size.", Debug::LogLevel::Error);
            return;
        }

        _isLoaded = true;
    }

    Font::~Font()
    {
        FT_Done_Face(_fontFace);
    }

    std::vector<Glyph*> Font::GetGlyphs(const std::string& msg)
    {
        // Get rune IDs.
        auto runeIds = std::vector<char32_t>{};
        utf8::utf8to32(msg.begin(), msg.end(), std::back_inserter(runeIds));

        // Collect glyphs.
        auto glyphs = std::vector<Glyph*>{};
        glyphs.reserve(runeIds.size());
        for (char32_t& runeId : runeIds)
        {
            // @todo If the glyph isn't loaded yet, load it and add to atlas.

            // Check if rune exists.
            auto it = _glyphs.find(runeId);
            if (it == _glyphs.end())
            {
                Debug::Log("Failed to find glyph rune ID " + std::to_string(runeId) + "` in font `" + _name + "`.", Debug::LogLevel::Warning, Debug::LogMode::Debug);
                continue;
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

    FontManager::FontManager()
    {
    }

    FontManager::~FontManager()
    {
        _fonts.clear();
        FT_Done_FreeType(_fontLibrary);
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

    void FontManager::LoadFont(const std::filesystem::path& fontPath, int pointSize)
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
        _fonts[fontName] = Font(fontName, fontPath, pointSize, _fontLibrary);
        if (!_fonts[fontName].IsLoaded())
        {
            Debug::Log("Failed to load font `" + fontName + "`.", Debug::LogLevel::Error);
            _fonts.erase(fontName);
        }
    }
}
