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
}
