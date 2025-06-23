#include "Framework.h"
#include "Math/Rng.h"

namespace Silent::Math
{
    /** @brief Current random seed for the `GenerateInt32` function.
     * The value is updated with each call to the RNG functions.
     */
    static uint32 RngSeed = 0;

    uint32 GenerateInt32()
    {
        RngSeed = (RngSeed * 1664525) + 1013904223;
        return RngSeed;
    }

    uint16 GenerateInt16()
    {
        return GenerateInt32() >> 17;
    }

    uint16 GenerateInt12()
    {
        return GenerateInt32() >> 20;
    }

    uint GetSeed()
    {
        return RngSeed;
    }

    void SetSeed(uint32 newSeed)
    {
        RngSeed = newSeed;
    }

    bool TestRng(uint bits)
    {
        return GenerateInt16() & ((1 << bits) - 1);
    }
}
