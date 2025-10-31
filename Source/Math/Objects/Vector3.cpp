#include "Framework.h"
#include "Math/Objects/Vector3.h"

#include "Math/Constants.h"
#include "Math/Objects/Matrix.h"
#include "Math/Objects/Vector3.h"

namespace Silent::Math
{
    const Vector3 Vector3::Zero  = Vector3(0.0f, 0.0f, 0.0f);
    const Vector3 Vector3::One   = Vector3(1.0f, 1.0f, 1.0f);
    const Vector3 Vector3::UnitX = Vector3(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::UnitY = Vector3(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::UnitZ = Vector3(0.0f, 0.0f, 1.0f);

    float Vector3::Length() const
    {
        return glm::length(ToGlmVec3());
    }

    float Vector3::LengthSquared() const
    {
        return glm::length2(ToGlmVec3());
    }

    float Vector3::Distance(const Vector3& from, const Vector3& to)
    {
        return glm::distance(from.ToGlmVec3(), to.ToGlmVec3());
    }

    float Vector3::DistanceSquared(const Vector3& from, const Vector3& to)
    {
        return glm::distance2(from.ToGlmVec3(), to.ToGlmVec3());
    }

    float Vector3::Dot(const Vector3& vec0, const Vector3& vec1)
    {
        return glm::dot(vec0.ToGlmVec3(), vec1.ToGlmVec3());
    }

    Vector3 Vector3::Cross(const Vector3& vec0, const Vector3& vec1)
    {
        return Vector3(glm::cross(vec0.ToGlmVec3(), vec1.ToGlmVec3()));
    }

    Vector3 Vector3::Min(const Vector3& vec0, const Vector3& vec1)
    {
        return glm::min(vec0.ToGlmVec3(), vec1.ToGlmVec3());
    }

    void Vector3::Min(const Vector3& vec)
    {
        *this = Vector3::Min(*this, vec);
    }

    Vector3 Vector3::Max(const Vector3& vec0, const Vector3& vec1)
    {
        return glm::max(vec0.ToGlmVec3(), vec1.ToGlmVec3());
    }

    void Vector3::Max(const Vector3& vec)
    {
        *this = Vector3::Max(*this, vec);
    }

    Vector3 Vector3::Clamp(const Vector3& vec, const Vector3& min, const Vector3& max)
    {
        return Vector3(glm::clamp(vec.ToGlmVec3(), min.ToGlmVec3(), max.ToGlmVec3()));
    }

    void Vector3::Clamp(const Vector3& min, const Vector3& max)
    {
        *this = Vector3::Clamp(*this, min, max);
    }

    Vector3 Vector3::Normalize(const Vector3& vec)
    {
        return Vector3(glm::normalize(vec.ToGlmVec3()));
    }

    void Vector3::Normalize()
    {
        *this = Vector3::Normalize(*this);
    }

    Vector3 Vector3::Lerp(const Vector3& from, const Vector3& to, float alpha)
    {
        return Vector3(glm::lerp(from.ToGlmVec3(), to.ToGlmVec3(), alpha));
    }

    void Vector3::Lerp(const Vector3& to, float alpha)
    {
        *this = Vector3::Lerp(*this, to, alpha);
    }

    Vector3 Vector3::Smoothstep(const Vector3& from, const Vector3& to, float alpha)
    {
        return Vector3(glm::smoothstep(from.ToGlmVec3(), to.ToGlmVec3(), glm::vec3(alpha)));
    }

    void Vector3::Smoothstep(const Vector3& to, float alpha)
    {
        *this = Vector3::Smoothstep(*this, to, alpha);
    }

    Vector3 Vector3::Transform(const Vector3& vec, const Matrix& transformMat)
    {
        return Vector3(glm::vec3(transformMat.ToGlmMat4() * glm::vec4(vec, 1.0f)));
    }

    void Vector3::Transform(const Matrix& transformMat)
    {
        *this = Vector3::Transform(*this, transformMat);
    }

    Vector3 Vector3::Translate(const Vector3& vec, const Vector3& dir, float dist)
    {
        return vec + (dir * dist);
    }

    void Vector3::Translate(const Vector3& dir, float dist)
    {
        *this = Vector3::Translate(*this, Vector3::Normalize(dir), dist);
    }

    Vector3 Vector3::Rotate(const Vector3& vec, const Matrix& rotMat)
    {
        return Vector3(glm::vec3(rotMat.ToGlmMat4() * glm::vec4(vec, 0.0f)));
    }

    void Vector3::Rotate(const Matrix& rotMat)
    {
        *this = Vector3::Rotate(*this, rotMat);
    }

    bool Vector3::Compare(const Vector3& vec0, const Vector3& vec1, float epsilon)
    {
        return Vector3::DistanceSquared(vec0, vec1) <= SQUARE(epsilon);
    }

    Matrix Vector3::ToTranslationMatrix() const
    {
        return Matrix::CreateTranslation(*this);
    }

    Matrix Vector3::ToScaleMatrix() const
    {
        return Matrix::CreateScale(*this);
    }

    Vector3i Vector3::ToVector3i(int shift) const
    {
        if (shift != NO_VALUE)
        {
            return Vector3i(FP_TO(x, shift), FP_TO(y, shift), FP_TO(z, shift));
        }

        return Vector3i((int)round(x), (int)round(y), (int)round(z));
    }

    const glm::vec3& Vector3::ToGlmVec3() const
    {
        return *(const glm::vec3*)this;
    }

    glm::vec3& Vector3::ToGlmVec3()
    {
        return *(glm::vec3*)this;
    }

    bool Vector3::operator==(const Vector3& vec) const
    {
        return ToGlmVec3() == vec.ToGlmVec3();
    }

    bool Vector3::operator!=(const Vector3& vec) const
    {
        return ToGlmVec3() != vec.ToGlmVec3();
    }

    Vector3& Vector3::operator+=(const Vector3& vec)
    {
        ToGlmVec3() += vec.ToGlmVec3();
        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& vec)
    {
        ToGlmVec3() -= vec.ToGlmVec3();
        return *this;
    }

    Vector3& Vector3::operator*=(const Vector3& vec)
    {
        ToGlmVec3() *= vec.ToGlmVec3();
        return *this;
    }

    Vector3& Vector3::operator*=(float scalar)
    {
        ToGlmVec3() *= scalar;
        return *this;
    }

    Vector3& Vector3::operator/=(const Vector3& vec)
    {
        ToGlmVec3() /= vec.ToGlmVec3();
        return *this;
    }

    Vector3& Vector3::operator/=(float scalar)
    {
        ToGlmVec3() /= scalar;
        return *this;
    }

    Vector3 Vector3::operator+(const Vector3& vec) const
    {
        return Vector3(ToGlmVec3() + vec.ToGlmVec3());
    }

    Vector3 Vector3::operator-(const Vector3& vec) const
    {
        return Vector3(ToGlmVec3() - vec.ToGlmVec3());
    }

    Vector3 Vector3::operator*(const Vector3& vec) const
    {
        return Vector3(ToGlmVec3() * vec.ToGlmVec3());
    }

    Vector3 Vector3::operator*(float scalar) const
    {
        return Vector3(ToGlmVec3() * scalar);
    }

    Vector3 Vector3::operator/(const Vector3& vec) const
    {
        return Vector3(ToGlmVec3() / vec.ToGlmVec3());
    }

    Vector3 Vector3::operator/(float scalar) const
    {
        return Vector3(ToGlmVec3() / scalar);
    }

    Vector3 Vector3::operator-() const
    {
        return Vector3(-ToGlmVec3());
    }
}
