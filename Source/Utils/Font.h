#pragma once

namespace Silent::Utils
{
    /** @brief Font chain metadata. */
    struct FontMetadata
    {
        std::string              Name           = {};
        std::vector<std::string> Filenames      = {};
        int                      PointSize      = 0;
        bool                     UseNativeScale = false;
    };

    /** @brief Rasterized glyph metadata. */
    struct GlyphMetadata
    {
        char32   CodePoint = 0;
        int      AtlasIdx  = 0;
        Vector2i Position  = Vector2i::Zero;
        Vector2i Size      = Vector2i::Zero;
    };

    /** @brief Shaped glyph data. */
    struct ShapedGlyph
    {
        const GlyphMetadata& Metadata;
        Vector2i             Advance = Vector2i::Zero;
        Vector2i             Offset  = Vector2i::Zero;
    };

    /** @brief Shaped text data. */
    struct ShapedText
    {
        std::vector<ShapedGlyph> Glyphs = {};
        int                      Width  = 0;
    };

    /** @brief Atlased font chain. */
    class Font
    {
        // ==========
        // Constants
        // ==========

        static constexpr int ATLAS_SIZE = 1024;

    private:
        static constexpr int GLYPH_PADDING = 1;

        // =======
        // Fields
        // =======

        std::string _name           = {};
        int         _pointSize      = 0;
        bool        _useNativeScale = false;
        float       _scaleFactor    = 0.0f;

        std::unordered_map<char32, GlyphMetadata> _glyphs         = {}; /** Key = code point, value = rasterized glyph metadata. */
        std::vector<smol_atlas_t*>                _rectAtlases    = {};
        std::vector<std::vector<byte>>            _textureAtlases = {};
        int                                       _activeAtlasIdx = 0;
        
        int                     _fontCount = 0;
        std::vector<FT_Face>    _ftFonts   = {};
        std::vector<hb_font_t*> _hbFonts   = {};

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `Font`. */
        Font() = default;

        /** @brief Constructs a `Font` from a chain of font file and adds it to a library, precaching a set of glyphs in its texture atlas.
         *
         * @param fontLib Library to load the font into.
         * @param metadata Font chain metadata.
         * @param path Path containing font files.
         * @param precacheGlyphs Glyphs to precache.
         */
        Font(FT_Library& fontLib, const FontMetadata& metadata, const std::filesystem::path& path, const std::string& precacheGlyphs);

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

        /** @brief Gets the monochrome texture atlases containing cached font glyphs.
         *
         * @return Glyph texture atlases.
         */
        const std::vector<std::vector<byte>>& GetTextureAtlases() const;

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
         * @param metadata Font chain metadata.
         * @param path Path containing font files.
         * @param precacheGlyphs Glyphs to precache in the atlas upon font initialization.
         */
        void LoadFont(const FontMetadata& metadata, const std::filesystem::path& path, const std::string& precacheGlyphs = {});
    };
}
