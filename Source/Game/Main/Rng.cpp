#include "Framework.h"
#include "Game/Main/Rng.h"

namespace Silent::Game
{
    uint g_RngSeed = 0;

    bool Rng_TestProbability(q19_12 chance)
    {
        return Rng_RandQ12() < chance;
    }

    int Rng_GenerateIntFromInput(int rand, int low, int high)
    {
        return (rand % ((high - low) + 1)) + low;
    }

    int Rng_GenerateUIntFromInput(int rand, int low, int high)
    {
        return ((uint)rand % ((high - low) + 1)) + low;
    }

    int Rng_GenerateInt(int low, int high)
    {
        return Rng_GenerateIntFromInput(Rng_Rand16(), low, high);
    }

    int Rng_GenerateUInt(int low, int high)
    {
        Rng_GenerateUIntFromInput(Rng_Rand16(), low, high);
    }

    int Rng_AddGeneratedUInt(int base, int low, int high)
    {
        return base + Rng_GenerateUInt(low, high);
    }

    q19_12 Rng_RandQ12()
    {
        int rand16 = Rng_Rand16();
        return Q12_FRACT(((rand16 * 2) ^ rand16) >> 3);
    }

    uint Rng_Rand32()
    {
        uint nextSeed = (g_RngSeed * 1664525) + 1013904223;
        g_RngSeed     = nextSeed;

        return nextSeed;
    }

    int Rng_Rand16()
    {
        return Rng_Rand32() >> 17;
    }

    uint Rng_GetSeed()
    {
        return g_RngSeed;
    }

    void Rng_SetSeed(uint nextSeed)
    {
        g_RngSeed = nextSeed;
    }

    ushort Rng_Rand12()
    {
        return Rng_Rand32() >> 20;
    }
}
