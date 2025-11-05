#pragma once

namespace Silent::Assets
{
    constexpr char ASSETS_FONTS_DIR_NAME[] = "Fonts";

    struct FontMetadata
    {
        std::string Name      = {};
        int         PointSize = 0;
        std::string Precache  = {};
    };

    extern const std::vector<FontMetadata> FONTS_METADATA;
}
