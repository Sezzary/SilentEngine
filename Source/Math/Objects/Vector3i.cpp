#include "Framework.h"
#include "Math/Objects/Vector3i.h"

#include "Math/Constants.h"
#include "Math/FixedPoint.h"
#include "Math/Objects/Matrix.h"
#include "Math/Objects/Vector3.h"

namespace Silent::Math
{
    const Vector3i Vector3i::Zero  = Vector3i(0, 0, 0);
    const Vector3i Vector3i::One   = Vector3i(1, 1, 1);
    const Vector3i Vector3i::UnitX = Vector3i(1, 0, 0);
    const Vector3i Vector3i::UnitY = Vector3i(0, 1, 0);
    const Vector3i Vector3i::UnitZ = Vector3i(0, 0, 1);

    float Vector3i::Length() const
    {
        return ToVector3().Length();
    }

    float Vector3i::LengthSquared() const
    {
        return ToVector3().LengthSquared();
    }

    float Vector3i::Distance(const Vector3i& from, const Vector3i& to)
    {
        return Vector3::Distance(from.ToVector3(), to.ToVector3());
    }

    float Vector3i::DistanceSquared(const Vector3i& from, const Vector3i& to)
    {
        return Vector3::DistanceSquared(from.ToVector3(), to.ToVector3());
    }

    Vector3i Vector3i::Min(const Vector3i& vec0, const Vector3i& vec1)
    {
        return glm::min(vec0.ToGlmVec3i(), vec1.ToGlmVec3i());
    }

    void Vector3i::Min(const Vector3i& vec)
    {
        *this = Vector3i::Min(*this, vec);
    }

    Vector3i Vector3i::Max(const Vector3i& vec0, const Vector3i& vec1)
    {
        return glm::max(vec0.ToGlmVec3i(), vec1.ToGlmVec3i());
    }

    void Vector3i::Max(const Vector3i& vec)
    {
        *this = Vector3i::Max(*this, vec);
    }

    Vector3i Vector3i::Clamp(const Vector3i& vec, const Vector3i& min, const Vector3i& max)
    {
        return Vector3i(glm::clamp(vec.ToGlmVec3i(), min.ToGlmVec3i(), max.ToGlmVec3i()));
    }

    void Vector3i::Clamp(const Vector3i& min, const Vector3i& max)
    {
        *this = Vector3i::Clamp(*this, min, max);
    }

    Vector3i Vector3i::Lerp(const Vector3i& from, const Vector3i& to, float alpha)
    {
        auto fromFlt = glm::vec3(from.ToGlmVec3i());
        auto toFlt   = glm::vec3(to.ToGlmVec3i());

        auto vecFlt = glm::lerp(fromFlt, toFlt, alpha);
        return Vector3i(glm::ivec3(round(vecFlt)));
    }

    void Vector3i::Lerp(const Vector3i& to, float alpha)
    {
        *this = Vector3i::Lerp(*this, to, alpha);
    }

    Vector3i Vector3i::Transform(const Vector3i& vec, const Matrix& mat)
    {
        return Vector3i(glm::ivec3(mat.ToGlmMat4() * glm::ivec4(vec, 1)));
    }

    void Vector3i::Transform(const Matrix& mat)
    {
        *this = Vector3i::Transform(*this, mat);
    }

    Vector3i Vector3i::Translate(const Vector3i& vec, const Vector3& dir, float dist)
    {
        return vec + Vector3i(dir * dist);
    }

    void Vector3i::Translate(const Vector3& dir, float dist)
    {
        *this = Vector3i::Translate(*this, Vector3::Normalize(dir), dist);
    }

    Vector3 Vector3i::ToVector3(int shift) const
    {
        if (shift != NO_VALUE)
        {
            return Vector3(FP_FLOAT(x, shift), FP_FLOAT(y, shift), FP_FLOAT(z, shift));
        }

        return Vector3(x, y, z);
    }

    const glm::ivec3& Vector3i::ToGlmVec3i() const
    {
        return *(const glm::ivec3*)this;
    }

    glm::ivec3& Vector3i::ToGlmVec3i()
    {
        return *(glm::ivec3*)this;
    }

    bool Vector3i::operator==(const Vector3i& vec) const
    {
        return ToGlmVec3i() == vec.ToGlmVec3i();
    }

    bool Vector3i::operator!=(const Vector3i& vec) const
    {
        return ToGlmVec3i() != vec.ToGlmVec3i();
    }

    Vector3i& Vector3i::operator+=(const Vector3i& vec)
    {
        ToGlmVec3i() += vec.ToGlmVec3i();
        return *this;
    }

    Vector3i& Vector3i::operator-=(const Vector3i& vec)
    {
        ToGlmVec3i() -= vec.ToGlmVec3i();
        return *this;
    }

    Vector3i& Vector3i::operator*=(const Vector3i& vec)
    {
        ToGlmVec3i() *= vec.ToGlmVec3i();
        return *this;
    }

    Vector3i& Vector3i::operator*=(float scalar)
    {
        auto vecFloat = glm::vec3(ToGlmVec3i()) * scalar;
        *this         = Vector3i(round(vecFloat));
        return *this;
    }

    Vector3i& Vector3i::operator/=(float scalar)
    {
        auto vecFloat = glm::vec3(ToGlmVec3i()) / scalar;
        *this         = Vector3i(round(vecFloat));
        return *this;
    }

    Vector3i Vector3i::operator+(const Vector3i& vec) const
    {
        return Vector3i(ToGlmVec3i() + vec.ToGlmVec3i());
    }

    Vector3i Vector3i::operator-(const Vector3i& vec) const
    {
        return Vector3i(ToGlmVec3i() - vec.ToGlmVec3i());
    }

    Vector3i Vector3i::operator*(const Vector3i& vec) const
    {
        return Vector3i(ToGlmVec3i() * vec.ToGlmVec3i());
    }

    Vector3i Vector3i::operator*(float scalar) const
    {
        auto vecFlt = glm::vec3(ToGlmVec3i()) * scalar;
        return Vector3i(round(vecFlt));
    }

    Vector3i Vector3i::operator/(float scalar) const
    {
        auto vecFlt = glm::vec3(ToGlmVec3i()) / scalar;
        return Vector3i(round(vecFlt));
    }

    Vector3i Vector3i::operator-() const
    {
        return Vector3i(-ToGlmVec3i());
    }
}
