#pragma once

namespace Silent::Game
{
    /** @brief Graphics options menu entries. */
    enum class GraphicsMenuEntry
    {
        Fullscreen,
        BrightnessLevel,
        FrameRate,
        AspectRatio,
        RenderScale,
        TextureFilter,
        TextQuality,
        Lighting,
        Antialiasing,
        DitheringScale,
        AmbientOcclusion,
        VertexJitter,
        FilmGrain,
        Vignette,
        CrtFilter,

        Count
    };

    /** @brief Controller for the graphics options menu. */
    void ControlGraphicsOptionsMenu();
}
