#include "Framework.h"
#include "Math/Objects/Color.h"

#include "Math/Objects/Vector4.h"

namespace Silent::Math
{
    const Color Color::Clear = Color(0.0f, 0.0f, 0.0f, 0.0f);
    const Color Color::Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
    const Color Color::White = Color(1.0f, 1.0f, 1.0f, 1.0f);
    const Color Color::Red   = Color(1.0f, 0.0f, 0.0f, 1.0f);
    const Color Color::Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
    const Color Color::Blue  = Color(0.0f, 0.0f, 1.0f, 1.0f);

    const float& Color::R() const
    {
        return x;
    }

    float& Color::R()
    {
        return x;
    }

    const float& Color::G() const
    {
        return y;
    }

    float& Color::G()
    {
        return y;
    }

    const float& Color::B() const
    {
        return z;
    }

    float& Color::B()
    {
        return z;
    }

    const float& Color::A() const
    {
        return w;
    }

    float& Color::A()
    {
        return w;
    }

    uchar Color::R8() const
    {
        return FP_COLOR(R());
    }

    uchar Color::G8() const
    {
        return FP_COLOR(G());
    }

    uchar Color::B8() const
    {
        return FP_COLOR(B());
    }

    uchar Color::A8() const
    {
        return FP_COLOR(A());
    }

    Color Color::Lerp(const Color& color0, const Color& color1, float alpha)
    {
        return {}; // @todo
        //return Color(glm::lerp(color0.ToGlmVec4(), color1.ToGlmVec4(), alpha));
    }

    void Color::Lerp(const Color& color, float alpha)
    {
        *this = Color::Lerp(*this, color, alpha);
    }

    Color Color::Invert(const Color& color)
    {
        return Color(1.0f - color.R(),
                     1.0f - color.G(),
                     1.0f - color.B(),
                     1.0f - color.A());
    }

    void Color::Invert()
    {
        *this = Color::Invert(*this);
    }

    Color Color::Blend(const Color& color0, const Color& color1, float alpha)
    {
        auto invAlpha = 1.0f - alpha;
        return Color((color0.R() * invAlpha) + (color1.R() * alpha),
                     (color0.G() * invAlpha) + (color1.G() * alpha),
                     (color0.B() * invAlpha) + (color1.B() * alpha),
                     (color0.A() * invAlpha) + (color1.A() * alpha));
    }

    void Color::Blend(const Color& color, float alpha)
    {
        *this = Color::Blend(*this, color, alpha);
    }

    Color Color::Brighten(const Color& color, float factor)
    {
        return Color(std::min(color.R() * factor, 1.0f),
                     std::min(color.G() * factor, 1.0f),
                     std::min(color.B() * factor, 1.0f),
                     color.A());
    }

    void Color::Brighten(float alpha)
    {
        *this = Color::Brighten(*this, alpha);
    }

    Color Color::Darken(const Color& color, float factor)
    {
        return Color(color.R() * factor,
                     color.G() * factor,
                     color.B() * factor,
                     color.A());
    }

    void Color::Darken(float alpha)
    {
        *this = Color::Darken(*this, alpha);
    }

    uint Color::ToPacked8Bit() const
    {
        return (FP_COLOR(R()) << 24) | (FP_COLOR(G()) << 16) | (FP_COLOR(B()) << 8) | FP_COLOR(A());
    }

    const Vector4& Color::ToVector4() const
    {
        return *(const Vector4*)this;
    }

    Vector4& Color::ToVector4()
    {
        return *(Vector4*)this;
    }

    const glm::vec4& Color::ToGlmVec4() const
    {
        return *(const glm::vec4*)this;
    }

    glm::vec4& Color::ToGlmVec4()
    {
        return *(glm::vec4*)this;
    }

    bool Color::operator ==(const Color& color) const
    {
        return R() == color.R() && G() == color.G() && B() == color.B() && A() == color.A();
    }

    bool Color::operator !=(const Color& color) const
    {
        return !(*this == color);
    }
}
