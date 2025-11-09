#include "Framework.h"
#include "Assets/Fonts.h"

namespace Silent::Assets
{
    constexpr auto POINT_SIZE_HIGH_RES = 48;
    constexpr auto POINT_SIZE_LOW_RES  = 16;

    // @todo Combine fonts.
    const std::vector<FontMetadata> FONTS_METADATA
    {
        FontMetadata
        {
            .Name      = "SilentSerif.ttf",
            .PointSize = POINT_SIZE_HIGH_RES
        },
        FontMetadata
        {
            .Name      = "UnifontExMono.ttf",
            .PointSize = POINT_SIZE_LOW_RES
        }
    };
}
