#include "Framework.h"
#include "Math/Objects/Quaternion.h"

#include "Math/Constants.h"
#include "Math/Objects/Vector3.h"
#include "Math/Objects/Vector4.h"

namespace Silent::Math
{
    const Quaternion Quaternion::Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

    Quaternion::Quaternion(const Vector3& dir)
    {
        auto  dirNorm = Vector3::Normalize(dir);
        float dot     = Vector3::Dot(Vector3::UnitZ, dirNorm);
        float rad     = glm::acos(dot);

        if (rad < EPSILON)
        {
            *this = Quaternion::Identity;
            return;
        }

        auto axis = Vector3::Cross(Vector3::UnitZ, dirNorm);
        axis.Normalize();

        float halfAngle    = rad / 2.0f;
        float sinHalfAngle = glm::sin(halfAngle);

        w = glm::cos(halfAngle);
        x = axis.x * sinHalfAngle;
        y = axis.y * sinHalfAngle;
        z = axis.z * sinHalfAngle;
    }

    short Quaternion::AngularDistance(const Quaternion& from, const Quaternion& to)
    {
        float dot = glm::dot(from.ToGlmQuat(), to.ToGlmQuat());
        dot       = glm::clamp(dot, -1.0f, 1.0f);

        float rad = glm::acos(dot) * 2.0f;
        return FP_ANGLE_FROM_RAD(rad);
    }

    Quaternion Quaternion::Invert(const Quaternion& quat)
    {
        return Quaternion(glm::inverse(quat.ToGlmQuat()));
    }

    void Quaternion::Invert()
    {
        *this = Quaternion::Invert(*this);
    }

    Quaternion Quaternion::Lerp(const Quaternion& from, const Quaternion& to, float alpha)
    {
        return Quaternion(glm::lerp(from.ToGlmQuat(), to.ToGlmQuat(), alpha));
    }

    void Quaternion::Lerp(const Quaternion& to, float alpha)
    {
        *this = Quaternion::Lerp(*this, to, alpha);
    }

    Quaternion Quaternion::Slerp(const Quaternion& from, const Quaternion& to, float alpha)
    {
        return Quaternion(glm::slerp(from.ToGlmQuat(), to.ToGlmQuat(), alpha));
    }

    void Quaternion::Slerp(const Quaternion& to, float alpha)
    {
        *this = Quaternion::Slerp(*this, to, alpha);
    }

    Vector3 Quaternion::ToDirection() const
    {
        return Vector3::Rotate(Vector3::UnitZ, ToRotationMatrix());
    }

    EulerAngles Quaternion::ToEulerAngles() const
    {
        auto eulerAnglesRad = glm::eulerAngles(ToGlmQuat());
        return EulerAngles(FP_ANGLE_FROM_RAD(eulerAnglesRad.x), FP_ANGLE_FROM_RAD(eulerAnglesRad.y), FP_ANGLE_FROM_RAD(eulerAnglesRad.z));
    }

    AxisAngle Quaternion::ToAxisAngle() const
    {
        float sinHalfAngle = glm::sqrt(1.0f - SQUARE(w));

        // Compute axis.
        auto axis = Vector3::Zero;
        if (sinHalfAngle < EPSILON)
        {
            axis = Vector3::UnitZ;
        }
        else
        {
            axis = Vector3(x, y, z) / sinHalfAngle;
        }

        // Compute angle.
        float rad = glm::acos(w) * 2.0f;

        // Create axis-angle.
        return AxisAngle(axis, rad);
    }

    Matrix Quaternion::ToRotationMatrix() const
    {
        return Matrix(glm::mat4_cast(*this));
    }

    const glm::quat& Quaternion::ToGlmQuat() const
    {
        return *(const glm::quat*)this;
    }

    glm::quat& Quaternion::ToGlmQuat()
    {
        return *(glm::quat*)this;
    }

    bool Quaternion::operator==(const Quaternion& quat) const
    {
        return ToGlmQuat() == quat.ToGlmQuat();
    }

    bool Quaternion::operator!=(const Quaternion& quat) const
    {
        return ToGlmQuat() != quat.ToGlmQuat();
    }

    Quaternion& Quaternion::operator+=(const Quaternion& quat)
    {
        ToGlmQuat() += quat.ToGlmQuat();
        return *this;
    }

    Quaternion& Quaternion::operator-=(const Quaternion& quat)
    {
        ToGlmQuat() -= quat.ToGlmQuat();
        return *this;
    }

    Quaternion& Quaternion::operator*=(const Quaternion& quat)
    {
        ToGlmQuat() *= quat.ToGlmQuat();
        return *this;
    }

    Quaternion& Quaternion::operator*=(float scalar)
    {
        ToGlmQuat() /= scalar;
        return *this;
    }

    Quaternion& Quaternion::operator/=(const Quaternion& quat)
    {
        ToGlmQuat() *= glm::inverse(quat.ToGlmQuat());
        return *this;
    }

    Quaternion Quaternion::operator+(const Quaternion& quat) const
    {
        return Quaternion(ToGlmQuat() + quat.ToGlmQuat());
    }

    Quaternion Quaternion::operator-(const Quaternion& quat) const
    {
        return Quaternion(ToGlmQuat() - quat.ToGlmQuat());
    }

    Quaternion Quaternion::operator*(const Quaternion& quat) const
    {
        return Quaternion(ToGlmQuat() * quat.ToGlmQuat());
    }

    Quaternion Quaternion::operator*(float scalar) const
    {
        return Quaternion(ToGlmQuat() * scalar);
    }

    Quaternion Quaternion::operator/(const Quaternion& quat) const
    {
        return ToGlmQuat() * glm::inverse(quat.ToGlmQuat());
    }
}
