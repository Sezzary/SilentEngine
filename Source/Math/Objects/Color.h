#pragma once

#include "Math/Objects/Vector4.h"

namespace Silent::Math
{
    /** @brief Normalized float-based RGBA color. */
    class Color : Vector4
    {
    public:
        // ========
        // Presets
        // ========

        static const Color Clear;
        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;

        // =============
        // Constructors
        // =============

        /** @brief Constructs a default `Color`. */
        constexpr Color() : Vector4(0.0f, 0.0f, 0.0f, 1.0f) {}

        /** @brief Constructs a `Color` from normalized RGBA components.
         *
         * @param r Red component.
         * @param g Green component.
         * @param b Blue component.
         * @param a Alpha component.
         */
        constexpr Color(float r, float g, float b, float a = 1.0f) : Vector4(r, g, b, a) {}

        /** @brief Constructs a `Color` from 8-bit RGBA components.
         *
         * @param r Red component.
         * @param g Green component.
         * @param b Blue component.
         * @param a Alpha component.
         */
        static constexpr Color From8Bit(uchar r, uchar g, uchar b, uchar a = FP_COLOR(1.0f))
        {
            return Color(FP_COLOR_FROM(r), FP_COLOR_FROM(g), FP_COLOR_FROM(b), FP_COLOR_FROM(a));
        }

        /** @brief Constructs a `Color` from packed 8-bit RGBA components.
         *
         * @param rgba Packed RGBA.
         */
        static constexpr Color FromPacked8Bit(uint packedRgba)
        {
            return Color::From8Bit((packedRgba >> 24) & FP_COLOR(1.0f),
                                   (packedRgba >> 16) & FP_COLOR(1.0f),
                                   (packedRgba >> 8)  & FP_COLOR(1.0f),
                                    packedRgba        & FP_COLOR(1.0f));
        }

        // ========
        // Getters
        // ========

        /** @brief Gets the real-only red component.
         *
         * @return Read-only red component
         */
        const float& R() const;

        /** @brief Gets the writable red component.
         *
         * @return Writable red component
         */
        float& R();

        /** @brief Gets the real-only green component.
         *
         * @return Read-only green component
         */
        const float& G() const;

        /** @brief Gets the writable green component.
         *
         * @return Writable green component
         */
        float& G();

        /** @brief Gets the real-only blue component.
         *
         * @return Read-only blue component
         */
        const float& B() const;

        /** @brief Gets the writable blue component.
         *
         * @return Writable blue component
         */
        float& B();

        /** @brief Gets the real-only alpha component.
         *
         * @return Read-only alpha component
         */
        const float& A() const;

        /** @brief Gets the writable alpha component.
         *
         * @return Writable alpha component
         */
        float& A();

        /** @brief Gets the red component converted to 8-bit.
         *
         * @return 8-bit red component.
         */
        uchar R8() const;

        /** @brief Gets the green component converted to 8-bit.
         *
         * @return 8-bit green component.
         */
        uchar G8() const;

        /** @brief Gets the blue component converted to 8-bit.
         *
         * @return 8-bit blue component.
         */
        uchar B8() const;

        /** @brief Gets the alpha component converted to 8-bit.
         *
         * @return 8-bit alpha component.
         */
        uchar A8() const;

        // ==========
        // Utilities
        // ==========

        static Color Lerp(const Color& color0, const Color& color1, float alpha);

        void Lerp(const Color& color, float alpha);

        /** @brief Inverts a color.
         *
         * @param color Color to invert.
         * @return Inverted color.
         */
        static Color Invert(const Color& color);

        /** @brief Inverts the color. */
        void Invert();

        static Color Blend(const Color& color0, const Color& color1, float alpha);

        void Blend(const Color& color, float alpha);

        /** @brief Brightens a color by a given factor.
         *
         * @param color Color to brighten.
         * @param factor Brightening factor.
         * @return Brightened color.
         */
        static Color Brighten(const Color& color, float factor);

        /** @brief Brightens the color by a given factor.
         *
         * @param factor Brightening factor.
         */
        void Brighten(float factor);

        /** @brief Darkens a color by a given factor.
         *
         * @param color Color to brighten.
         * @param factor Darkening factor.
         * @return Darkened color.
         */
        static Color Darken(const Color& color, float factor);

        /** @brief Darkens the color by a given factor.
         *
         * @param factor Darkening factor.
         */
        void Darken(float factor);

        // ===========
        // Converters
        // ===========

        /** @brief Converts the color to packed 8-bit components.
         *
         * @return Packed 8-bit components.
         */
        uint ToPacked8Bit() const;

        const Vector4& ToVector4() const;
        Vector4& ToVector4();
        const glm::vec4& ToGlmVec4() const;
        glm::vec4& ToGlmVec4();

        // ==========
        // Operators
        // ==========

        bool   operator ==(const Color& color) const;
        bool   operator !=(const Color& color) const;
        Color& operator =(const Color& color) = default;
    };
}
