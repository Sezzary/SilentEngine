#pragma once

namespace Silent::Utils
{
    /** @brief Rasterized glyph metadata. */
    struct GlyphMetadata
    {
        char32   CodePoint = 0;              /** Unicode character ID. */
        int      AtlasIdx  = 0;              /** Index of the atlas in which the glyph is cached. */
        Vector2i Position  = Vector2i::Zero; /** Pixel position in atlas. */
        Vector2i Size      = Vector2i::Zero; /** Pixel size in atlas. */
    };

    /** @brief Shaped glyph data. */
    struct ShapedGlyph
    {
        const GlyphMetadata& Metadata;                 /** Rasterized glyph metadata. */
        Vector2i             Advance = Vector2i::Zero; /** Pixel line advance. */
        Vector2i             Offset  = Vector2i::Zero; /** Pixel offset. */
    };

    /** @brief Atlased font. */
    class Font
    {
    private:
        // ========
        // Aliases
        // ========

        using PackedRects = rectpack2D::empty_spaces<false, rectpack2D::default_empty_spaces>;

        // ==========
        // Constants
        // ==========

        static constexpr uint ATLAS_SIZE    = 2048;
        static constexpr uint GLYPH_PADDING = 1;

        // =======
        // Fields
        // =======

        std::string                               _name      = {};      /** Font name. */
        int                                       _pointSize = 0;       /** Point size. */
        std::unordered_map<char32, GlyphMetadata> _glyphs    = {};      /** Key = code point, value = rasterized glyph metadata. */
        std::vector<PackedRects>                  _rectPacks = {};      /** Glyph rectangle packs. */
        std::vector<std::vector<byte>>            _atlases   = {};      /** Monochrome glyph texture atlases. */

        int        _activeAtlasIdx = 0;       /** Index of the atlas currently used for caching. */
        float      _scaleFactor    = 0.0f;
        FT_Face    _ftFace         = {};      /** FreeType typeface handle. */
        hb_font_t* _hbFont         = nullptr; /** HarfBuzz font handle. */

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `Font`. */
        Font() = default;

        /** @brief Constructs a `Font` from a font file and adds it to a library, precaching a set of glyphs in the bitmat texture atlas.
         *
         * @param fontLib Library to load the font into.
         * @param path Font file path.
         * @param pointSize Point size at which to load the font.
         * @param precacheGlyphs Glyphs to precache.
         */
        Font(FT_Library& fontLib, const std::filesystem::path& path, int pointSize, const std::string& precacheGlyphs);

        /** @brief Gracefully destroys the `Font`, freeing resources. */
        ~Font();

        // ========
        // Getters
        // ========

        /** @brief Gets the point size.
         *
         * @return point size.
         */
        int GetPointSize() const;

        /** @brief Gets the monochrome texture atlases containing cached glyphs.
         *
         * @return Glyph texture atlases.
         */
        const std::vector<std::vector<byte>>& GetAtlases() const;

        /** @brief Gets the shaped glyphs for a message.
         *
         * @param msg Message to parse.
         * @return Shaped glyphs.
         */
        std::vector<ShapedGlyph> GetShapedGlyphs(const std::string& msg);

    private:
        // ========
        // Helpers
        // ========

        /** @brief Gets the code points for the glyphs in a string.
         *
         * @param str String to parse.
         * @return Code points for each glyph.
         */
        std::vector<char32> GetCodePoints(const std::string& str) const;

        /** @brief Caches a new glyph in the texture atlas.
         *
         * @param codePoint Code point of the glyph to cache.
         */
        void CacheGlyph(char32 codePoint);

        /** @brief Adds a new glyph texture atlas to use for caching. */
        void AddAtlas();
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
         * @param precacheGlyphs Glyphs to precache in the atlas upon font initialization.
         */
        void LoadFont(const std::filesystem::path& fontPath, int pointSize, const std::string& precacheGlyphs = {});
    };
}
