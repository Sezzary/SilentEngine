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

    FontGlyphUvs GetFont8GlyphUvs(char charCode);
    FontGlyphUvs GetFont16GlyphUvs(char charCode);

    int GetFont16GlyphPixelWidth(char charCode);
}
