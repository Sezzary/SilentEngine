#include "Framework.h"
#include "Assets/Fonts.h"

#include "Utils/Font.h"

using namespace Silent::Utils;

namespace Silent::Assets
{
    // @todo Define these in a Lua script instead.
    const std::vector<FontMetadata> FONTS_METADATA
    {
        {
            .Name      = "ModernSerif",
            .Filenames =
            {
                "FreeSerif.ttf",
                "NotoSerifJP-Medium.ttf",
                "NotoSerifKR-Medium.ttf"
            },
            .PointSize = 64,
            .Trackings =
            {
                0.1f,
                0.0f,
                0.0f
            },
            .EnableAntialiasing = true
        },
        {
            .Name      = "RetroSerif",
            .Filenames =
            {
                "Times_16.ttf",
                "PsxBios.ttf",
                "UnifontExMono.ttf"
            },
            .PointSize = 16,
            .Trackings =
            {
                1.0f / 16.0f,
                1.0f / 16.0f,
                1.0f / 16.0f
            },
            .EnableAntialiasing = false
        },
        {
            .Name      = "ModernSans",
            .Filenames =
            {
                "NotoSans-Bold.ttf",
                "NotoSansJP-Bold.ttf",
                "NotoSansKR-Bold.ttf"
            },
            .PointSize = 40,
            .Trackings =
            {
                0.0f,
                0.0f,
                0.0f
            },
            .EnableAntialiasing = true
        },
        {
            .Name      = "RetroSans",
            .Filenames =
            {
                "SilentControl10.ttf",
                "PixelMplus10-Regular.ttf",
                "dalmoori10.ttf"
            },
            .PointSize = 10,
            .Trackings =
            {
                1.0f / 10.0f,
                1.0f / 10.0f,
                1.0f / 10.0f
            },
            .EnableAntialiasing = false
        },
        {
            .Name      = "Handwriting",
            .Filenames = 
            {
                "FreeSerifItalic.ttf"
                // @todo Need others for Japanese and Korean.
            },
            .PointSize = 64,
            .Trackings =
            {
                0.0f,
                0.0f,
                0.0f
            },
            .EnableAntialiasing = true
        }
    };
}
