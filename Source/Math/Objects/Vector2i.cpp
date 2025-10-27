#include "Framework.h"
#include "Math/Objects/Vector2i.h"

#include "Math/Constants.h"
#include "Math/Objects/Matrix.h"
#include "Math/Objects/Vector2.h"

namespace Silent::Math
{
    const Vector2i Vector2i::Zero  = Vector2i(0, 0);
    const Vector2i Vector2i::One   = Vector2i(1, 1);
    const Vector2i Vector2i::UnitX = Vector2i(1, 0);
    const Vector2i Vector2i::UnitY = Vector2i(0, 1);

    float Vector2i::Length() const
    {
        return ToVector2().Length();
    }

    float Vector2i::LengthSquared() const
    {
        return ToVector2().LengthSquared();
    }

    float Vector2i::Distance(const Vector2i& from, const Vector2i& to)
    {
        return Vector2::Distance(from.ToVector2(), to.ToVector2());
    }

    float Vector2i::DistanceSquared(const Vector2i& from, const Vector2i& to)
    {
        return Vector2::DistanceSquared(from.ToVector2(), to.ToVector2());
    }

    Vector2i Vector2i::Min(const Vector2i& vec0, const Vector2i& vec1)
    {
        return glm::min(vec0.ToGlmVec2i(), vec1.ToGlmVec2i());
    }

    void Vector2i::Min(const Vector2i& vec)
    {
        *this = Vector2i::Min(*this, vec);
    }

    Vector2i Vector2i::Max(const Vector2i& vec0, const Vector2i& vec1)
    {
        return glm::max(vec0.ToGlmVec2i(), vec1.ToGlmVec2i());
    }

    void Vector2i::Max(const Vector2i& vec)
    {
        *this = Vector2i::Max(*this, vec);
    }

    Vector2i Vector2i::Clamp(const Vector2i& vec, const Vector2i& min, const Vector2i& max)
    {
        return Vector2i(glm::clamp(vec.ToGlmVec2i(), min.ToGlmVec2i(), max.ToGlmVec2i()));
    }

    void Vector2i::Clamp(const Vector2i& min, const Vector2i& max)
    {
        *this = Vector2i::Clamp(*this, min, max);
    }

    Vector2i Vector2i::Lerp(const Vector2i& from, const Vector2i& to, float alpha)
    {
        auto fromFlt = glm::vec2(from.ToGlmVec2i());
        auto toFlt   = glm::vec2(to.ToGlmVec2i());

        auto vecFlt = glm::lerp(fromFlt, toFlt, alpha);
        return Vector2i(glm::ivec2(round(vecFlt)));
    }

    void Vector2i::Lerp(const Vector2i& to, float alpha)
    {
        *this = Vector2i::Lerp(*this, to, alpha);
    }

    Vector2i Vector2i::Transform(const Vector2i& vec, const Matrix& mat)
    {
        return Vector2i(glm::ivec2(mat.ToGlmMat4() * glm::ivec4(vec, 0, 1)));
    }

    void Vector2i::Transform(const Matrix& mat)
    {
        *this = Vector2i::Transform(*this, mat);
    }

    Vector2i Vector2i::Translate(const Vector2i& vec, const Vector2& dir, float dist)
    {
        return vec + Vector2i(dir * dist);
    }

    void Vector2i::Translate(const Vector2& dir, float dist)
    {
        *this = Vector2i::Translate(*this, Vector2::Normalize(dir), dist);
    }

    Vector2 Vector2i::ToVector2() const
    {
        return Vector2((float)x, (float)y);
    }

    const glm::ivec2& Vector2i::ToGlmVec2i() const
    {
        return *(const glm::ivec2*)this;
    }

    glm::ivec2& Vector2i::ToGlmVec2i()
    {
        return *(glm::ivec2*)this;
    }

    bool Vector2i::operator==(const Vector2i& vec) const
    {
        return ToGlmVec2i() == vec.ToGlmVec2i();
    }

    bool Vector2i::operator!=(const Vector2i& vec) const
    {
        return ToGlmVec2i() != vec.ToGlmVec2i();
    }

    Vector2i& Vector2i::operator+=(const Vector2i& vec)
    {
        ToGlmVec2i() += vec.ToGlmVec2i();
        return *this;
    }

    Vector2i& Vector2i::operator-=(const Vector2i& vec)
    {
        ToGlmVec2i() -= vec.ToGlmVec2i();
        return *this;
    }

    Vector2i& Vector2i::operator*=(const Vector2i& vec)
    {
        ToGlmVec2i() *= vec.ToGlmVec2i();
        return *this;
    }

    Vector2i& Vector2i::operator*=(float scalar)
    {
        auto vecFloat = glm::vec2(ToGlmVec2i()) * scalar;
        *this         = Vector2i(round(vecFloat));
        return *this;
    }

    Vector2i& Vector2i::operator/=(float scalar)
    {
        auto vecFloat = glm::vec2(ToGlmVec2i()) / scalar;
        *this         = Vector2i(round(vecFloat));
        return *this;
    }

    Vector2i Vector2i::operator+(const Vector2i& vec) const
    {
        return Vector2i(ToGlmVec2i() + vec.ToGlmVec2i());
    }

    Vector2i Vector2i::operator-(const Vector2i& vec) const
    {
        return Vector2i(ToGlmVec2i() - vec.ToGlmVec2i());
    }

    Vector2i Vector2i::operator*(const Vector2i& vec) const
    {
        return Vector2i(ToGlmVec2i() * vec.ToGlmVec2i());
    }

    Vector2i Vector2i::operator*(float scalar) const
    {
        auto vecFlt = glm::vec2(ToGlmVec2i()) * scalar;
        return Vector2i(round(vecFlt));
    }

    Vector2i Vector2i::operator/(float scalar) const
    {
        auto vecFlt = glm::vec2(ToGlmVec2i()) / scalar;
        return Vector2i(round(vecFlt));
    }

    Vector2i Vector2i::operator-() const
    {
        return Vector2i(-ToGlmVec2i());
    }
}
