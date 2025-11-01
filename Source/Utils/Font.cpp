#include "Framework.h"
#include "Utils/Font.h"

#include "Application.h"
#include "Renderer/Renderer.h"

namespace Silent::Utils
{
    Font::Font(FT_Library& fontLib, const std::filesystem::path& fontPath, int pointSize)
    {
        if (FT_Init_FreeType(&fontLib))
        {
            Debug::Log("Failed to initialize font library.", Debug::LogLevel::Error);
            return;
        }

        if (FT_New_Face(fontLib, fontPath.string().c_str(), 0, &_fontFace))
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
        auto glyphs = std::vector<Glyph*>{};

        for (auto thing : msg)
        {

        }
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
        _fonts[fontName] = Font(_fontLibrary, fontPath, pointSize);
        if (!_fonts[fontName].IsLoaded())
        {
            Debug::Log("Failed to load font `" + fontName + "`.", Debug::LogLevel::Error);
            _fonts.erase(fontName);
        }
    }
}
