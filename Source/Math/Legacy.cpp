#include "Framework.h"
#include "Math/Legacy.h"

#include "Math/Constants.h"
#include "Math/FixedPoint.h"
#include "Math/Objects/EulerAngles.h"
#include "Math/Objects/Matrix.h"
#include "Math/Objects/Vector3i.h"

using namespace Silent;
using namespace Silent::Math;

//namespace Silent::Math
//{
    constexpr float Q4_SCALE  = 1.0f / (float)FP_TO(1, Q4_SHIFT);
    constexpr float Q6_SCALE  = 1.0f / (float)FP_TO(1, Q6_SHIFT);
    constexpr float Q8_SCALE  = 1.0f / (float)FP_TO(1, Q8_SHIFT);
    constexpr float Q12_SCALE = 1.0f / (float)FP_TO(1, Q12_SHIFT);

    MATRIX::MATRIX(const Matrix& mat)
    {
        // Set rotation.
        m[0][0] = (short)(mat[0][0] * Q12_SCALE);
        m[1][0] = (short)(mat[0][1] * Q12_SCALE);
        m[2][0] = (short)(mat[0][2] * Q12_SCALE);
        m[0][1] = (short)(mat[1][0] * Q12_SCALE);
        m[1][1] = (short)(mat[1][1] * Q12_SCALE);
        m[2][1] = (short)(mat[1][2] * Q12_SCALE);
        m[0][2] = (short)(mat[2][0] * Q12_SCALE);
        m[1][2] = (short)(mat[2][1] * Q12_SCALE);
        m[2][2] = (short)(mat[2][2] * Q12_SCALE);

        // Set translation.
        t[0] = (int)(mat[3][0] * Q12_SCALE);
        t[1] = (int)(mat[3][1] * Q12_SCALE);
        t[2] = (int)(mat[3][2] * Q12_SCALE);
    }

    Matrix MATRIX::ToMatrix() const
    {
        auto mat = Matrix::Identity;

        // Convert rotation.
        mat[0][0] = m[0][0] * Q12_SCALE;
        mat[0][1] = m[1][0] * Q12_SCALE;
        mat[0][2] = m[2][0] * Q12_SCALE;
        mat[1][0] = m[0][1] * Q12_SCALE;
        mat[1][1] = m[1][1] * Q12_SCALE;
        mat[1][2] = m[2][1] * Q12_SCALE;
        mat[2][0] = m[0][2] * Q12_SCALE;
        mat[2][1] = m[1][2] * Q12_SCALE;
        mat[2][2] = m[2][2] * Q12_SCALE;

        // Convert translation.
        mat[3][0] = t[0] * Q12_SCALE;
        mat[3][1] = t[1] * Q12_SCALE;
        mat[3][2] = t[2] * Q12_SCALE;

        return mat;
    }

    Vector3 VECTOR3::ToVector3(QType qType) const
    {
        float scale = 0.0f;
        switch (qType)
        {
            case QType::Q4:
            {
                scale = Q4_SCALE;
                break;
            }
            case QType::Q6:
            {
                scale = Q6_SCALE;
                break;
            }
            case QType::Q8:
            {
                scale = Q8_SCALE;
                break;
            }
            case QType::Q12:
            {
                scale = Q12_SCALE;
                break;
            }
        }

        return Vector3((float)x, (float)y, (float)z) * scale;
    }

    q19_12 Math_Sin(q19_12 angle)
    {
        return Q12(glm::sin(FP_ANGLE_TO_RAD(angle)));
    }

    q19_12 Math_Cos(q19_12 angle)
    {
        return Q12(glm::cos(FP_ANGLE_TO_RAD(angle)));
    }
//}
