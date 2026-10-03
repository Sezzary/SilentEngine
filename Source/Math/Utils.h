#pragma once

#include "Math/Constants.h"
#include "Math/Objects/Vector3.h"

namespace Silent::Math
{
    /** @brief Floors a value to the nearest step.
     *
     * @param x Value to floor.
     * @param step Flooring step.
     * @return `x` floored to the nearest `step`.
     */
    float FloorToStep(float x, float step);

    /** @brief Ceils a value to the nearest step.
     *
     * @param x Value to ceil.
     * @param step Ceiling step.
     * @return `x` Ceiled to the nearest `step`.
     */
    float CeilToStep(float x, float step);

    /** @brief Rounds a value to the nearest step.
     *
     * @param x Value to round.
     * @param step Rounding step.
     * @return `x` rounded to the nearest `step`.
     */
    float RoundToStep(float x, float step);

    /** @brief Remaps a value from one value range to another.
     *
     * @param x Value to remap.
     * @param fromMin Minimim value of first range.
     * @param fromMax Maximum value of first range.
     * @param toMin Minimum value of second range.
     * @param toMax Maximum value of second range.
     * @return `x` remapped from the first range to the second.
     */
    float Remap(float x, float fromMin, float fromMax, float toMin, float toMax);

    /** @brief Wraps a value to be within the range `[min, max]`.
     *
     * @param val Value to wrap.
     * @param min Lower bound.
     * @param max Upper bound.
     * @return `val` wrapped to the valid range.
     */
    int WrapRange(int val, int min, int max);

    /** @brief Wraps a value to be within the range `[0, max]`.
     *
     * @param val Value to wrap.
     * @param max Upper bound.
     * @return `val` wrapped to the valid positive range.
     */
    int WrapRange(int val, int max);

    /** @brief Checks if two values are approximately equal.
     *
     * @param a First value.
     * @param b Second value.
     * @param epsilon Comparison epsilon.
     * @return `true` if the values are approximately equal, `false` otherwise.
     */
    bool IsApproxEqual(float a, float b, float epsilon = EPSILON);

    /** @brief Computes the surface slope angle, relative to a downward axis.
     *
     * @param normal Surface normal.
     * @param axis Downward axis.
     * @return Surface slope angle.
     */
    short GetSurfaceSlopeAngle(const Vector3& normal, const Vector3& axis = Vector3::UnitY);

    /** @brief Computes the surface aspect angle, relative to a downward axis.
     *
     * @param normal Surface normal.
     * @param axis Downward axis.
     * @return Surface aspect angle.
     */
    short GetSurfaceAspectAngle(const Vector3& normal, const Vector3& axis = Vector3::UnitY);

    /** @brief Computes the distance to a line.
     *
     * @param from Start.
     * @param lineStart Line start.
     * @param lineEnd Line end.
     * @return Distance to the line.
     */
    float GetDistanceToLine(const Vector3& from, const Vector3& lineStart, const Vector3& lineEnd);

    /** @brief Gets the closest point on a line.
     *
     * @param from Start.
     * @param lineStart Line start.
     * @param lineEnd Line end.
     * @return Closest point on the line.
     */
    Vector3 GetClosestPointOnLine(const Vector3& from, const Vector3& lineStart, const Vector3& lineEnd);

    /** @brief Gets the closest point on a line, perpendicular to a downward axis.
     *
     * @param from Start.
     * @param lineStart Line start.
     * @param lineEnd Line end.
     * @param axis Downward axis.
     * @return Closest point on the line.
     */
    Vector3 GetClosestPointOnLinePerp(const Vector3& from, const Vector3& lineStart, const Vector3& lineEnd,
                                      const Vector3& axis = Vector3::UnitY);

    /** @brief Checks if a given point is in front of another, relative to a downward axis.
     *
     * @param from First point.
     * @param to Second point.
     * @param ref Reference orienting point.
     * @param axis Downward axis.
     * @return `true` if the second point is in front of the first, `false` otherwise.
     */
    bool IsPointInFront(const Vector3& from, const Vector3& to, const Vector3& ref,
                        const Vector3& axis = Vector3::UnitY);

    /** @brief Checks if a given point is in to the left of another, relative to a downward axis.
     *
     * @param from First point.
     * @param to Second point.
     * @param ref Reference orienting point.
     * @param axis Downward axis.
     * @return `true` if the second point is to the left of the first, `false` otherwise.
     */
    bool IsPointOnLeft(const Vector3& from, const Vector3& to, const Vector3& ref,
                       const Vector3& axis = Vector3::UnitY);

    /** @brief Gets the sign of a value.
     *
     * @tparam T Numeric type.
     * @param val Value to check.
     * @return `1` if the value is >= 0, `-1` otherwise.
     */
    template <typename T>
    requires std::is_arithmetic_v<T>
    int GetSign(T val)
    {
        return (val >= 0) ? 1 : -1;
    }

    /** @brief Wraps an enum entry index to the valid enum range.
     *
     * @note The enum must be sequential, start from 0, and have a final entry named `Count`.
     *
     * @param idx Entry index wrap.
     */
    template <typename Enum>
    Enum WrapEnum(int idx)
    {
        return (Enum)WrapRange(idx, (int)Enum::Count);
    }
}
