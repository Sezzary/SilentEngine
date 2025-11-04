#pragma once

namespace Silent::Utils
{
    /** @brief Atlased font glyph. */
    struct Glyph
    {
        char32   RuneId   = 0;
        Vector2i Position = Vector2i::Zero;
        Vector2i Size     = Vector2i::Zero;
        Vector2i Bearing  = Vector2i::Zero;
        int      Advance  = 0;
    };

    /** @brief Atlased font. */
    class Font
    {
    private:
        static constexpr uint DEFAULT_ATLAS_SIZE = 2048;
        static constexpr uint GLYPH_PADDING      = 1;

        // =======
        // Fields
        // =======

        bool                                _isLoaded   = false; /** Load status. */
        std::string                         _name       = {};    /** Font name. */
        FT_Face                             _face       = {};    /** Loaded typeface file. */
        std::unordered_map<int, Glyph>      _glyphs     = {};    /** Key = rune ID, value = atlased glyph info. */
        std::vector<rectpack2D::space_rect> _glyphRects = {};    /** Optimally packed glyph rectangles. */
        std::vector<byte>                   _atlas      = {};    /** Rasterized glyph bitmap texture atlas. */

    public:
        // =============
        // Constructors
        // =============

        // @todo Remove.
        Font() = default;

        /** @brief Constructs a `Font` from a font file and adds it to a library.
         *
         * @param name Font name.
         * @param path Font file path.
         * @param pointSize Point size at which to load the font.
         * @param fontLib Library to load the font into.
         */
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

        /** @brief Checks if the font is loaded and usable.
         *
         * @param `true` if the font is loaded, `false` otherwise.
         */
        bool IsLoaded() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes the font with glyphs precached in the bitmap texture atlas.
         *
         * @param precacheGlyphs Glyphs to precache.
         */
        void Initialize(const std::string& precacheGlyphs);

    private:
        // ========
        // Helpers
        // ========

        /** @brief Gets the rune IDs for the glyphs in a string.
         *
         * @param str String to parse.
         * @return Rune IDs for each glyph.
         */
        std::vector<char32> GetRuneIds(const std::string& str) const;

        /** @brief Caches a new glyph in the bitmap texture atlas.
         *
         * @param runeId Rune ID of the glyph to cache.
         */
        void CacheGlyph(char32 runeId);

        /** @brief Registers a new glyph for rasterization.
         *
         * @param runeId Rune ID of the glyph to register.
         */
        void RegisterGlyph(char32 runeId);

        /** @brief Rasterizes a new glyph and adds it to the bitmap texture atlas.
         *
         * @param runeId Rune ID of the glyph to rasterize.
         */
        void RasterizeGlyph(char32 runeId);
    };

    /** @brief Atlased font manager. */
    class FontManager
    {
    private:
        // =======
        // Fields
        // =======

        FT_Library                            _library = {};
        std::unordered_map<std::string, Font> _fonts   = {}; /** Key = font name, value = atlased font. */

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
