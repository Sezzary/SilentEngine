#pragma once

#include "Framework.h"

namespace Silent::Math
{
    constexpr float PI           = glm::pi<float>();
    constexpr float PI_MUL_2     = PI * 2.0f;
    constexpr float PI_MUL_4     = PI * 4.0f;
    constexpr float PI_DIV_2     = PI / 2.0f;
    constexpr float PI_DIV_4     = PI / 4.0f;
    constexpr float SQRT_2       = 1.41421356237f;
    constexpr float EPSILON      = 0.00001f;
    constexpr uint  GOLDEN_RATIO = 0x9E3779B9;

    constexpr int LINE_VERTEX_COUNT = 2;
    constexpr int LINE_IDX_COUNT    = 2;
    constexpr int TRI_VERTEX_COUNT  = 3;
    constexpr int TRI_IDX_COUNT     = 3;
    constexpr int QUAD_VERTEX_COUNT = 4;
    constexpr int QUAD_IDX_COUNT    = 6;
    constexpr int RGB_COMP_COUNT    = 3;
    constexpr int RGBA_COMP_COUNT   = 4;

    constexpr std::array<int, TRI_VERTEX_COUNT * 2> QUAD_TRI_IDXS = { 0, 1, 2, 0, 2, 3 };

    /** @brief Geometry containment types. */
    enum class ContainmentType
    {
        None,
        Intersects,
        Contains
    };

    /** @brief Squares a value.
     *
     * @param x Value to be squared.
     * @return `x` squared.
     */
    constexpr auto SQUARE = [](auto x)
    {
        return x * x;
    };

    /** @brief Cubes a value.
     *
     * @param x Value to be cubed.
     * @return `x` cubed.
     */
    constexpr auto CUBE = [](auto x)
    {
        return x * x * x;
    };

    /** @brief Determines the smaller of two values.
     *
     * @param a First value.
     * @param b Second value.
     * @return `true` if `a` is smaller than `b`, `false` otherwise.
     */
    constexpr auto MIN = [](auto a, auto b)
    {
        return (a < b) ? a : b;
    };

    /** @brief Computes the minimum of two values and subtracts a base offset.
     *
     * @param a First value.
     * @param b Second value.
     * @param base Base value to subtract.
     * @return Minimum of `a` and `b`, minus `base`.
     */
    #define MIN_EXTENT_OFFSET(a, b, base) \
        (((a) < (b)) ? ((a) - (base)) : ((b) - (base)))

    /** @brief Determines the larger of two values.
     *
     * @param a First value.
     * @param b Second value.
     * @return `true` if `a` is larger than `b`, `false` otherwise.
     */
    constexpr auto MAX = [](auto a, auto b)
    {
        return (a > b) ? a : b;
    };

    /** @brief Clamps a value to the range `[min, max]`.
     *
     * @param x Value to clamp.
     * @param min Lower range.
     * @param max Upper range.
     * @return `x` clamped to the range `[min, max]`.
     */
    constexpr auto CLAMP = [](auto x, auto min, auto max)
    {
        return (x < min) ? min : ((x > max) ? max : x);
    };

    /** @brief Clamps a value to be not less than a given lower bound.
     *
     * @param x Value to clamp.
     * @param min Lower bound.
     * @return `x` if `x >= min`, otherwise `min`.
     */
    constexpr auto CLAMP_LOW = [](auto x, auto min)
    {
        return (x < min) ? min : x;
    };

    /** @brief Clamps a value to be not greater than a given upper bound.
     *
     * @param x Value to clamp.
     * @param max Upper bound.
     * @return `x` if `x <= max`, otherwise `max`.
     */
    constexpr auto CLAMP_HIGH = [](auto x, auto max)
    {
        return (x > max) ? max : x;
    };

    /** @brief Clamps a value to the inclusive range `[min, max]`.
     *
     * Uses `CLAMP_HIGH`, then `CLAMP_LOW` to ensure the value
     * is between the specified bounds.
     *
     * @param x Value to clamp.
     * @param min Lower bound.
     * @param max Upper bound.
     * @return `x` clamped to the range `[min, max]`.
     */
    constexpr auto CLAMP_RANGE = [](auto x, auto min, auto max)
    {
        return CLAMP_LOW(CLAMP_HIGH(x, max), min);
    };

    /** @brief Clamps a value to the range `[min, max]` using `MIN` first.
     *
     * Takes the smaller value between `x` and `max`, then ensures the result
     * is not less than `min`.
     *
     * @param x Value to clamp.
     * @param min Lower bound.
     * @param max Upper bound.
     * @return `x` clamped to the range `[min, max]`.
     */
    constexpr auto CLAMP_MIN_THEN_LOW = [](auto x, auto min, auto max)
    {
        return CLAMP_LOW(MIN(x, max), min);
    };

    /** @brief Clamps a value to the range `[min, max]` using `CLAMP_LOW` first.
     *
     * Ensures `x` is not less than `min`, then uses `MIN` to restrict
     * the result to not exceed `max`.
     *
     * @param x Value to clamp.
     * @param min Lower bound.
     * @param max Upper bound.
     * @return `x` clamped to the range `[min, max]`.
     */
    constexpr auto CLAMP_LOW_THEN_MIN = [](auto x, auto min, auto max)
    {
        return MIN(CLAMP_LOW(x, min), max);
    };

    /** @brief Computes the absolute value.
     *
     * @param x Value.
     * @return Absolute value of `x`.
     */
    constexpr auto ABS = [](auto x)
    {
        return (x < 0) ? -x : x;
    };

    /** @brief Computes the absolute value of an integer by shifting.
     *
     * @param x Value.
     * @return Absolute value of `x`.
     */
    constexpr auto ABS_32 = [](auto x)
    {
        return (x ^ (x >> 31)) - (x >> 31);
    };

    /** @brief Computes the absolute sum of two values.
     *
     * @param a First value.
     * @param b Second value.
     * @return Absolute sum of `a` and `b`.
     */
    constexpr auto ABS_ADD = [](auto a, auto b)
    {
        return (a >= 0) ? (b + a) : (b - a);
    };

    /** @brief Computes the absolute difference between two values.
     *
     * @param a First value.
     * @param b Second value.
     * @return Absolute difference between `a` and `b`.
     */
    constexpr auto ABS_DIFF = [](auto a, auto b)
    {
        return ABS(a - b);
    };

    /** @brief Checks if two values have different signs.
     *
     * @param a First value.
     * @param b Second value.
     * @return `true` if `a` and `b` have different signs, `false` otherwise.
     */
    constexpr auto DIFF_SIGN = [](auto a, auto b)
    {
        return (a >= 0 && b < 0) || (a < 0 && b >= 0);
    };

    /** @brief Divides an integer by another using a shift.
     *
     * @note `b` must be a power of 2.
     * Supported divisors: 2, 4, 8, 16, 32, 64, 128, 256.
     *
     * @param a Numerator.
     * @param b Denominator.
     * @return Result of `a` divided by `b`.
     */
    constexpr auto DIV_FAST = [](auto a, auto b)
    {
        return ((a) >> (((b) == 2)   ? 1 :
                        ((b) == 4)   ? 2 :
                        ((b) == 8)   ? 3 :
                        ((b) == 16)  ? 4 :
                        ((b) == 32)  ? 5 :
                        ((b) == 64)  ? 6 :
                        ((b) == 128) ? 7 :
                        ((b) == 256) ? 8 :
                        0))
    }

    /** @brief Scales a large `x` before trigonometric multiplication.
     *
     * @note "Range-based scaling mechanism common in fixed-point DSP or min-level game engine math." - ChatGPT
     *
     * @param x Value to use for overflow computation.
     * @return 4 if `x` overflows, otherwise 0. */
    constexpr auto OVERFLOW_GUARD = [](auto x)
    {
        return (((uint)x + SHRT_MAX) >= USHRT_MAX) ? 4 : 0;
    };

    /** @brief Rounds a floating-point value.
     *
     * @param x Value to round.
     * @return `x` rounded to the closest integer.
     */
    constexpr float ROUND(float x)
    {
        return (float)((x > 0.0f) ? (int)(x + 0.5f) : (int)(x - 0.5f));
    }

    /** @brief Floors a floating-point value.
     *
     * @param x Value to floor.
     * @return `x` floored to the closest integer.
     */
    constexpr float FLOOR(float x)
    {
        return (float)((int)x - ((x < 0.0f) && x != (int)x));
    }

    /** @brief Floors a value to the closest step.
     *
     * @param x Value to floor.
     * @param step Flooring step.
     * @return `x` floored to the closest `step`.
     */
    constexpr auto FLOOR_TO_STEP = [](auto x, auto step)
    {
        return (x >= 0) ? (x / step) : ((x - (step - 1)) / step);
    };

    /** @brief Ceils a floating-point value.
     *
     * @param x Value to ceil.
     * @return `x` ceiled to the closest integer.
     */
    constexpr float CEIL(float x)
    {
        return (float)((int)x + ((x > 0.0f) && x != (int)x));
    }

    /** @brief Converts degrees to radians.
     *
     * @param deg Angle in degrees to covnert.
     * @return Angle converted to radians.
     */
    constexpr float DEG_TO_RAD(float deg)
    {
        return (deg * PI) / 180.0f;
    }
}
