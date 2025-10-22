#pragma once

namespace Silent::Renderer
{
    enum class FontType
    {
        Debug
    };

    /** @brief Text align modes. */
    enum class TextAlignMode
    {
        Left,
        Right,
        Center
    };

    struct Text
    {
        std::string   Message   = {};
        TextAlignMode AlignMode = TextAlignMode::Left;
        Vector2       Position  = Vector2::Zero;
        float         Scale     = 0.0f;
        Color         Col       = Color::Clear;
    };
}
