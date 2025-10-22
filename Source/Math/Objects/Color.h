#pragma once

#include "Math/Constants.h"

namespace Silent::Math
{
    class Vector4;

    // @todo Use `Vector4`?

    /** @brief Normalized float-based RGBA color. */
    class Color : glm::vec4
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
        constexpr Color() : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) {}

        // @todo
        //constexpr Color(const glm::vec4& color) : glm::vec4(color) {}

        /** @brief Constructs a `Color` from `float` RGBA components.
         *
         * @param r Red component.
         * @param g Green component.
         * @param b Blue component.
         * @param a Alpha component.
         */
        constexpr Color(float r, float g, float b, float a = 1.0f) : glm::vec4(r, g, b, a) {}

        /** @brief Constructs a `Color` from 8-bit RGBA components.
         *
         * @param r Red component.
         * @param g Green component.
         * @param b Blue component.
         * @param a Alpha component.
         */
        static constexpr Color From8Bit(uchar r, uchar g, uchar b, uchar a = FP_COLOR(1.0f))
        {
            return Color(r / (float)FP_COLOR(1.0f),
                         g / (float)FP_COLOR(1.0f),
                         b / (float)FP_COLOR(1.0f),
                         a / (float)FP_COLOR(1.0f));
        }

        /** @brief Constructs a `Color` from packed 8-bit RGBA components.
         *
         * @param rgba Packed RGBA.
         */
        static constexpr Color FromPacked8Bit(uint packedRgba)
        {
            return Color::From8Bit((packedRgba >> 24) & FP_COLOR(1.0f),
                                   (packedRgba >> 16) & FP_COLOR(1.0f),
                                   (packedRgba >>  8) & FP_COLOR(1.0f),
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
        void         Lerp(const Color& color, float alpha);
        static Color Invert(const Color& color);
        void         Invert();
        static Color Blend(const Color& color0, const Color& color1, float alpha);
        void         Blend(const Color& color, float alpha);
        static Color Brighten(const Color& color, float factor);
        void         Brighten(float alpha);
        static Color Darken(const Color& color, float factor);
        void         Darken(float alpha);

        // ===========
        // Converters
        // ===========

        /** @brief Converts the color to packed 8-bit components.
         *
         * @return Packed 8-bit components.
         */
        uint ToPacked8Bit() const;

        const Vector4&   ToVector4() const;
        Vector4&         ToVector4();
        const glm::vec4& ToGlmVec4() const;
        glm::vec4&       ToGlmVec4();

        // ==========
        // Operators
        // ==========

        bool   operator ==(const Color& color) const;
        bool   operator !=(const Color& color) const;
        Color& operator =(const Color& color) = default;
    };
}
