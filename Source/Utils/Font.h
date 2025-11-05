#pragma once

namespace Silent::Utils
{
    /** @brief Atlased glyph metadata. */
    struct Glyph
    {
        char32   CodePoint = 0;
        Vector2i Position  = Vector2i::Zero;
        Vector2i Size      = Vector2i::Zero;
        Vector2i Bearing   = Vector2i::Zero;
        int      Advance   = 0;
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

        bool                              _isLoaded   = false;                                   /** Load status. */
        std::string                       _name       = {};                                      /** Font name. */
        FT_Face                           _face       = {};                                      /** Loaded typeface file. */
        std::unordered_map<char32, Glyph> _glyphs     = {};                                      /** Key = code point, value = atlased glyph metadata. */
        PackedRects                       _glyphRects = PackedRects({ ATLAS_SIZE, ATLAS_SIZE }); /** Packed glyph rectangles. */
        std::vector<byte>                 _atlas      = {};                                      /** Monochrome glyph texture atlas. */

    public:
        // =============
        // Constructors
        // =============

        // @todo Remove.
        Font() = default;

        /** @brief Constructs a `Font` from a font file and adds it to a library, precaching a set of glyphs in the bitmat texture atlas.
         *
         * @param fontLib Library to load the font into.
         * @param name Font name.
         * @param path Font file path.
         * @param pointSize Point size at which to load the font.
         * @param precacheGlyphs Glyphs to precache.
         */
        Font(FT_Library& fontLib, const std::string& name, const std::filesystem::path& path, int pointSize, const std::string& precacheGlyphs);

        /** @brief Gracefully destroys the `Font`, freeing resources. */
        ~Font();

        // ========
        // Getters
        // ========

        /** @brief Gets the monochrome texture atlas containing cached glyphs.
         *
         * @return Glyph texture atlas.
         */
        const std::vector<byte>& GetAtlas();

        /** @brief Gets metadata for glyphs in a message.
         *
         * @param msg Message to parse.
         * @return Message glyph metadata.
         */
        std::vector<Glyph*> GetGlyphs(const std::string& msg);

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if the font is loaded and usable.
         *
         * @param `true` if the font is loaded, `false` otherwise.
         */
        bool IsLoaded() const;

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

        /** @brief Caches a new glyph in the bitmap texture atlas.
         *
         * @param codePoint Code point of the glyph to cache.
         * @return `true` if the glyph was cached successfully, `false` otherwise.
         */
        bool CacheGlyph(char32 codePoint);
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
