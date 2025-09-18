#pragma once

#include "Math/Constants.h"

namespace Silent::Math
{
    class Matrix;

    class Vector2 : public glm::vec2
    {
    public:
        // Constants

        static constexpr uint AXIS_COUNT = 2;

        // Presets

        static const Vector2 Zero;
        static const Vector2 One;
        static const Vector2 UnitX;
        static const Vector2 UnitY;

        // Constructors

        constexpr Vector2() = default;
        constexpr Vector2(const glm::vec2& vec) : glm::vec2(vec) {}
        constexpr Vector2(float x)              : glm::vec2(x) {}
        constexpr Vector2(float x, float y)     : glm::vec2(x, y) {}

        // Utilities

        float Length() const;
        float LengthSquared() const;
        
        static float Distance(const Vector2& from, const Vector2& to);
        static float DistanceSquared(const Vector2& from, const Vector2& to);
        static float Dot(const Vector2& vec0, const Vector2& vec1);
        static float Cross(const Vector2& vec0, const Vector2& vec1);

        static Vector2 Min(const Vector2& vec0, const Vector2& vec1);
        void           Min(const Vector2& vec);
        static Vector2 Max(const Vector2& vec0, const Vector2& vec1);
        void           Max(const Vector2& vec);
        static Vector2 Clamp(const Vector2& vec, const Vector2& min, const Vector2& max);
        void           Clamp(const Vector2& min, const Vector2& max);
        static Vector2 Normalize(const Vector2& vec);
        void           Normalize();
        static Vector2 Lerp(const Vector2& from, const Vector2& to, float alpha);
        void           Lerp(const Vector2& to, float alpha);
        static Vector2 Smoothstep(const Vector2& from, const Vector2& to, float alpha);
        void           Smoothstep(const Vector2& to, float alpha);
        static Vector2 Transform(const Vector2& vec, const Matrix& mat);
        void           Transform(const Matrix& mat);
        static Vector2 Rotate(const Vector2& vec, const Matrix& mat);
        void           Rotate(const Matrix& mat);
        static Vector2 Translate(const Vector2& vec, const Vector2& dir, float dist);
        void           Translate(const Vector2& dir, float dist);

        static bool Compare(const Vector2& vec0, const Vector2& vec1, float epsilon = EPSILON);
    
        // Converters

        const glm::vec2& ToGlmVec2() const;
        glm::vec2&       ToGlmVec2();

        // Operators

        bool     operator==(const Vector2& vec) const;
        bool     operator!=(const Vector2& vec) const;
        Vector2& operator=(const Vector2& vec) = default;
        Vector2& operator+=(const Vector2& vec);
        Vector2& operator-=(const Vector2& vec);
        Vector2& operator*=(const Vector2& vec);
        Vector2& operator*=(float scalar);
        Vector2& operator/=(const Vector2& vec);
        Vector2& operator/=(float scalar);
        Vector2  operator+(const Vector2& vec) const;
        Vector2  operator-(const Vector2& vec) const;
        Vector2  operator*(const Vector2& vec) const;
        Vector2  operator*(float scalar) const;
        Vector2  operator/(const Vector2& vec) const;
        Vector2  operator/(float scalar) const;
        Vector2  operator-() const;
    };
}
