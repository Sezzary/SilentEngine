#pragma once

namespace Silent::Utils
{
    struct Glyph
    {
        char32   RuneId   = 0;
        Vector2i Position = Vector2i::Zero;
        Vector2i Size     = Vector2i::Zero;
        Vector2i Bearing  = Vector2i::Zero;
        int      Advance  = 0;
    };

    class Font
    {
    private:
        static constexpr uint DEFAULT_ATLAS_SIZE = 1024;

        // =======
        // Fields
        // =======

        bool                                _isLoaded     = false;
        std::string                         _name         = {};
        FT_Face                             _face         = {};
        std::unordered_map<int, Glyph>      _glyphs       = {}; /** Key = rune ID, value = glyph info. */
        std::vector<rectpack2D::space_rect> _packedGlyphs = {};
        std::vector<byte>                   _atlasPixels  = {};

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

        void CacheGlyph(char32 runeId);

        void RasterizeGlyph(const Glyph& glyph);

    private:
        // ========
        // Helpers
        // ========

        void LoadGlyph(int runeId);

        void BuildAtlas();
    };

    /** @brief Font manager. */
    class FontManager
    {
    private:
        // =======
        // Fields
        // =======

        FT_Library                            _library = {};
        std::unordered_map<std::string, Font> _fonts   = {}; /** Key = font name, value = font. */

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

        /** @brief Gets a loaded font. If the font is missing, it regurns `nullptr`.
         *
         * @param fontName Name of the font to retrieve.
         * @return Loaded font.
         */
        Font* GetFont(const std::string& fontName);

        // ==========
        // Utilities
        // ==========

        /** @brief Loads and registers a font file.
         *
         * @param fontPath Font file path.
         * @param pointSize Vertical rasterization point size.
         * @param glyphPrecache Glyphs to precache in the atlas upon font initialization.
         */
        void LoadFont(const std::filesystem::path& fontPath, int pointSize, const std::string& glyphPrecache);
    };
}
