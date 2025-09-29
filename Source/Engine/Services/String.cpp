#include "Framework.h"
#include "Engine/Services/String.h"

#include "Engine/Application.h"
#include "Engine/Assets/Assets.h"
#include "Engine/Assets/Parsers/Tim.h"

namespace Silent::Services
{
    constexpr char FONT_8_X_8_ASSET_NAME[]   = "1ST/FONT8.TIM";
    constexpr char FONT_12_X_16_ASSET_NAME[] = "1ST/FONT16.TIM";

    static const auto FONT_8_X_8_GLYPH_COORDS = std::unordered_map<char, Vector2i> // Key = character code, value = coordinate in `TIM/FONT8.TIM` 8x8 glyph atlas.
    {
        { '*', Vector2i(0,  0) },
        { '+', Vector2i(1,  0) },
        { ',', Vector2i(2,  0) },
        { '-', Vector2i(3,  0) },
        { '.', Vector2i(4,  0) },
        { '/', Vector2i(5,  0) },
        { '0', Vector2i(6,  0) },
        { '1', Vector2i(7,  0) },
        { '2', Vector2i(8,  0) },
        { '3', Vector2i(9,  0) },
        { '4', Vector2i(10, 0) },
        { '5', Vector2i(11, 0) },
        { '6', Vector2i(12, 0) },
        { '7', Vector2i(13, 0) },
        { '8', Vector2i(14, 0) },
        { '9', Vector2i(15, 0) },
        { ':', Vector2i(16, 0) },
        { ';', Vector2i(17, 0) },
        { '<', Vector2i(18, 0) },
        { '=', Vector2i(19, 0) },
        { '>', Vector2i(20, 0) },
        { '?', Vector2i(21, 0) },
        { '@', Vector2i(22, 0) }, // "©".
        { 'A', Vector2i(23, 0) },
        { 'B', Vector2i(24, 0) },
        { 'C', Vector2i(25, 0) },
        { 'D', Vector2i(26, 0) },
        { 'E', Vector2i(27, 0) },
        { 'F', Vector2i(28, 0) },
        { 'G', Vector2i(29, 0) },
        { 'H', Vector2i(30, 0) },
        { 'I', Vector2i(31, 0) },
        { 'J', Vector2i(0,  1) },
        { 'K', Vector2i(1,  1) },
        { 'L', Vector2i(2,  1) },
        { 'M', Vector2i(3,  1) },
        { 'N', Vector2i(4,  1) },
        { 'O', Vector2i(5,  1) },
        { 'P', Vector2i(6,  1) },
        { 'Q', Vector2i(7,  1) },
        { 'R', Vector2i(8,  1) },
        { 'S', Vector2i(9,  1) },
        { 'T', Vector2i(10, 1) },
        { 'U', Vector2i(11, 1) },
        { 'V', Vector2i(12, 1) },
        { 'W', Vector2i(13, 1) },
        { 'X', Vector2i(14, 1) },
        { 'Y', Vector2i(15, 1) },
        { 'Z', Vector2i(16, 1) },
        { '^', Vector2i(17, 1) },
        { '_', Vector2i(18, 1) }
    };

    static const auto FONT_12_X_16_GLYPH_COORDS = std::unordered_map<char, Vector2i> // Key = character code, value = coordinate in `1ST/FONT16.TIM` 12x16 glyph atlas.
    {
        { '\'', Vector2i(0, 0)  },
        { '(',  Vector2i(0, 1)  },
        { ')',  Vector2i(0, 2)  },
        { '*',  Vector2i(0, 3)  },  // "=".
        { '+',  Vector2i(0, 4)  },
        { ',',  Vector2i(0, 5)  },
        { '-',  Vector2i(0, 6)  },
        { '.',  Vector2i(0, 7)  },
        { '/',  Vector2i(0, 8)  },
        { '0',  Vector2i(0, 9)  },
        { '1',  Vector2i(0, 10) },
        { '2',  Vector2i(0, 11) },
        { '3',  Vector2i(0, 12) },
        { '4',  Vector2i(0, 13) },
        { '5',  Vector2i(0, 14) },
        { '6',  Vector2i(0, 15) },
        { '7',  Vector2i(0, 16) },
        { '8',  Vector2i(0, 17) },
        { '9',  Vector2i(0, 18) },
        { ':',  Vector2i(0, 19) },
        { ';',  Vector2i(0, 20) },
        { '<',  Vector2i(0, 21) },
        { '=',  Vector2i(0, 22) }, // "-".
        { '>',  Vector2i(0, 23) },
        { '?',  Vector2i(0, 24) },
        { '@',  Vector2i(0, 25) }, // Star.
        { 'A',  Vector2i(0, 26) },
        { 'B',  Vector2i(0, 27) },
        { 'C',  Vector2i(0, 28) },
        { 'D',  Vector2i(0, 29) },
        { 'E',  Vector2i(0, 30) },
        { 'F',  Vector2i(0, 31) },
        { 'G',  Vector2i(0, 32) },
        { 'H',  Vector2i(0, 33) },
        { 'I',  Vector2i(0, 34) },
        { 'J',  Vector2i(0, 35) },
        { 'K',  Vector2i(0, 36) },
        { 'L',  Vector2i(0, 37) },
        { 'M',  Vector2i(0, 38) },
        { 'N',  Vector2i(0, 39) },
        { 'O',  Vector2i(0, 40) },
        { 'P',  Vector2i(0, 41) },
        { 'Q',  Vector2i(0, 42) },
        { 'R',  Vector2i(0, 43) },
        { 'S',  Vector2i(0, 44) },
        { 'T',  Vector2i(0, 45) },
        { 'U',  Vector2i(0, 46) },
        { 'V',  Vector2i(0, 47) },
        { 'W',  Vector2i(0, 48) },
        { 'X',  Vector2i(0, 49) },
        { 'Y',  Vector2i(0, 50) },
        { 'Z',  Vector2i(0, 51) },
        { '[',  Vector2i(0, 52) }, // Left double-quote.
        { '\\', Vector2i(0, 53) }, // "!".
        { ']',  Vector2i(0, 54) }, // Right double-quote.
        { '^',  Vector2i(0, 55) }, // "&".
        { '_',  Vector2i(0, 56) }, // Space.
        { '`',  Vector2i(0, 57) },
        { 'a',  Vector2i(0, 58) },
        { 'b',  Vector2i(0, 59) },
        { 'c',  Vector2i(0, 60) },
        { 'd',  Vector2i(0, 61) },
        { 'e',  Vector2i(0, 62) },
        { 'f',  Vector2i(0, 63) },
        { 'g',  Vector2i(0, 64) },
        { 'h',  Vector2i(0, 65) },
        { 'i',  Vector2i(0, 66) },
        { 'j',  Vector2i(0, 67) },
        { 'k',  Vector2i(0, 68) },
        { 'l',  Vector2i(0, 69) },
        { 'm',  Vector2i(0, 70) },
        { 'n',  Vector2i(0, 71) },
        { 'o',  Vector2i(0, 72) },
        { 'p',  Vector2i(0, 73) },
        { 'q',  Vector2i(0, 74) },
        { 'r',  Vector2i(0, 75) },
        { 's',  Vector2i(0, 76) },
        { 't',  Vector2i(0, 77) },
        { 'u',  Vector2i(0, 78) },
        { 'v',  Vector2i(0, 79) },
        { 'w',  Vector2i(0, 80) },
        { 'x',  Vector2i(0, 81) },
        { 'y',  Vector2i(0, 82) },
        { 'z',  Vector2i(0, 83) }
    };

    static const auto FONT_12_X_16_GLYPH_PIXEL_WIDTHS = std::unordered_map<char, int> // Key = character code, value = glyph pixel width.
    {
        { '\'', 3  },
        { '(',  7  },
        { ')',  7  },
        { '*',  11 }, // "=".
        { '+',  11 },
        { ',',  4  },
        { '-',  10 },
        { '.',  4  },
        { '/',  6  },
        { '0',  10 },
        { '1',  10 },
        { '2',  10 },
        { '3',  10 },
        { '4',  10 },
        { '5',  10 },
        { '6',  10 },
        { '7',  10 },
        { '8',  10 },
        { '9',  10 },
        { ':',  4  },
        { ';',  4  },
        { '<',  10 },
        { '=',  11 }, // "-".
        { '>',  10 },
        { '?',  8  },
        { '@',  13 }, // Star.
        { 'A',  12 },
        { 'B',  12 },
        { 'C',  12 },
        { 'D',  13 },
        { 'E',  11 },
        { 'F',  11 },
        { 'G',  13 },
        { 'H',  12 },
        { 'I',  9  },
        { 'J',  9  },
        { 'K',  12 },
        { 'L',  12 },
        { 'M',  13 },
        { 'N',  12 },
        { 'O',  13 },
        { 'P',  11 },
        { 'Q',  13 },
        { 'R',  12 },
        { 'S',  10 },
        { 'T',  11 },
        { 'U',  13 },
        { 'V',  12 },
        { 'W',  12 },
        { 'X',  12 },
        { 'Y',  11 },
        { 'Z',  12 },
        { '[',  6  }, // Left double-quote.
        { '\\', 4  }, // "!".
        { ']',  6  }, // Right double-quote.
        { '^',  8  }, // "&".
        { '_',  0  }, // Space (unused).
        { '`',  3  },
        { 'a',  9  },
        { 'b',  10 },
        { 'c',  9  },
        { 'd',  9  },
        { 'e',  9  },
        { 'f',  7  },
        { 'g',  11 },
        { 'h',  11 },
        { 'i',  6  },
        { 'j',  6  },
        { 'k',  10 },
        { 'l',  6  },
        { 'm',  13 },
        { 'n',  11 },
        { 'o',  10 },
        { 'p',  11 },
        { 'q',  10 },
        { 'r',  8  },
        { 's',  8  },
        { 't',  7  },
        { 'u',  10 },
        { 'v',  10 },
        { 'w',  12 },
        { 'x',  10 },
        { 'y',  10 },
        { 'z',  9  }
    };

    FontGlyphUvs Get8x8FontGlyphUvs(char charCode)
    {
        constexpr auto GLYPH_RES       = Vector2i(8, 8);
        constexpr int  GLYPH_ROW_COUNT = 32;

        auto& assets = g_App.GetAssets();

        // Get asset.
        const auto asset = assets.GetAsset(FONT_8_X_8_ASSET_NAME);
        if (asset == nullptr || asset->State != AssetState::Loaded)
        {
            Log(std::string("Failed to get glyph UVs from unloaded asset `") + FONT_8_X_8_ASSET_NAME + "`.", LogLevel::Warning);
            return {};
        }
        auto data = GetAssetData<TimAsset>(asset);

        // Get glyph coords in atlas.
        auto coordsIt = FONT_8_X_8_GLYPH_COORDS.find(charCode);
        if (coordsIt == FONT_8_X_8_GLYPH_COORDS.end())
        {
            Log("Failed to get glyph UVs for char `" + std::string(1, charCode) + "` from asset + `" + FONT_8_X_8_ASSET_NAME + "`.", LogLevel::Warning);
            return {};
        }
        const auto& coords = coordsIt->second;

        // Compute base glyph pixel position.
        auto pixelPos = (coords * GLYPH_RES).ToVector2();

        // Compute glyph UVs.
        auto atlasRes = data->Resolution.ToVector2();
        auto uvs      = FontGlyphUvs
        {
            .Uv0 = pixelPos                                       / atlasRes,
            .Uv1 = (pixelPos + Vector2(0.0f, GLYPH_RES.y))        / atlasRes,
            .Uv2 = (pixelPos + Vector2(GLYPH_RES.x, GLYPH_RES.y)) / atlasRes,
            .Uv3 = (pixelPos + Vector2(GLYPH_RES.x, 0.0f))        / atlasRes,
        };
        return uvs;
    }

    FontGlyphUvs Get12x16FontGlyphUvs(char charCode)
    {
        constexpr auto GLYPH_RES                = Vector2i(12, 16);
        constexpr int  GLYPH_ROW_PIXEL_OFFSET_X = 4;
        constexpr int  GLYPH_ROW_COUNT          = 21;

        auto& assets = g_App.GetAssets();

        // Get asset.
        const auto asset = assets.GetAsset(FONT_12_X_16_ASSET_NAME);
        if (asset == nullptr || asset->State != AssetState::Loaded)
        {
            Log("Failed to get glyph UVs for char `" + std::string(1, charCode) + "` from unloaded asset + `" + FONT_12_X_16_ASSET_NAME + "`.", LogLevel::Warning);
            return {};
        }
        auto data = GetAssetData<TimAsset>(asset);

        // Get glyph coords in atlas.
        auto coordsIt = FONT_12_X_16_GLYPH_COORDS.find(charCode);
        if (coordsIt == FONT_12_X_16_GLYPH_COORDS.end())
        {
            Log("Failed to get glyph UVs for char `" + std::string(1, charCode) + "` from asset + `" + FONT_12_X_16_ASSET_NAME + "`.", LogLevel::Warning);
            return {};
        }
        const auto& coords = coordsIt->second;

        // Compute base glyph pixel position.
        int  pixelOffsetX = (coords.x / GLYPH_ROW_COUNT) * GLYPH_ROW_PIXEL_OFFSET_X; // HACK: Required due to odd glyph layout in atlas.
        auto pixelPos     = Vector2i((coords.x * GLYPH_RES.x) + pixelOffsetX, 0).ToVector2();

        // Compute glyph UVs.
        auto atlasRes = data->Resolution.ToVector2();
        auto uvs      = FontGlyphUvs
        {
            .Uv0 = pixelPos                                       / atlasRes,
            .Uv1 = (pixelPos + Vector2(0.0f, GLYPH_RES.y))        / atlasRes,
            .Uv2 = (pixelPos + Vector2(GLYPH_RES.x, GLYPH_RES.y)) / atlasRes,
            .Uv3 = (pixelPos + Vector2(GLYPH_RES.x, 0.0f))        / atlasRes,
        };
        return uvs;
    }

    int Get12x16FontGlyphPixelWidth(char charCode)
    {
        // Get glyph pixel width.
        auto widthIt = FONT_12_X_16_GLYPH_PIXEL_WIDTHS.find(charCode);
        if (widthIt == FONT_12_X_16_GLYPH_PIXEL_WIDTHS.end())
        {
            Log("Failed to get glyph pixel width for char `" + std::string(1, charCode) + "` from asset + `" + FONT_12_X_16_ASSET_NAME + "`.", LogLevel::Warning);
            return 0;
        }
        int width = widthIt->second;

        return width;
    }

    int Get12x16FontStringPixelWidth(const std::string& str)
    {
        // Compute string pixel width.
        int width = 0;
        for (char charCode : str)
        {
            // Get glyph pixel width.
            auto widthIt = FONT_12_X_16_GLYPH_PIXEL_WIDTHS.find(charCode);
            if (widthIt == FONT_12_X_16_GLYPH_PIXEL_WIDTHS.end())
            {
                Log("Failed to get glyph pixel width for char `" + std::string(1, charCode) + "` from asset + `" + FONT_12_X_16_ASSET_NAME + "`.", LogLevel::Warning);
                continue;
            }
            int glyphWidth = widthIt->second;

            // Accumulate string pixel width.
            width += glyphWidth;
        }

        return width;
    }
}
