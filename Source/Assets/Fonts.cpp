#include "Framework.h"
#include "Assets/Fonts.h"

namespace Silent::Assets
{
    /** @brief String color IDs for strings displayed in screen space.
     * Used as indices into `STRING_COLORS`.
     */
    enum class e_StringColorId
    {
        Gold,
        DarkGrey,
        Green,
        Nuclear,
        Red,
        LightGrey,
        White,

        Count
    };

    const std::array<Color, (int)e_StringColorId::Count> STRING_COLORS =
    {
        Color::From8Bit(160, 128, 64),
        Color::From8Bit(32,  32,  32),
        Color::From8Bit(24,  128, 40),
        Color::From8Bit(8,   184, 96),
        Color::From8Bit(128, 0,   0),
        Color::From8Bit(100, 100, 100),
        Color::From8Bit(128, 128, 128)
    };

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
