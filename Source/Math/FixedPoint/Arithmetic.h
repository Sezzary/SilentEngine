#pragma once

#include "Math/Constants.h"
#include "Math/FixedPoint/Constants.h"

namespace Silent::Math
{
    /** @brief Converts an integer to a fixed-point Q format.
     *
     * @note Deprecated.
     *
     * @param x Integer to convert.
     * @param shift Fixed-point shift.
     * @return `x` converted to fixed-point.
     */
    constexpr int FP_TO(float x, int shift)
    {
        return (int)ROUND(x * (1 << shift));
    }

    /** @brief Converts a float to a fixed-point Q format.
     *
     * @param x Floating-point value to convert.
     * @param shift Fixed-point shift.
     * @return `x` converted to fixed-point.
     */
    constexpr int TO_FIXED(float x, int shift)
    {
        return x * (1 << shift);
    }

    /** @brief Converts an integer from a fixed-point Q format.
     *
     * @param x Integer to convert.
     * @param shift Fixed-point shift.
     * @return `x` converted from fixed-point.
     */
    constexpr int FP_FROM(int x, int shift)
    {
        return x >> shift;
    }

    /** @brief Converts an integer from a fixed-point Q format to floating-point.
     *
     * @param x Fixed-point value to convert.
     * @param shift Fixed-point shift.
     * @return `x` converted to floating-point.
     */
    constexpr float FP_TO_FLT(int x, int shift)
    {
        return (float)x / (float)FP_TO(1.0f, shift);
    }

    /** @brief Floors a fixed-point value.
     *
     * @param x Fixed-point value to floor.
     * @param shift Fixed-point shift.
     * @return Fixed-point result of `x` floored to the closest integer.
     */
    constexpr int FP_FLOOR(int x, int shift)
    {
        return TO_FIXED(FP_FROM(x, shift), shift);
    }

    /** @brief Truncates a value in a fixed-point Q format toward zero.
     *
     * @param x Fixed-point value to truncate.
     * @return Fixed-point result of `x` truncated toward zero.
     */
    constexpr int FP_TRUNCATE(int x, int shift)
    {
        return TO_FIXED(x / TO_FIXED(1.0f, shift), shift);
    }

    /** @brief Converts an integer from a scaled fixed-point Q format rounded to the nearest value.
     *
     * @param x Fixed-point value to convert.
     * @param scale Fixed-point scale.
     * @param shift Fixed-point shift.
     * @return `x` rounded and converted from fixed-point.
     */
    constexpr int FP_ROUND_SCALED(int x, int scale, int shift)
    {
        return (x + ((TO_FIXED(1.0f, shift) * scale) - 1)) / (TO_FIXED(1.0f, shift) * scale);
    }

    /** @brief Converts an integer from a fixed-point Q format rounded toward 0.
     *
     * @param x Fixed-point value to convert.
     * @param shift Fixed-point shift.
     * @return `x` rounded toward 0 and converted from fixed-point.
     */
    constexpr int FP_ROUND_TO_ZERO(int x, int shift)
    {
        return (FP_FROM(x, shift) + (x >> 31)) >> 1;
    }

    /** @brief Multiplies two integers in a fixed-point Q format.
     *
     * @param a First fixed-point factor.
     * @param b Second fixed-point factor.
     * @param shift Fixed-point shift.
     * @return Fixed-point product of `a` and `b`.
     */
    constexpr int FP_MULTIPLY(int a, int b, int shift)
    {
        return (a * b) >> shift;
    }

    /** @brief Multiplies two integers in a fixed-point Q format.
     * Alternative to `FP_MULTIPLY` using division instead of a bitwise shift.
     *
     * @param a First fixed-point factor.
     * @param b Second fixed-point factor.
     * @param shift Fixed-point shift.
     * @return Fixed-point product of `a` and `b`.
     */
    constexpr int FP_MULTIPLY_ALT(int a, int b, int shift)
    {
        return (a * b) / TO_FIXED(1.0f, shift);
    }

    /** @brief Multiplies two integers in a fixed-point Q format, using 64-bit intermediates for higher precision.
     *
     * @param a First fixed-point factor.
     * @param b Second fixed-point factor.
     * @param shift Fixed-point shift.
     * @return Precise fixed-point product of `a` and `b`.
     */
    constexpr int FP_MULTIPLY_PRECISE(int a, int b, int shift)
    {
        return ((int64)a * (int64)b) >> shift;
    }

    /** @brief Multiplies an integer in a fixed-point Q format by a float converted to a fixed-point Q format.
     *
     * @param a First fixed-point factor.
     * @param b Second floating-point factor.
     * @param shift Fixed-point shift.
     * @return Fixed-point product of `a` and `b`.
     */
    constexpr int FP_MULTIPLY_FLOAT(int a, float b, int shift)
    {
        return FP_MULTIPLY(a, FP_TO(b, shift), shift);
    }

    /** @brief Multiplies an integer in a fixed-point Q format by a float converted to fixed-point Q format,
     * using a 64-bit intermediates for higher precision.
     *
     * @param a First fixed-point factor.
     * @param b Second floating-point factor.
     * @param shift Fixed-point shift.
     * @return Precise product of `a` and `b` converted from fixed-point.
     */
    constexpr int FP_MULTIPLY_FLOAT_PRECISE(int a, float b, int shift)
    {
        return FP_MULTIPLY((int64)(a), (int64)TO_FIXED(b, shift), shift);
    }

    /** @brief Divides an integer in a fixed-point Q format by another.
     *
     * @param a Fixed-point numerator.
     * @param b Fixed-point denominator.
     * @return Fixed-point result of `a` divided by `b`.
     */
    constexpr int FP_DIVIDE(int a, int b, int shift)
    {
        return (a << shift) / b;
    }

    /** @brief Squares a fixed-point value.
     *
     * @param x Fixed-point value to be squared.
     * @param shift Fixed-point shift.
     * @return Fixed-point square of `x`.
     */
    constexpr int FP_SQUARE(int x, int shift)
    {
        return FP_MULTIPLY(x, x, shift);
    }

    /** @brief Squares a fixed-point value, using 64-bit intermediates for higher precision.
     *
     * @param x Fixed-point value to be squared.
     * @param shift Fixed-point shift.
     * @return Fixed-point square of `x`.
     */
    constexpr int FP_SQUARE_PRECISE(int x, int shift)
    {
        return FP_MULTIPLY_PRECISE(x, x, shift);
    }

    /** @brief Floors a Q19.12 fixed-point value.
     *
     * @param x Q19.12 fixed-point value to floor.
     * @return Q19.12 result of `x` floored to the closest integer.
     */
    constexpr q19_12 Q12_FLOOR(q19_12 x)
    {
        return FP_FLOOR(x, Q12_SHIFT);
    }

    /** @brief Truncates a Q19.12 fixed-point value toward zero.
     *
     * @param x Q19.12 fixed-point value to truncate.
     * @return Q19.12 result of `x` truncated toward zero.
     */
    constexpr q19_12 Q12_TRUNC(q19_12 x, int shift)
    {
        return FP_TRUNCATE(x, Q12_SHIFT);
    }

    /** @brief Multiplies two integers in Q19.12 fixed-point.
     *
     * @param a First Q19.12 fixed-point factor.
     * @param b Second Q19.12 fixed-point factor.
     * @return Q19.12 product of `a` and `b`.
     */
    constexpr q19_12 Q12_MULT(q19_12 a, q19_12 b)
    {
        return FP_MULTIPLY(a, b, Q12_SHIFT);
    }

    /** @brief Multiplies two integers in Q19.12 fixed-point.
     * Alternative to `Q12_MULT` using division instead of a bitwise shift.
     *
     * @param a First Q19.12 fixed-point factor.
     * @param b Second Q19.12 fixed-point factor.
     * @return Q19.12 product of `a` and `b`.
     */
    constexpr q19_12 Q12_MULT_ALT(q19_12 a, q19_12 b)
    {
        return FP_MULTIPLY_ALT(a, b, Q12_SHIFT);
    }

    /** @brief Multiplies two integers in Q19.12 fixed-point, using 64-bit intermediates for higher precision.
     *
     * @param a First Q19.12 fixed-point factor.
     * @param b Second Q19.12 fixed-point factor.
     * @return Precise Q19.12 product of `a` and `b`.
     */
    constexpr q19_12 Q12_MULT_PRECISE(q19_12 a, q19_12 b)
    {
        return FP_MULTIPLY_PRECISE(a, b, Q12_SHIFT);
    }

    /** @brief Multiplies an integer in Q19.12 fixed-point by a float converted to Q19.12 fixed-point.
     *
     * @param a First Q19.12 fixed-point factor.
     * @param b Second floating-point factor.
     * @return Q19.12 product of `a` and `b`.
     */
    constexpr q19_12 Q12_MULT_FLOAT(q19_12 a, float b)
    {
        return FP_MULTIPLY_FLOAT(a, b, Q12_SHIFT);
    }

    /** @brief Multiplies an integer in Q19.12 fixed-point by a float converted to Q19.12 fixed-point,
     * using 64-bit intermediates for higher precision.
     *
     * @param a First Q19.12 fixed-point factor.
     * @param b Second floating-point factor.
     * @return Precise Q19.12 product of `a` and `b`.
     */
    constexpr q19_12 Q12_MULT_FLOAT_PRECISE(q19_12 a, float b)
    {
        return FP_MULTIPLY_FLOAT_PRECISE(a, b, Q12_SHIFT);
    }

    /** @brief Divides an integer in Q19.12 fixed-point by another.
     *
     * @param a Q19.12 fixed-point numerator.
     * @param b Q19.12 fixed-point denominator.
     * @return Q19.12 result of `a` divided by `b`.
     */
    constexpr q19_12 Q12_DIV(q19_12 a, q19_12 b)
    {
        return FP_DIVIDE(a, b, Q12_SHIFT);
    }

    /** @brief Squares a Q19.12 value.
     *
     * @param x Q19.12 value to be squared.
     * @return Q19.12 square of `x`.
     */
    constexpr q19_12 Q12_SQUARE(q19_12 x)
    {
        return FP_SQUARE(x, Q12_SHIFT);
    }

    /** @brief Squares a fixed-point value, using 64-bit intermediates for higher precision.
     *
     * @param x Q19.12 value to be squared.
     * @return Q19.12 square of `x`.
     */
    constexpr q19_12 Q12_SQUARE_PRECISE(q19_12 x)
    {
        return FP_SQUARE_PRECISE(x, Q12_SHIFT);
    }
}
