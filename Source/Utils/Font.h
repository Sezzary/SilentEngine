#pragma once

namespace Silent::Utils
{
    struct Glyph
    {
        int      Id       = 0;
        Vector2i Position = Vector2i::Zero;
        Vector2i Size     = Vector2i::Zero;
        Vector2i Bearing  = Vector2i::Zero;
        int      Advance  = 0;
    };

    class Font
    {
    private:
        // =======
        // Fields
        // =======

        FT_Face                        _fontFace = {};
        std::unordered_map<int, Glyph> _glyphs   = {}; /** Key = glyph ID, value = glyph info. */
        bool                           _isLoaded = false;

    public:
        // =============
        // Constructors
        // =============

        Font(FT_Library& fontLib, const std::filesystem::path& fontPath, int pointSize);

        ~Font();

        // ========
        // Getters
        // ========

        std::vector<Glyph*> GetGlyphs(const std::string& msg);

        // ==========
        // Inquirers
        // ==========

        bool IsLoaded() const;

        // ==========
        // Utilities
        // ==========

        void CacheGlyphs(const std::string& glyphs);

        void UncacheGlyphs();

    private:
        // ========
        // Helpers
        // ========

        void LoadGlyph(int glyphId);

        void BuildAtlas();
    };

    class FontManager
    {
    private:
        // =======
        // Fields
        // =======

        FT_Library                            _fontLibrary = {};
        std::unordered_map<std::string, Font> _fonts       = {}; /** Key = font name, value = font. */

    public:
        // =============
        // Constructors
        // =============

        FontManager();

        ~FontManager();

        // ========
        // Getters
        // ========

        Font* GetFont(const std::string& fontName);

        // ==========
        // Utilities
        // ==========

        void LoadFont(const std::filesystem::path& fontPath, int pointSize);
    };
}
