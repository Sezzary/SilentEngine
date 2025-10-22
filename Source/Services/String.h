#pragma once

namespace Silent::Services
{
    struct FontGlyphUvs
    {
        Vector2 Uv0 = Vector2::Zero;
        Vector2 Uv1 = Vector2::Zero;
        Vector2 Uv2 = Vector2::Zero;
        Vector2 Uv3 = Vector2::Zero;
    };

    FontGlyphUvs Get8x8FontGlyphUvs(char charCode);
    FontGlyphUvs Get12x16FontGlyphUvs(char charCode);

    int Get12x16FontGlyphPixelWidth(char charCode);
    int Get12x16FontStringPixelWidth(const std::string& str);
}
