#include "Framework.h"
#include "Math/Objects/Vector2.h"

#include "Math/Constants.h"
#include "Math/Objects/Matrix.h"
#include "Math/Objects/Vector2i.h"

namespace Silent::Math
{
    const Vector2 Vector2::Zero  = Vector2(0.0f, 0.0f);
    const Vector2 Vector2::One   = Vector2(1.0f, 1.0f);
    const Vector2 Vector2::UnitX = Vector2(1.0f, 0.0f);
    const Vector2 Vector2::UnitY = Vector2(0.0f, 1.0f);

    float Vector2::Length() const
    {
        return glm::length(ToGlmVec2());
    }

    float Vector2::LengthSquared() const
    {
        return glm::length2(ToGlmVec2());
    }

    float Vector2::Distance(const Vector2& from, const Vector2& to)
    {
        return glm::distance(from.ToGlmVec2(), to.ToGlmVec2());
    }

    float Vector2::DistanceSquared(const Vector2& from, const Vector2& to)
    {
        return glm::distance2(from.ToGlmVec2(), to.ToGlmVec2());
    }

    float Vector2::Dot(const Vector2& vec0, const Vector2& vec1)
    {
        return glm::dot(vec0.ToGlmVec2(), vec1.ToGlmVec2());
    }

    float Vector2::Cross(const Vector2& vec0, const Vector2& vec1)
    {
        return (vec0.x * vec1.y) - (vec0.y * vec1.x);
    }

    Vector2 Vector2::Min(const Vector2& vec0, const Vector2& vec1)
    {
        return glm::min(vec0.ToGlmVec2(), vec1.ToGlmVec2());
    }

    void Vector2::Min(const Vector2& vec)
    {
        *this = Vector2::Min(*this, vec);
    }

    Vector2 Vector2::Max(const Vector2& vec0, const Vector2& vec1)
    {
        return glm::max(vec0.ToGlmVec2(), vec1.ToGlmVec2());
    }

    void Vector2::Max(const Vector2& vec)
    {
        *this = Vector2::Max(*this, vec);
    }

    Vector2 Vector2::Clamp(const Vector2& vec, const Vector2& min, const Vector2& max)
    {
        return Vector2(glm::clamp(vec.ToGlmVec2(), min.ToGlmVec2(), max.ToGlmVec2()));
    }

    void Vector2::Clamp(const Vector2& min, const Vector2& max)
    {
        *this = Vector2::Clamp(*this, min, max);
    }

    Vector2 Vector2::Normalize(const Vector2& vec)
    {
        return Vector2(glm::normalize(vec.ToGlmVec2()));
    }

    void Vector2::Normalize()
    {
        *this = Vector2::Normalize(*this);
    }

    Vector2 Vector2::Lerp(const Vector2& from, const Vector2& to, float alpha)
    {
        return Vector2(glm::lerp(from.ToGlmVec2(), to.ToGlmVec2(), alpha));
    }

    void Vector2::Lerp(const Vector2& to, float alpha)
    {
        *this = Vector2::Lerp(*this, to, alpha);
    }

    Vector2 Vector2::Transform(const Vector2& vec, const Matrix& mat)
    {
        return Vector2(glm::vec2(mat.ToGlmMat4() * glm::vec4(vec, 0.0f, 1.0f)));
    }

    void Vector2::Transform(const Matrix& mat)
    {
        *this = Vector2::Transform(*this, mat);
    }

    Vector2 Vector2::Translate(const Vector2& vec, const Vector2& dir, float dist)
    {
        return vec + (dir * dist);
    }

    void Vector2::Translate(const Vector2& dir, float dist)
    {
        *this = Vector2::Translate(*this, Vector2::Normalize(dir), dist);
    }

    bool Vector2::Compare(const Vector2& vec0, const Vector2& vec1, float epsilon)
    {
        return Vector2::DistanceSquared(vec0, vec1) <= SQUARE(epsilon);
    }

    Vector2i Vector2::ToVector2i(int shift) const
    {
        return (shift != NO_VALUE) ? Vector2i(FP_TO(x, shift), FP_TO(y, shift)) :
                                     Vector2i((int)std::round(x), (int)std::round(y));
    }

    const glm::vec2& Vector2::ToGlmVec2() const
    {
        return *(const glm::vec2*)this;
    }

    glm::vec2& Vector2::ToGlmVec2()
    {
        return *(glm::vec2*)this;
    }

    bool Vector2::operator==(const Vector2& vec) const
    {
        return ToGlmVec2() == vec.ToGlmVec2();
    }

    bool Vector2::operator!=(const Vector2& vec) const
    {
        return ToGlmVec2() != vec.ToGlmVec2();
    }

    Vector2& Vector2::operator+=(const Vector2& vec)
    {
        ToGlmVec2() += vec.ToGlmVec2();
        return *this;
    }

    Vector2& Vector2::operator-=(const Vector2& vec)
    {
        ToGlmVec2() -= vec.ToGlmVec2();
        return *this;
    }

    Vector2& Vector2::operator*=(const Vector2& vec)
    {
        ToGlmVec2() *= vec.ToGlmVec2();
        return *this;
    }

    Vector2& Vector2::operator*=(float scalar)
    {
        ToGlmVec2() *= scalar;
        return *this;
    }

    Vector2& Vector2::operator/=(const Vector2& vec)
    {
        ToGlmVec2() /= vec.ToGlmVec2();
        return *this;
    }

    Vector2& Vector2::operator/=(float scalar)
    {
        ToGlmVec2() /= scalar;
        return *this;
    }

    Vector2 Vector2::operator+(const Vector2& vec) const
    {
        return Vector2(ToGlmVec2() + vec.ToGlmVec2());
    }

    Vector2 Vector2::operator-(const Vector2& vec) const
    {
        return Vector2(ToGlmVec2() - vec.ToGlmVec2());
    }

    Vector2 Vector2::operator*(const Vector2& vec) const
    {
        return Vector2(ToGlmVec2() * vec.ToGlmVec2());
    }

    Vector2 Vector2::operator*(float scalar) const
    {
        return Vector2(ToGlmVec2() * scalar);
    }

    Vector2 Vector2::operator/(const Vector2& vec) const
    {
        return Vector2(ToGlmVec2() / vec.ToGlmVec2());
    }

    Vector2 Vector2::operator/(float scalar) const
    {
        return Vector2(ToGlmVec2() / scalar);
    }

    Vector2 Vector2::operator-() const
    {
        return Vector2(-ToGlmVec2());
    }
}
