#pragma once

#include "Math/Constants.h"
#include "Math/FixedPoint/Arithmetic.h"
#include "Math/FixedPoint/Constants.h"

namespace Silent::Math
{
    // ====
    // Raw
    // ====

    /** @brief Converts a floating-point value to Q27.4 fixed-point.
     *
     * @param x Value to convert.
     * @return `x` converted to Q27.4 fixed-point.
     */
    constexpr q27_4 Q4(float x)
    {
        return TO_FIXED(x, Q4_SHIFT);
    }

    /** @brief Converts Q27.4 fixed-point value to floating-point.
     *
     * @param x Q27.4 fixedd-point value to convert.
     * @return `x` converted to floating-point.
     */
    constexpr float Q4_TO_FLT(q27_4 x)
    {
        return FP_TO_FLT(x, Q4_SHIFT);
    }

    /** @brief Converts a floating-point value to Q25.6 fixed-point.
     *
     * @param x Value to convert.
     * @return `x` converted to Q25.6 fixed-point.
     */
    constexpr q25_6 Q6(float x)
    {
        return TO_FIXED(x, Q6_SHIFT);
    }

    /** @brief Converts Q25.6 fixed-point value to floating-point.
     *
     * @param x Q25.6 fixedd-point value to convert.
     * @return `x` converted to floating-point.
     */
    constexpr float Q6_TO_FLT(q25_6 x)
    {
        return FP_TO_FLT(x, Q6_SHIFT);
    }

    /** @brief Converts a floating-point value to Q23.8 fixed-point.
     *
     * @param x Value to convert.
     * @return `x` converted to Q23.8 fixed-point.
     */
    constexpr q23_8 Q8(float x)
    {
        return TO_FIXED(x, Q8_SHIFT);
    }

    /** @brief Converts a floating-point value to clamped Q25.8 fixed-point.
     *
     * @param x Value to convert.
     * @return `x` converted to clamped Q25.8 fixed-point.
     */
    constexpr q23_8 Q8_CLAMPED(float x)
    {
        return CLAMP(Q8(x), Q8(0.0f), Q8(1.0f) - 1);
    }

    /** @brief Converts Q23.8 fixed-point value to floating-point.
     *
     * @param x Q23.8 fixedd-point value to convert.
     * @return `x` converted to floating-point.
     */
    constexpr float Q8_TO_FLT(q23_8 x)
    {
        return FP_TO_FLT(x, Q8_SHIFT);
    }

    /** @brief Converts a floating-point value to Q19.12 fixed-point.
     *
     * @param x Value to convert (`float`).
     * @return `x` converted to Q19.12 fixed-point.
     */
    constexpr q19_12 Q12(float x)
    {
        return TO_FIXED(x, Q12_SHIFT);
    }

    /** @brief Converts a floating-point value to clamped Q19.12 fixed-point.
     *
     * @param x Value to convert.
     * @return `x` converted to clamped Q19.12 fixed-point.
     */
    constexpr q19_12 Q12_CLAMPED(float x)
    {
        return CLAMP(Q12(x), Q12(0.0f), (Q12(1.0f) - 1));
    }

    /** @brief Converts Q19.12 fixed-point value to floating-point.
     *
     * @param x Q19.12 fixedd-point value to convert.
     * @return `x` converted to floating-point.
     */
    constexpr float Q12_TO_FLT(q19_12 x)
    {
        return FP_TO_FLT(x, Q12_SHIFT);
    }

    /** @brief Converts a fixed-point value from Q27.4 to Q23.8.
     *
     * @param x Q27.4 fixed-point value to convert.
     * @return `x` converted to Q23.8 fixed-point.
     */
    constexpr q23_8 Q4_TO_Q8(q27_4 x)
    {
        return x << 4;
    }

    /** @brief Converts a fixed-point value from Q27.4 to Q19.12.
     *
     * @param x Q27.4 fixed-point value to convert.
     * @return `x` converted to Q19.12 fixed-point.
     */
    constexpr q19_12 Q4_TO_Q12(q27_4 x)
    {
        return x << 8;
    }

    /** @brief Converts a fixed-point value from Q25.6 to Q19.12.
     *
     * @param x Q25.6 fixed-point value to convert.
     * @return `x` converted to Q19.12 fixed-point.
     */
    constexpr q19_12 Q6_TO_Q12(q25_6 x)
    {
        return x << 6;
    }

    /** @brief Converts a fixed-point value from Q23.8 to Q19.12.
     *
     * @param x Q23.8 fixed-point value to convert.
     * @return `x` converted to Q19.12 fixed-point.
     */
    constexpr q23_8 Q8_TO_Q12(q19_12 x)
    {
        return x << 4;
    }

    /** @brief Converts a fixed-point value from Q23.8 to Q27.4.
     *
     * @param x Q23.8 fixed-point value to convert.
     * @return `x` converted to Q27.4 fixed-point.
     */
    constexpr q27_4 Q8_TO_Q4(q23_8 x)
    {
        return x >> 4;
    }

    /** @brief Converts a fixed-point value from Q21.10 to Q19.12.
     *
     * @param x Q21.10 fixed-point value to convert.
     * @return `x` converted to Q19.12 fixed-point.
     */
    constexpr q19_12 Q10_TO_Q12(q21_10 x)
    {
        return x << 2;
    }

    /** @brief Converts a fixed-point value from Q19.12 to Q27.4.
     *
     * @param x Q19.12 fixed-point value to convert.
     * @return `x` converted to Q27.4 fixed-point.
     */
    constexpr q27_4 Q12_TO_Q4(q19_12 x)
    {
        return x >> 8;
    }

    /** @brief Converts a fixed-point value from Q19.12 to Q25.6.
     *
     * @param x Q19.12 fixed-point value to convert.
     * @return `x` converted to Q25.6 fixed-point.
     */
    constexpr q25_6 Q12_TO_Q6(q19_12 x)
    {
        return x >> 6;
    }

    /** @brief Converts a fixed-point value from Q19.12 to Q23.8.
     *
     * @param x Q19.12 fixed-point value to convert.
     * @return `x` converted to Q23.8 fixed-point.
     */
    constexpr q23_8 Q12_TO_Q8(q19_12 x)
    {
        return x >> 4;
    }

    /** @brief Converts a fixed-point value from Q19.12 to Q21.10.
     *
     * @param x Q19.12 fixed-point value to convert.
     * @return `x` converted to Q21.10 fixed-point.
     */
    constexpr q21_10 Q12_TO_Q10(q19_12 x)
    {
        return x >> 2;
    }

    /** @brief Extracts the fractional part of a value in Q19.12 fixed-point.
     *
     * @param x Q19.12 fixed-point value.
     * @return Fractional part of `x` in Q19.12 fixed-point.
     */
    constexpr q19_12 Q12_FRACT(q19_12 x)
    {
        return x & 0xFFF;
    }

    /** @brief Extracts the fractional part of a value in 23.8 fixed-point.
     *
     * @param x Q23.8 fixed-point value.
     * @return Fractional part of `x` in Q23.8 fixed-point.
     */
    constexpr q23_8 Q8_FRACT(q23_8 x)
    {
        return x & 0xFF;
    }

    // =========
    // Abstract
    // =========

    /** @brief Converts a normalized floating-point analog stick value in the range `[-1.0f, 1.0f]` to Q0.7 fixed-point,
     * clamped integer range `[-128, 127]`.
     *
     * @param analog Analog stick value (`float`).
     * @return Analog stick value in Q0.7 fixed-point, clamped integer range `[-128, 127]` (`q0_7`).
     */
    constexpr q0_7 FP_STICK(float analog)
    {
        return ((analog) >= 0) ? CLAMP(Q8(analog) / 2, 0, (Q8(1.0f) / 2) - 1) :
                                -CLAMP(Q8(ABS(analog)) / 2, 0, Q8(1.0f) / 2);
    }

    /** @brief Converts a normalized floating-point color component in the range `[0.0f, 1.0f]` to Q0.8 fixed-point,
     * integer range `[0, 255]`.
     *
     * TODO: Deprecated, don't use. Doesn't make sense to have `float` color components in this project.
     *
     * @param comp Floating-point color component.
     * @return Q0.8 fixed-point color component, clamped integer range `[0, 255]`.
     */
    constexpr q0_8 Q8_COLOR(float comp)
    {
        return (q0_8)(comp * (FP_TO(1.0f, Q8_SHIFT) - 1));
    }

    /** @brief Converts an 8-bit color value in the range `[0, 255]` to a normalized color format in the range
     * `[0.0f, 1.0f]`.
     *
     * @param comp Fixed-point color component.
     * @return Floating-point color component.
     */
    constexpr float Q8_COLOR_FROM(q0_8 comp)
    {
        return (comp == Q8_COLOR(1.0f)) ? 1.0f : std::clamp((float)comp / (float)FP_TO(1.0f, Q8_SHIFT), 0.0f, 1.0f);
    }

    /** @brief Converts floating-point degrees to signed Q3.12 fixed-point, full rotation integer range `[0, 4096]`.
     *
     * This angle format is used in world space.
     *
     * @note 1 degree = 11.377778 units.
     *
     * @param deg Degrees (`float`).
     * @return Unsigned Q3.12 fixed-point angle, full rotation integer range `[0, 4096]`.
     */
    constexpr q3_12 Q12_ANGLE(float deg)
    {
        return Q12(deg / 360.0f);
    }

    /** @brief Converts floating-point radians to fixed-point degrees in Q3.12 format.
     *
     * @param rad Angle in radians.
     * @return Unsigned Q3.12 fixed-point angle, full rotation integer range `[0, 4096]`.
     */
    constexpr q3_12 Q12_ANGLE_FROM_RAD(float rad)
    {
        return Q12_ANGLE(rad / (PI / 180.0f));
    }

    /** @brief Converts fixed-point degrees in Q3.12 format to floating-point radians.
     *
     * @param deg Angle in degrees.
     * @return Unsigned Q3.12 fixed-point angle, full rotation integer range `[0, 4096]`
     */
    constexpr float Q12_ANGLE_TO_RAD(q3_12 deg)
    {
        return (deg * (360.0f / (float)Q12_ANGLE(360.0f))) * (PI / 180.0f);
    }

    /** @brief Converts floating-point degrees to unsigned Q0.8 fixed-point, clamped full rotation integer range
     * `[0, 255]`.
     *
     * This angle format is used in map data.
     *
     * @note 1 degree = 0.711111 units.
     *
     * @param deg Angle in degrees.
     * @return Unsigned Q0.8 fixed-point angle, clamped full rotation integer range `[0, 255]`.
     */
    constexpr q0_8 Q8_ANGLE(float deg)
    {
        return Q8_CLAMPED(deg / 360.0f);
    }

    /** @brief Converts an unsigned Q0.8 fixed-point angle, full rotation integer range `[0, 255]` to
     * unsigned Q3.12 fixed-point, full rotation integer range `[0, 4096]`.
     *
     * @param angle Unsigned Q0.8 fixed-point angle, full rotation integer range `[0, 255]`.
     * @return Unsigned Q3.12 fixed-point angle, full rotation integer range `[0, 4096]`.
     */
    constexpr q3_12 Q12_ANGLE_FROM_Q8(q0_8 angle)
    {
        return Q8_TO_Q12(angle);
    }

    /** @brief Normalizes a signed Q3.12 fixed-point angle to the clamped unsigned integer range `[0, 4095]`.
     *
     * @note Has the same effect as `Q12_ANGLE_NORM_U`. Could they somehow be combined?
     *
     * @param angle Signed Q3.12 fixed-point angle, full rotation integer range `[-2048, 2047]`.
     * @return Unsigned Q3.12 fixed-point angle, wrapped to the clamped integer range `[0, 4095]`.
     */
    constexpr q3_12 Q12_ANGLE_ABS(q3_12 angle)
    {
        return Q12_FRACT(angle + Q12_ANGLE(360.0f));
    }

    /** @brief Normalizes an unsigned Q3.12 fixed-point angle to the clamped signed integer range `[-2048, 2047]`.
     *
     * @param angle Unsigned Q3.12 fixed-point angle, full rotation integer range `[0, 4095]`.
     * @return Signed Q3.12 fixed-point angle wrapped to the clamped integer range `[-2048, 2047]`.
     */
    constexpr q3_12 Q12_ANGLE_NORM_S(q19_12 angle)
    {
        return (angle << 20) >> 20;
    }

    /** @brief Normalizes a signed Q3.12 fixed-point angle to the clamped unsigned range `[0, 4095]`.
     *
     * @param angle Signed Q3.12 fixed-point angle, full rotation integer range `[-2048, 2047]`.
     * @return Unsigned Q3.12 fixed-point angle, wrapped to the clamped integer range `[0, 4095]`.
     */
    constexpr q3_12 Q12_ANGLE_NORM_U(q3_12 angle)
    {
        return angle & (Q12_ANGLE(360.0f) - 1);
    }

    /** @brief Converts floating-point radians in the range `[-PI, PI]` to the fixed-point full rotation,
     * integer range `[0, 20480]`.
     *
     * This angle format is only used in `vcSetDataToVwSystem`.
     *
     * @note π = 10240 units.
     *
     * @param rad Angle in radians.
     * @return Fixed-point radian representation, full rotation integer range `[0, 20480]`.
     */
    constexpr int FP_RADIAN(float rad)
    {
        return (((rad < 0.0f) ? (PI + (PI - ABS(rad))) : rad) * ((float)FP_PI / PI)) *
               ((rad < 0.0f || rad >= PI) ? 1.0f : 2.0f);
    }

    /** @brief Computes the square 2D distance between two positions in Q19.12 fixed-point,
     * using Q21.8 fixed-point intermediates to avoid overflow.
     *
     * @param from First Q19.12 position.
     * @param to Second Q19.12 position.
     * @param return 2D Q19.12 distance between two positions.
     */
    #define Q12_2D_DISTANCE_SQR(from, to)        \
        SQUARE(Q12_TO_Q8((to).vx - (from).vx)) + \
        SQUARE(Q12_TO_Q8((to).vz - (from).vz));
}
