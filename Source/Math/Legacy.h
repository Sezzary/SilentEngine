#pragma once

#include "Math/Objects/EulerAngles.h"
#include "Math/Objects/Matrix.h"
#include "Math/Objects/Vector2i.h"
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
        Vector2i vertex0;
        Vector2i vertex1;
    };

    struct s_Triangle2d
    {
        Vector2i vertex0;
        Vector2i vertex1;
        Vector2i vertex2;
    };

    struct s_Quad2d
    {
        Vector2i vertex0;
        Vector2i vertex1;
        Vector2i vertex2;
        Vector2i vertex3;
    };

    struct s_ColoredLine2d
    {
        s_Line2d line;
        ushort   r;
        ushort   g;
        ushort   b;
        ushort   __pad_E; // Maybe 4th component of a RGB+code struct?
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

    /** @brief Computes the sine in Q19.12 of degrees in Q19.12, integer range `[0, 4096]`.
     *
     * TODO: Matched on decomp.me.
     * Possible original name: `shRsin`.
     *
     * @param angle Fixed-point degrees in Q19.12, integer range `[0, 4096]`.
     * @return Sine in Q19.12, integer range `[0, 4096]`.
     */
    q19_12 Math_Sin(q19_12 angle);
    
    /** @brief Computes the cosine in Q19.12 of degrees in Q19.12, integer range `[0, 4096]`.
     *
     * TODO: Matched on decomp.me.
     * Possible original name: `shRcos`.
     *
     * @param angle Fixed-point degrees in Q19.12, integer range `[0, 4096]`.
     * @return Cosine in Q19.12, integer range `[0, 4096]`.
     */
    q19_12 Math_Cos(q19_12 angle);
//}
