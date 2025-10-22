#pragma once

#include "Math/Objects/EulerAngles.h"
#include "Math/Objects/Matrix.h"
#include "Math/Objects/Vector3i.h"

using namespace Silent;
using namespace Silent::Math;

//namespace Silent::Math
//{
    constexpr uint LINE_VERTEX_COUNT = 2;
    constexpr uint RECT_VERTEX_COUNT = 4;
    constexpr uint BOX_VERTEX_COUNT  = 8;

    /** @brief Fixed-point Q types. */
    enum class QType
    {
        Q4,
        Q6,
        Q8,
        Q12
    };

    /** @brief PsyQ matrix. */
    struct MATRIX
    {
        short m[3][3]; /** 3x3 rotation matrix. */
        int   t[3];    /** Transfer vector. */

        MATRIX(const Matrix& mat);

        Matrix ToMatrix() const;
    };

    /** @brief PsyQ `short`-based XY vector. */
    struct DVECTOR
    {
        short vx;
        short vy;
    };

    /** @brief `DVECTOR` variant with a `vz` component instead of `vy`. */
    struct DVECTOR_XZ
    {
        short vx;
        short vz;
    };

    /** @brief PsyQ `short`-based XYZ vector. */
    struct SVECTOR3
    {
        short vx;
        short vy;
        short vz;
    };
    using SVECTOR = SVECTOR3;

    /** @brief PsyQ `int`-based XYZ vector. */
    struct VECTOR3 : Vector3i
    {
        int& vx;
        int& vy;
        int& vz;

        VECTOR3(int x, int y, int z) : Vector3i(x, y, z), vx(this->x), vy(this->y), vz(this->z) {}

        Vector3 ToVector3(QType qType) const;
    };
    using VECTOR = VECTOR3;

    /** @brief PsyQ coordinate. */
    struct GsCOORD2PARAM
    {
        VECTOR3 scale;
        SVECTOR rotate;
        VECTOR3 trans;
    };

    /** @brief PsyQ bone coordinate. */
    struct GsCOORDINATE2
    {
        ulong                 flg;   /** `bool`. */
        MATRIX                coord;
        MATRIX                workm;
        GsCOORD2PARAM*        param;
        struct GsCOORDINATE2* super; /** Parent. */
        struct GsCOORDINATE2* sub;   /** Child. */
    };
    
    struct s_Line2d
    {
        DVECTOR vertex0_0;
        DVECTOR vertex1_4;
    };

    struct s_Triangle2d
    {
        DVECTOR vertex0_0;
        DVECTOR vertex1_4;
        DVECTOR vertex2_8;
    };

    struct s_Quad2d
    {
        DVECTOR vertex0_0;
        DVECTOR vertex1_4;
        DVECTOR vertex2_8;
        DVECTOR vertex3_C;
    };

    struct s_ColoredLine2d
    {
        s_Line2d line_0;
        ushort    r_8;
        ushort    g_A;
        ushort    b_C;
        ushort    __pad_E; // Maybe 4th component of a RGB+code struct?
    };

    struct s_LineBorder
    {
        s_Line2d lines_0[RECT_VERTEX_COUNT];
    };

    struct s_QuadBorder
    {
        s_Quad2d quads_0[RECT_VERTEX_COUNT];
    };

    struct s_PrimColor
    {
        uchar r;
        uchar g;
        uchar b;
        uchar p;
    };
//}
