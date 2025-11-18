#pragma once

#include "Math/Objects/Vector2.h"

namespace Silent::Math
{
    class BezierCurve2
    {
    private:
        // ==========
        // Constants
        // ==========

        static constexpr auto CONTROL_POINT_COUNT = 4;

        // ========
        // Fields
        // ========

        std::array<Vector2, CONTROL_POINT_COUNT> _controlPoints = {};

    public:
        // ========
        // Presets
        // ========

        static const BezierCurve2 Zero;
        static const BezierCurve2 Linear;
        static const BezierCurve2 EaseIn;
        static const BezierCurve2 EaseOut;
        static const BezierCurve2 EaseInOut;

        // =============
        // Constructors
        // =============

        /** @brief Constructs a default uninitialised `BezierCurve2`. */
        BezierCurve2() = default;

        /** @brief Constructs a defined `BezierCurve2`.
         *
         * @param start Start point.
         * @param end End point.
         * @param startHandle Start point's handle.
         * @param endHandle End point's handle.
         */
        BezierCurve2(const Vector2& start, const Vector2& end, const Vector2& startHandle, const Vector2& endHandle);

        // ========
        // Getters
        // ========

        /** @brief Gets the start point.
         *
         * @return Start point.
         */
        const Vector2& GetStart() const;

        /** @brief Gets the end point.
         *
         * @return End point.
         */
        const Vector2& GetEnd() const;

        /** @brief Gets the start point's handle.
         *
         * @return Start point's handle.
         */
        const Vector2& GetStartHandle() const;

        /** @brief Gets the end point's handle.
         *
         * @return End point's handle.
         */
        const Vector2& GetEndHandle() const;

        /** @brief Computes a point at a given alpha along the curve using De Casteljau interpolation.
         *
         * @param alpha Alpha along the curve.
         */
        Vector2 GetPoint(float alpha) const;

        /** @brief Computes a Y position on the curve corresponding to a given X position using Newton-Raphson iteration.
         *
         * @param x X position forresponding to the desired Y position..
         */
        float GetY(float x) const;

        // ========
        // Setters
        // ========

        /** @brief Sets the start point.
         *
         * @param point New start point.
         */
        void SetStart(const Vector2& point);

        /** @brief Sets the end point.
         *
         * @param point New end point.
         */
        void SetEnd(const Vector2& point);

        /** @brief Sets the start point's handle.
         *
         * @param point New start point handle.
         */
        void SetStartHandle(const Vector2& point);

        /** @brief Sets the end point's handle.
         *
         * @param point New end point handle.
         */
        void SetEndHandle(const Vector2& point);

        // ==========
        // Operators
        // ==========

        bool operator==(const BezierCurve2& curve) const;
        bool operator!=(const BezierCurve2& curve) const;

    private:
        // ========
        // Helpers
        // ========

        /** @brief Computes the derivative at a given curve alpha using De Casteljau interpolation.
         *
         * @param alpha Curve alpha to use in the computation.
         */
        Vector2 GetDerivative(float alpha) const;
    };
}
