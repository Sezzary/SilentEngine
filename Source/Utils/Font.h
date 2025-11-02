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

        std::string                    _name     = {};
        FT_Face                        _fontFace = {};
        std::unordered_map<int, Glyph> _glyphs   = {}; /** Key = rune ID, value = glyph info. */
        bool                           _isLoaded = false;

    public:
        // =============
        // Constructors
        // =============

        Font() = default;

        Font(const std::string& name, const std::filesystem::path& path, int pointSize, FT_Library& fontLib);

        /** @brief Gracefully destroys the `Font`, freeing resources. */
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

        void LoadGlyph(int runeId);

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

        /** @brief Constructs a default `FontManager`. */
        FontManager();

        /** @brief Gracefully destroys the `FontManager`, freeing resources. */
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
