#include "Framework.h"
#include "Psx.h"
#include "Math/Compatibility.h"

#include "Math/Constants.h"
#include "Math/FixedPoint/Constants.h"
#include "Math/Objects/EulerAngles.h"
#include "Math/Objects/Matrix.h"
#include "Math/Objects/Vector3i.h"

namespace Silent::Math
{
    constexpr float Q4_SCALE  = 1.0f / (float)FP_TO(1, Q4_SHIFT);
    constexpr float Q6_SCALE  = 1.0f / (float)FP_TO(1, Q6_SHIFT);
    constexpr float Q8_SCALE  = 1.0f / (float)FP_TO(1, Q8_SHIFT);
    constexpr float Q12_SCALE = 1.0f / (float)FP_TO(1, Q12_SHIFT);

    /*MATRIX ToPsxMatrix(const Matrix& mat)
    {
        auto newMat = MATRIX{};

        // Set rotation.
        newMat.m[0][0] = (short)(mat[0][0] * Q12_SCALE);
        newMat.m[1][0] = (short)(mat[0][1] * Q12_SCALE);
        newMat.m[2][0] = (short)(mat[0][2] * Q12_SCALE);
        newMat.m[0][1] = (short)(mat[1][0] * Q12_SCALE);
        newMat.m[1][1] = (short)(mat[1][1] * Q12_SCALE);
        newMat.m[2][1] = (short)(mat[1][2] * Q12_SCALE);
        newMat.m[0][2] = (short)(mat[2][0] * Q12_SCALE);
        newMat.m[1][2] = (short)(mat[2][1] * Q12_SCALE);
        newMat.m[2][2] = (short)(mat[2][2] * Q12_SCALE);

        // Set translation.
        newMat.t[0] = (int)(mat[3][0] * Q12_SCALE);
        newMat.t[1] = (int)(mat[3][1] * Q12_SCALE);
        newMat.t[2] = (int)(mat[3][2] * Q12_SCALE);

        return newMat;
    }*/

    /*Matrix ToMatrix(const MATRIX& mat)
    {
        auto newMat = Matrix::Identity;

        // Convert rotation.
        newMat[0][0] = mat.m[0][0] * Q12_SCALE;
        newMat[0][1] = mat.m[1][0] * Q12_SCALE;
        newMat[0][2] = mat.m[2][0] * Q12_SCALE;
        newMat[1][0] = mat.m[0][1] * Q12_SCALE;
        newMat[1][1] = mat.m[1][1] * Q12_SCALE;
        newMat[1][2] = mat.m[2][1] * Q12_SCALE;
        newMat[2][0] = mat.m[0][2] * Q12_SCALE;
        newMat[2][1] = mat.m[1][2] * Q12_SCALE;
        newMat[2][2] = mat.m[2][2] * Q12_SCALE;

        // Convert translation.
        newMat[3][0] = mat.t[0] * Q12_SCALE;
        newMat[3][1] = mat.t[1] * Q12_SCALE;
        newMat[3][2] = mat.t[2] * Q12_SCALE;

        return newMat;
    }*/

    Vector3 VECTOR3::ToVector3(float qScale) const
    {
        return Vector3((float)vx, (float)vy, (float)vz) * qScale;
    }
}
