#pragma once

#include "Framework.h"

namespace Silent::Math
{
    constexpr uint Q4_SHIFT       = 4;
    constexpr uint Q8_SHIFT       = 8;
    constexpr uint Q12_SHIFT      = 12;
    constexpr uint FP_ANGLE_COUNT = 1 << Q12_SHIFT;
    constexpr int  FP_PI          = 0x5000 / 2;

    constexpr float PI       = glm::pi<float>();
    constexpr float PI_MUL_2 = PI * 2.0f;
    constexpr float PI_MUL_4 = PI * 4.0f;
    constexpr float PI_DIV_2 = PI / 2.0f;
    constexpr float PI_DIV_4 = PI / 4.0f;
    constexpr float SQRT_2   = 1.41421356237f;
    constexpr float EPSILON  = 0.00001f;

    enum class ContainmentType
    {
        None,
        Intersects,
        Contains
    };

    /** @brief Squares a numeric value. */
    constexpr auto SQUARE = [](auto x)
    {
        return x * x;
    };

    /** @brief Cubes a numeric value. */
    constexpr auto CUBE = [](auto x)
    {
        return x * x * x;
    };

    /** @brief Determines the smaller of two values. */
    constexpr auto MIN = [](auto a, auto b)
    {
        return (a < b) ? a : b;
    };

    /** @brief Determines the larger of two values. */
    constexpr auto MAX = [](auto a, auto b)
    {
        return (a > b) ? a : b;
    };

    /** @brief Clamps a value to the range `[min, max]`. */
    constexpr auto CLAMP = [](auto x, auto min, auto max)
    {
        return (x < min) ? min : ((x > max) ? max : x);
    };

    /** @brief Computes the absolute value. */
    constexpr auto ABS = [](auto x)
    {
        return (x < 0) ? -x : x;
    };

    /** @brief Computes the absolute sum of two values. */
    constexpr auto ABS_ADD = [](auto a, auto b)
    {
        return (a >= 0) ? (b + a) : (b - a);
    };

    /** @brief Computes the absolute difference between two values. */
    constexpr auto ABS_DIFF = [](auto a, auto b)
    {
        return ((a - b) < 0) ? (b - a) : (a - b);
    };

    /** @brief Rounds a floating-point value. */
    constexpr float ROUND(float x)
    {
        return (float)((x > 0.0f) ? (int)(x + 0.5f) : (int)(x - 0.5f));
    }

    /** @brief Converts an integer from a scaled fixed-point Q format rounded to the nearest value. */
    /*constexpr int FP_ROUND_SCALED(int x, int scale, uint shift)
    {
        return (x + ((FP_TO(1, shift) * scale) - 1)) / (FP_TO(1, shift) * scale);
    }*/

    /** @brief Converts an integer from a fixed-point Q format rounded toward 0. */
    /*#define FP_ROUND_TO_ZERO(x, shift) \
        ((s32)(FP_FROM(x, (shift)) + ((u32)x >> 31)) >> 1)*/

    /** @brief Floors a floating-point value. */
    constexpr float FLOOR(float x)
    {
        return (float)((int)x - ((x < 0.0f) && x != (int)x));
    }

    /** @brief Ceils a floating-point value. */
    constexpr float CEIL(float x)
    {
        return (float)((int)x + ((x > 0.0f) && x != (int)x));
    }
    
    /** @brief Converts an integer to a fixed-point Q format. */
    constexpr int FP_TO(int x, uint shift)
    {
        return x << shift;
    }

    /** @brief Converts a float to a fixed-point Q format. */
    constexpr int FP_FLOAT_TO(float x, uint shift)
    {
        return (int)ROUND(x * FP_TO(1, shift));
    }

    /** @brief Converts an integer from a fixed-point Q format. */
    constexpr int FP_FROM(int x, uint shift)
    {
        return x >> shift;
    }

    /** @brief Multiplies two integers in a fixed-point Q format and converts the result from the fixed-point Q format. */
    constexpr int FP_MULTIPLY(int a, int b, uint shift)
    {
        return (a * b) >> shift;
    }

    /** @brief Multiplies an integer by a float converted to fixed-point Q format and converts the result from the fixed-point Q format. */
    constexpr int FP_MULTIPLY_FLOAT(int a, float b, uint shift)
    {
        return FP_MULTIPLY(a, FP_FLOAT_TO(b, shift), shift);
    }

    /** @brief Multiplies an integer by a float converted to fixed-point Q format, using a 64-bit intermediate for higher precision. */
    constexpr int FP_MULTIPLY_PRECISE(int a, float b, uint shift)
    {
        return FP_MULTIPLY((int64)a, FP_FLOAT_TO(b, shift), shift);
    }

    /** @brief Converts a floating-point alpha in the range `[0.0f, 1.0f]` to a fixed-point alpha in Q3.12 format. */
    constexpr short FP_ALPHA(float alpha)
    {
        return (short)FP_FLOAT_TO(alpha, Q12_SHIFT);
    }

    /** @brief Converts a normalized color value in the range `[0.0f, 1.0f]` to an 8-bit color format in the range `[0, 255]`. */
    constexpr uchar FP_COLOR(float val)
    {
        return (uchar)(val * (FP_TO(1, Q8_SHIFT) - 1));
    }

    /** @brief Converts floating-point degrees to fixed-point degrees in Q1.15 format. */
    constexpr short FP_ANGLE(float deg)
    {
        return (short)ROUND(deg * (360.0f / (float)FP_ANGLE_COUNT));
    }

    /** @brief Converts floating-point radians to fixed-point degrees in Q1.15 format. */
    constexpr short FP_ANGLE_FROM_RAD(float rad)
    {
        return FP_ANGLE(rad / (PI / 180.0f));
    }

    /** @brief Converts fixed-point degrees in Q1.15 format to floating-point radians. */
    constexpr float FP_ANGLE_TO_RAD(short deg)
    {
        return (deg * (360.0f / (float)FP_ANGLE_COUNT)) * (PI / 180.0f);
    }

    /** @brief Converts floating-point radians in the range `[-PI, PI]` to fixed-point radians in the range `[0, 0x5000]`. */
    constexpr int FP_RADIAN(float rad)
    {
        return (((rad < 0.0f) ? (PI + (PI - ABS(rad))) : rad) * ((float)FP_PI / PI)) *
               ((rad < 0.0f || rad >= PI) ? 1.0f : 2.0f);
    }

    /** @brief Converts floating-point meters to fixed-point world units in Q12.8 format. */
    constexpr int FP_METER(float meters)
    {
        return FP_FLOAT_TO(meters, Q8_SHIFT);
    }
}
