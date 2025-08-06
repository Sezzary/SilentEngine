#pragma once

namespace Silent::Compatibility
{
    struct VECTOR3 : Vector3i
    {
        int& vx;
        int& vy;
        int& vz;

        VECTOR3(int x, int y, int z) : Vector3i(x, y, z), vx(this->x), vy(this->y), vz(this->z) {}
    };
    using VECTOR = VECTOR3;

    struct SVECTOR3 : EulerAngles
    {
        short& vx;
        short& vy;
        short& vz;

        SVECTOR3(short x, short y, short z) : EulerAngles(x, y, z), vx(this->x), vy(this->y), vz(this->z) {}
    };
    using SVECTOR = SVECTOR3;
}
