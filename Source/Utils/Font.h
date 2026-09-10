#pragma once

namespace Silent::Utils
{
    /** @brief Font chain metadata. */
    struct FontMetadata
    {
        std::string              Name               = {};
        std::vector<std::string> Filenames          = {};
        int                      PointSize          = 0;
        std::vector<float>       Trackings          = {};
        bool                     EnableAntialiasing = false;
    };

    /** @brief Rasterized glyph attributes. */
    struct GlyphAttribs
    {
        char32   CodePoint     = 0;
        int      AtlasIdx      = 0;
        Vector2i AtlasPosition = Vector2i::Zero;
        Vector2i AtlasSize     = Vector2i::Zero;

        Vector2 Bearing   = Vector2::Zero;
        float   Spacing   = 0.0f;
        float   Ascender  = 0.0f;
        float   Descender = 0.0f;
        float   MinY      = 0.0f;
        float   MaxY      = 0.0f;
    };

    /** @brief Shaped glyph data. */
    struct ShapedGlyph
    {
        const GlyphAttribs& Attribs;
        float               Spacing = 0.0f;
    };

    /** @brief Shaped text data. */
    struct ShapedText
    {
        std::vector<ShapedGlyph> Glyphs = {};
        float                    Width  = 0.0f;
    };

    /** @brief Atlased font chain. */
    class Font
    {
    public:
        // ==========
        // Constants
        // ==========

        static constexpr int ATLAS_SIZE = 1024;

    private:
        static constexpr int GLYPH_PADDING = 1;

        // =======
        // Fields
        // =======

        std::string        _name               = {};
        int                _pointSize          = 0;
        std::vector<float> _trackings          = {};
        bool               _enableAntialiasing = false;
        
        std::vector<FT_Face>                     _ftFonts        = {};
        std::unordered_map<char32, GlyphAttribs> _glyphs         = {}; /** Key = code point, value = rasterized glyph attributes. */
        std::vector<smol_atlas_t*>               _rectAtlases    = {};
        std::vector<std::vector<byte>>           _textureAtlases = {};
        int                                      _activeAtlasIdx = 0;

        std::set<int> _dirtyGpuAtlasIdxs = {};

    public:
        // =============
        // Constructors
        // =============

        /** @brief Creates a default uninitialized instance. */
        Font() = default;

        /** @brief Creates an instance from a chain of font files and adds it to a library and precaches a set of glyphs
         * in the texture atlas.
         *
         * @param fontLib Library to load the font into.
         * @param metadata Font chain metadata.
         * @param path Path containing font files.
         * @param precacheGlyphs Glyphs to precache.
         */
        Font(FT_Library& fontLib, const FontMetadata& metadata, const stdfs::path& path,
             const std::string& precacheGlyphs);

        /** @brief Gracefully destroys the instance and frees resources. */
        ~Font();

        // ========
        // Getters
        // ========

        /** @brief Gets the name.
         *
         * @return Font name.
         */
        const std::string& GetName() const;

        /** @brief Gets the point size.
         *
         * @return Font point size.
         */
        int GetPointSize() const;

        /** @brief Gets the monochrome texture atlases containing cached font glyphs.
         *
         * @return Glyph texture atlases.
         */
        const std::vector<std::vector<byte>>& GetTextureAtlases() const;

        /** @brief Gets the indices of glyph atlas textures which require updating on the GPU.
         *
         * @return Dirty glyph texture atlas indices.
         */
        const std::set<int>& GetDirtyGpuAtlasIdxs() const;

        /** @brief Gets the shaped text for a message.
         *
         * @param msg Message to parse.
         * @return Shaped text.
         */
        ShapedText GetShapedText(const std::string& msg);

        // ==========
        // Utilities
        // ==========

        /** @brief Clears all indices of glyph texture atlases marked for updating on the GPU. */
        void ClearDirtyGpuAtlasIdxs();

    private:
        // ========
        // Helpers
        // ========

        /** @brief Caches a new glyph in the texture atlas.
         *
         * @param codePoint Code point of the glyph to cache.
         */
        void CacheGlyph(char32 codePoint);

        /** @brief Inserts a glyph rectangle into the active rectangle atlas.
         *
         * @param size Glyph size in pixels.
         * @param codePoint Code point of the glyph.
         * @return Inserted glyph rectangle.
         * @throws `std::runtime_error` if rect insertion fails.
         */
        smol_atlas_item_t& InsertGlyphRect(const Vector2i& size, char32 codePoint);

        /** @brief Rasterizes a glyph and inserts the pixels into the active texture atlas.
         *
         * @param ftFont FreeType font.
         * @param attribs Attributes of the glyph to rasterize.
         */
        void RasterizeGlyph(const FT_Face& ftFont, const GlyphAttribs& attribs);

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

        /** @brief Creates a default instance. */
        FontManager();

        /** @brief Gracefully destroys the instance and frees resources. */
        ~FontManager();

        // ========
        // Getters
        // ========

        /** @brief Gets a loaded font.
         *
         * @param name Name of the font to retrieve.
         * @return Loaded font or `nullptr` if missing.
         */
        Font* GetFont(const std::string& name);

        // ==========
        // Utilities
        // ==========

        /** @brief Loads and registers a font as a font chain.
         *
         * @param metadata Font chain metadata.
         * @param path Path containing font files.
         * @param precacheGlyphs Glyphs to precache in the atlas on font initialization.
         */
        void LoadFont(const FontMetadata& metadata, const stdfs::path& path,
                      const std::string& precacheGlyphs = {});
    };
}
