#pragma once

namespace Silent::Utils::Compatibility
{
    struct MATRIX
    {
        short m[3][3]; // 3x3 rotation matrix.
        int   t[3];    // Transfer vector.

        MATRIX(const Matrix& mat);

        Matrix ToMatrix() const;
    };

    struct SVECTOR3 : EulerAngles
    {
        short& vx;
        short& vy;
        short& vz;

        SVECTOR3(short x, short y, short z) : EulerAngles(x, y, z), vx(this->x), vy(this->y), vz(this->z) {}
    };
    using SVECTOR = SVECTOR3;

    struct VECTOR3 : Vector3i
    {
        int& vx;
        int& vy;
        int& vz;

        VECTOR3(int x, int y, int z) : Vector3i(x, y, z), vx(this->x), vy(this->y), vz(this->z) {}
    };
    using VECTOR = VECTOR3;

    struct GsCOORD2PARAM
    {
        VECTOR3 scale;
        SVECTOR rotate;
        VECTOR3 trans;
    };

    struct GsCOORDINATE2
    {
        unsigned long         flg;
        MATRIX                coord;
        MATRIX                workm;
        GsCOORD2PARAM*        param;
        struct GsCOORDINATE2* super;
        struct GsCOORDINATE2* sub;
    };

    /** @brief Computes the dot product of the first column of a matrix with a vector in Q17.15 format. */
    /*int FP_MULTIPLY_MATRIX(const MATRIX& mat, const VECTOR3& vec)
    {
        return ((mat.m[0][0] * vec.vx) +
                (mat.m[1][0] * vec.vy) +
                (mat.m[2][0] * vec.vz)) >> 17;
    }*/
}
