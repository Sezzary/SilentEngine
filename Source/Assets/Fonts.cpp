#include "Framework.h"
#include "Assets/Fonts.h"

namespace Silent::Assets
{
    const std::vector<FontMetadata> FONTS_METADATA
    {
        FontMetadata
        {
            .Name      = "Smooth",
            .Filenames = { "SilentSmooth.ttf" },
            .PointSize = 48
        },
        FontMetadata
        {
            .Name      = "Retro",
            .Filenames =
            {
                "Times_16.ttf",
                //"PsxBios.ttf",
                "UnifontExMono.ttf"
            },
            .PointSize = 32
        }
    };
}
