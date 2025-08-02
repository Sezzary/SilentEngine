#include "Framework.h"
#include "Math/Objects/EulerAngles.h"

#include "Math/Constants.h"
#include "Math/Objects/AxisAngle.h"
#include "Math/Objects/Matrix.h"
#include "Math/Objects/Quaternion.h"
#include "Math/Objects/Vector3.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Math
{
    const EulerAngles EulerAngles::Identity = EulerAngles(0, 0, 0);

    EulerAngles::EulerAngles(const Vector3& dir)
    {
        auto dirNorm = Vector3::Normalize(dir);

        x = FP_ANGLE_FROM_RAD(-glm::asin(dirNorm.y));
        y = FP_ANGLE_FROM_RAD(glm::atan2(dirNorm.x, dirNorm.z));
        z = FP_ANGLE_FROM_RAD(0.0f);
    }

    EulerAngles EulerAngles::InterpConstant(const EulerAngles& from, const EulerAngles& to, short angularVel)
    {
        return EulerAngles(InterpConstant(from.x, to.x, angularVel),
                           InterpConstant(from.y, to.y, angularVel),
                           InterpConstant(from.z, to.z, angularVel));
    }

    void EulerAngles::InterpConstant(const EulerAngles& to, short angularVel)
    {
        *this = InterpConstant(*this, to, angularVel);
    }

    EulerAngles EulerAngles::Lerp(const EulerAngles& from, const EulerAngles& to, float alpha, short epsilon)
    {
        return EulerAngles(Lerp(from.x, to.x, alpha, epsilon),
                           Lerp(from.y, to.y, alpha, epsilon),
                           Lerp(from.z, to.z, alpha, epsilon));
    }

    void EulerAngles::Lerp(const EulerAngles& to, float alpha, short epsilon)
    {
        *this = Lerp(*this, to, alpha, epsilon);
    }

    EulerAngles EulerAngles::Slerp(const EulerAngles& from, const EulerAngles& to, float alpha)
    {
        auto quatFrom = from.ToQuaternion();
        auto quatTo   = to.ToQuaternion();

        auto quat = Quaternion::Slerp(quatFrom, quatTo, alpha);
        return quat.ToEulerAngles();
    }

    void EulerAngles::Slerp(const EulerAngles& to, float alpha)
    {
        *this = Slerp(*this, to, alpha);
    }

    bool EulerAngles::Compare(const EulerAngles& eulerAngles0, const EulerAngles& eulerAngles1, short epsilon)
    {
        return Compare(eulerAngles0.x, eulerAngles1.x, epsilon) &&
               Compare(eulerAngles0.y, eulerAngles1.y, epsilon) &&
               Compare(eulerAngles0.z, eulerAngles1.z, epsilon);
    }

    Vector3 EulerAngles::ToDirection() const
    {
        return Vector3::Rotate(Vector3::UnitZ, ToRotationMatrix());
    }

    Quaternion EulerAngles::ToQuaternion() const
    {
        return Quaternion(glm::quat(glm::vec3(FP_ANGLE_TO_RAD(x), FP_ANGLE_TO_RAD(y), FP_ANGLE_TO_RAD(z))));
    }

    AxisAngle EulerAngles::ToAxisAngle() const
    {
        auto quat = ToQuaternion();
        return quat.ToAxisAngle();
    }

    Matrix EulerAngles::ToRotationMatrix() const
    {
        return Matrix(glm::yawPitchRoll(FP_ANGLE_TO_RAD(y), FP_ANGLE_TO_RAD(x), FP_ANGLE_TO_RAD(z)));
    }

    glm::vec3 EulerAngles::ToGlmEulerAngles() const
    {
        return glm::vec3(FP_ANGLE_TO_RAD(y), FP_ANGLE_TO_RAD(x), FP_ANGLE_TO_RAD(z));
    }

    bool EulerAngles::operator==(const EulerAngles& eulerAngles) const
    {
        return x == eulerAngles.x && y == eulerAngles.y && z == eulerAngles.z;
    }

    bool EulerAngles::operator!=(const EulerAngles& eulerAngles) const
    {
        return !(*this == eulerAngles);
    }

    EulerAngles& EulerAngles::operator=(const EulerAngles& eulerAngles)
    {
        x = eulerAngles.x;
        y = eulerAngles.y;
        z = eulerAngles.z;
        return *this;
    }

    EulerAngles& EulerAngles::operator+=(const EulerAngles& eulerAngles)
    {
        x += eulerAngles.x;
        y += eulerAngles.y;
        z += eulerAngles.z;
        return *this;
    }

    EulerAngles& EulerAngles::operator-=(const EulerAngles& eulerAngles)
    {
        x -= eulerAngles.x;
        y -= eulerAngles.y;
        z -= eulerAngles.z;
        return *this;
    }

    EulerAngles& EulerAngles::operator*=(const EulerAngles& eulerAngles)
    {
        x *= eulerAngles.x;
        y *= eulerAngles.y;
        z *= eulerAngles.z;
        return *this;
    }

    EulerAngles& EulerAngles::operator*=(float scalar)
    {
        x = (short)(x * scalar);
        y = (short)(y * scalar);
        z = (short)(z * scalar);
        return *this;
    }

    EulerAngles& EulerAngles::operator/=(float scalar)
    {
        x = (short)(x / scalar);
        y = (short)(y / scalar);
        z = (short)(z / scalar);
        return *this;
    }

    EulerAngles EulerAngles::operator+(const EulerAngles& eulerAngles) const
    {
        return EulerAngles(x + eulerAngles.x, y + eulerAngles.y, z + eulerAngles.z);
    }

    EulerAngles EulerAngles::operator-(const EulerAngles& eulerAngles) const
    {
        return EulerAngles(x - eulerAngles.x, y - eulerAngles.y, z - eulerAngles.z);
    }

    EulerAngles EulerAngles::operator*(const EulerAngles& eulerAngles) const
    {
        return EulerAngles(x * eulerAngles.x, y * eulerAngles.y, z * eulerAngles.z);
    }

    EulerAngles EulerAngles::operator*(float scalar) const
    {
        return EulerAngles((short)round((float)x * scalar), (short)round((float)y * scalar), (short)round((float)z * scalar));
    }

    EulerAngles EulerAngles::operator/(float scalar) const
    {
        return EulerAngles((short)round((float)x / scalar), (short)round((float)y / scalar), (short)round((float)z / scalar));
    }

    short EulerAngles::InterpConstant(short from, short to, short angularVel)
    {
        if (Compare(from, to, angularVel))
        {
            return to;
        }

        int sign = GetSign(to - from);
        return (short)(from + (angularVel * sign));
    }

    short EulerAngles::Lerp(short from, short to, float alpha, short epsilon)
    {
        if (Compare(from, to, epsilon))
        {
            return to;
        }

        short delta = to - from;
        return (short)round(from + (delta * std::clamp(alpha, 0.0f, 1.0f)));
    }

    bool EulerAngles::Compare(short angle0, short angle1, short epsilon)
    {
        short delta = angle1 - angle0;
        return abs(delta) <= epsilon;
    }
}
