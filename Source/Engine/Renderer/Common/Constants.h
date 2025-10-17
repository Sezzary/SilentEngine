#pragma once

namespace Silent::Renderer
{
    constexpr auto SCREEN_SPACE_RES           = Vector2(100.0f);
    constexpr auto RETRO_SCREEN_SPACE_RES     = Vector2(320.0f, 240.0f);
    constexpr char SCREENSHOT_FILENAME_BASE[] = "Screenshot_";

    constexpr uint PRIMITIVE_2D_COUNT_MAX = 256;
    constexpr uint SPRITE_2D_COUNT_MAX    = 256;
    constexpr uint PRIMITIVE_3D_COUNT_MAX = 256;
}
