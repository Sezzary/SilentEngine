#pragma once

#include "Framework.h"

namespace Silent::Math
{
    constexpr float PI       = glm::pi<float>();
    constexpr float PI_MUL_2 = PI * 2.0f;
    constexpr float PI_MUL_4 = PI * 4.0f;
    constexpr float PI_DIV_2 = PI / 2.0f;
    constexpr float PI_DIV_4 = PI / 4.0f;
    constexpr float SQRT_2   = 1.41421356237f;
    constexpr float EPSILON  = 0.00001f;

    constexpr uint TRIANGLE_VERTEX_COUNT = 3;
    constexpr uint QUAD_VERTEX_COUNT     = 4;

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
        return ABS(a - b);
    };

    /** @brief Checks if two values have different signs. */
    constexpr auto DIFF_SIGN = [](auto a, auto b)
    {
        return (a >= 0 && b < 0) || (a < 0 && b >= 0);
    };

    /** @brief Rounds a floating-point value. */
    constexpr float ROUND(float x)
    {
        return (float)((x > 0.0f) ? (int)(x + 0.5f) : (int)(x - 0.5f));
    }

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
}
