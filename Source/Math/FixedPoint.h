#pragma once

#include "Math/Constants.h"

namespace Silent::Math
{
    constexpr uint Q4_SHIFT       = 4;
    constexpr uint Q6_SHIFT       = 6;
    constexpr uint Q8_SHIFT       = 8;
    constexpr uint Q12_SHIFT      = 12;
    constexpr uint FP_ANGLE_COUNT = 1 << Q12_SHIFT;
    constexpr int  FP_PI          = 0x5000 / 2;

    // =====================
    // Arithmetic and Utils
    // =====================

    /** @brief Converts an integer to a fixed-point Q format.
     *
     * @note Deprecated.
     *
     * @param x `int` to convert.
     * @param shift Fixed-point shift.
     * @return `x` converted to fixed-point.
     */
    constexpr int FP_TO(float x, uint shift)
    {
        return (int)ROUND(x * (1 << shift));
    }

    /** @brief Converts an integer from a fixed-point Q format. */
    constexpr int FP_FROM(int x, uint shift)
    {
        return x >> shift;
    }

    /** @brief Converts an integet from a fixed-point Q format to floating-point. */
    constexpr float FP_FLOAT(int x, uint shift)
    {
        return (float)x / (float)FP_TO(1.0f, shift);
    }

    /** @brief Converts an integer from a scaled fixed-point Q format rounded to the nearest value.
     *
     * @param x Fixed-point value to convert.
     * @param scale Fixed-point scale.
     * @param shift Fixed-point shift.
     * @return `x` rounded and converted from fixed-point.
     */
    #define FP_ROUND_SCALED(x, scale, shift) \
        (((x) + ((FP_TO(1.0f, shift) * (scale)) - 1)) / (FP_TO(1.0f, shift) * (scale)))

    /** @brief Converts an integer from a fixed-point Q format rounded toward 0.
     *
     * @param x Fixed-point value to convert.
     * @param shift Fixed-point shift.
     * @return `x` rounded toward 0 and converted from fixed-point.
     */
    #define FP_ROUND_TO_ZERO(x, shift) \
        ((s32)(FP_FROM(x, shift) + ((u32)(x) >> 31)) >> 1)

    /** @brief Multiplies two integers in a fixed-point Q format and converts the result from the fixed-point Q format. */
    constexpr int FP_MULTIPLY(int a, int b, uint shift)
    {
        return (a * b) >> shift;
    }

    /** @brief Multiplies two integers in a fixed-point Q format, using 64-bit intermediates for higher precision,
     * and converts the result from the fixed-point Q format.
     */
    constexpr int FP_MULTIPLY_PRECISE(int a, int b, uint shift)
    {
        return ((int64)a * (int64)b) >> shift;
    }

    /** @brief Multiplies an integer by a float converted to fixed-point Q format and converts the result from the fixed-point Q format. */
    constexpr int FP_MULTIPLY_FLOAT(int a, float b, uint shift)
    {
        return FP_MULTIPLY(a, FP_TO(b, shift), shift);
    }

    /** @brief Multiplies an integer by a float converted to fixed-point Q format, using a 64-bit intermediate for higher precision. */
    constexpr int FP_MULTIPLY_FLOAT_PRECISE(int a, float b, uint shift)
    {
        return FP_MULTIPLY((int64)a, FP_TO(b, shift), shift);
    }

    // ==================================
    // Raw Q Format Conversion and Utils
    // ==================================

    /** @brief Converts a floating-point value to Q*.4 fixed-point.
     *
     * @param x Value to convert (`float`).
     * @return `x` converted to Q*.4 fixed-point.
     */
    #define Q4(x) \
        FP_TO(x, Q4_SHIFT)

    /** @brief Converts a floating-point value to Q*.8 fixed-point.
     *
     * @param x Value to convert (`float`).
     * @return `x` converted to Q*.8 fixed-point.
     */
    #define Q8(x) \
        FP_TO(x, Q8_SHIFT)

    /** @brief Converts a floating-point value to clamped Q*.8 fixed-point.
     *
     * @param x Value to convert (`float`).
     * @return `x` converted to clamped Q*.8 fixed-point.
     */
    #define Q8_CLAMPED(x) \
        CLAMP(Q8(x), 0, Q8(1.0f) - 1)

    /** @brief Converts a floating-point value to Q*.12 fixed-point.
     *
     * @param x Value to convert (`float`).
     * @return `x` converted to Q*.12 fixed-point.
     */
    #define Q12(x) \
        FP_TO(x, Q12_SHIFT)

    /** @brief Converts a floating-point value to clamped Q*.12 fixed-point.
     *
     * @param x Value to convert (`float`).
     * @return `x` converted to clamped Q*.12 fixed-point.
     */
    #define Q12_CLAMPED(x) \
        CLAMP(Q12(x), 0, Q12(1.0f) - 1)

    /** @brief Converts a fixed-point value from Q*.4 to Q*.8.
     *
     * @param x Q*.4 fixed-point value to convert.
     * @return `x` converted to Q*.8 fixed-point.
     */
    #define Q4_TO_Q8(x) \
        ((x) << 4)

    /** @brief Converts a fixed-point value from Q*.4 to Q*.12.
     *
     * @param x Q*.4 fixed-point value to convert.
     * @return `x` converted to Q*.12 fixed-point.
     */
    #define Q4_TO_Q12(x) \
        ((x) << 8)

    /** @brief Converts a fixed-point value from Q*.6 to Q*.12.
     *
     * @param x Q*.6 fixed-point value to convert.
     * @return `x` converted to Q*.12 fixed-point.
     */
    #define Q6_TO_Q12(x) \
        ((x) << 6)

    /** @brief Converts a fixed-point value from Q*.8 to Q*.12.
     *
     * @param x Q*.8 fixed-point value to convert.
     * @return `x` converted to Q*.12 fixed-point.
     */
    #define Q8_TO_Q12(x) \
        ((x) << 4)

    /** @brief Converts a fixed-point value from Q*.8 to Q*.4.
     *
     * @param x Q*.8 fixed-point value to convert.
     * @return `x` converted to Q*.4 fixed-point.
     */
    #define Q8_TO_Q4(x) \
        ((x) >> 4)

    /** @brief Converts a fixed-point value from Q*.12 to Q*.4.
     *
     * @param x Q*.12 fixed-point value to convert.
     * @return `x` converted to Q*.4 fixed-point.
     */
    #define Q12_TO_Q4(x) \
        ((x) >> 8)

    /** @brief Converts a fixed-point value from Q*.12 to Q*.6.
     *
     * @param x Q*.12 fixed-point value to convert.
     * @return `x` converted to Q*.6 fixed-point.
     */
    #define Q12_TO_Q6(x) \
        ((x) >> 6)

    /** @brief Converts a fixed-point value from Q*.12 to Q*.8.
     *
     * @param x Q*.12 fixed-point value to convert.
     * @return `x` converted to Q*.8 fixed-point.
     */
    #define Q12_TO_Q8(x) \
        ((x) >> 4)

    /** @brief Extracts the fractional part of a value in Q*.12 fixed-point.
     *
     * @param x Q*.12 fixed-point value.
     * @return Fractional part of `x` in Q*.12 fixed-point.
     */
    #define Q12_FRACT(x) \
        ((x) & 0xFFF)

    /** @brief Converts a normalized color value in the range `[0.0f, 1.0f]` to an 8-bit color format in the range `[0, 255]`. */
    constexpr uchar FP_COLOR(float val)
    {
        return (uchar)(val * (FP_TO(1.0f, Q8_SHIFT) - 1));
    }

    /** @brief Converts an 8-bit color value in the range `[0, 255]` to a normalized color format in the range `[0.0f, 1.0f]`. */
    constexpr float FP_COLOR_FROM(uchar val)
    {
        return (val == FP_COLOR(1.0f)) ? 1.0f : std::clamp((float)val / (float)FP_TO(1.0f, Q8_SHIFT), 0.0f, 1.0f);
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

    /** @brief Wraps fixed-point degrees in Q3.12 format to the range of a single turn. */
    constexpr int FP_ANGLE_TRUNCATE(int angle)
    {
        return (angle << 20) >> 20;
    }

    /** @brief Wraps fixed-point degrees in Q3.12 format to the range of a single turn and computes the absolute value. */
    constexpr int FP_ANGLE_TRUNCATE_ABS(int angle)
    {
        return (FP_ANGLE_TRUNCATE(angle) ^ (angle >> 31)) - (angle >> 31);
    }

    /** @brief Converts floating-point radians in the range `[-PI, PI]` to fixed-point radians in the range `[0, 0x5000]`. */
    constexpr int FP_RADIAN(float rad)
    {
        return (((rad < 0.0f) ? (PI + (PI - ABS(rad))) : rad) * ((float)FP_PI / PI)) *
               ((rad < 0.0f || rad >= PI) ? 1.0f : 2.0f);
    }
}
