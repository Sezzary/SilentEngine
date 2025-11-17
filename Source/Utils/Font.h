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

    /** @brief Shaped text line data. */
    struct ShapedText
    {
        std::vector<ShapedGlyph> Glyphs = {}; /** Shaped glyphs. */
        int                      Width  = 0;  /** Line width in pixels. */
    };

    /** @brief Atlased font chain. */
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

        std::string                               _name        = {};   /** Font name. */
        int                                       _pointSize   = 0;    /** Point size. */
        float                                     _scaleFactor = 0.0f; /** Relative pixel space scale factor. */
        std::unordered_map<char32, GlyphMetadata> _glyphs      = {};   /** Key = code point, value = rasterized glyph metadata. */
        
        std::vector<PackedRects>       _rectPacks = {};     /** Glyph rectangle packs. */
        std::vector<std::vector<byte>> _atlases   = {};     /** Monochrome glyph texture atlases. */
        int                            _activeAtlasIdx = 0; /** Index of the atlas currently used for caching. */
        
        int                     _fontCount = 0;  /** Number of font files in the chain. */
        std::vector<FT_Face>    _ftFonts   = {}; /** FreeType font face handles. */
        std::vector<hb_font_t*> _hbFonts   = {}; /** HarfBuzz font handles. */

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `Font`. */
        Font() = default;

        /** @brief Constructs a `Font` from a chain of font file and adds it to a library, precaching a set of glyphs in its texture atlas.
         *
         * @param fontLib Library to load the font into.
         * @param name Font name to use for retrieval.
         * @param filenames Font chain filenames.
         * @param path Path containing font files.
         * @param pointSize Point size at which to load the font.
         * @param precacheGlyphs Glyphs to precache.
         */
        Font(FT_Library& fontLib, const std::string& name, const std::vector<std::string>& filenames, const std::filesystem::path& path, int pointSize,
             const std::string& precacheGlyphs);

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

        /** @brief Gets the shaped text for a message.
         *
         * @param msg Message to parse.
         * @return Shaped text.
         */
        ShapedText GetShapedText(const std::string& msg);

    private:
        // ========
        // Helpers
        // ========

        /** @brief Gets the code points for the glyphs in a message.
         *
         * @param msg Message to parse.
         * @return Code points for each glyph.
         */
        std::vector<char32> GetCodePoints(const std::string& msg) const;

        /** @brief Gets a shaping buffer for a given message.
         *
         * @param msg Message to create the buffer for.
         * @return Shaping buffer.
         */
        hb_buffer_t* GetShapingBuffer(const std::string& msg) const;

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

        /** @brief Gets a loaded font. If the font is missing, it returns `nullptr`.
         *
         * @param name Name of the font to retrieve.
         * @return Loaded font.
         */
        Font* GetFont(const std::string& name);

        // ==========
        // Utilities
        // ==========

        /** @brief Loads and registers a font chain.
         *
         * @param name Font name to use for retrieval.
         * @param filenames Font chain filenames.
         * @param path Path containing font files.
         * @param pointSize Vertical rasterization point size.
         * @param precacheGlyphs Glyphs to precache in the atlas upon font initialization.
         */
        void LoadFont(const std::string& name, const std::vector<std::string>& filenames, const std::filesystem::path& path, int pointSize,
                      const std::string& precacheGlyphs = {});
    };
}
