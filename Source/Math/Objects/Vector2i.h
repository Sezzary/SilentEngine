#pragma once

namespace Silent::Math
{
    class Matrix;
    class Vector2;

    class Vector2i : public glm::ivec2
    {
    public:
        // Constants

        static constexpr uint AXIS_COUNT = 2;

        // Presets

        static const Vector2i Zero;
        static const Vector2i One;
        static const Vector2i UnitX;
        static const Vector2i UnitY;

        // Constructors

        constexpr Vector2i() = default;
        constexpr Vector2i(const glm::ivec2& vec) : glm::ivec2(vec) {}
        constexpr Vector2i(int x)                 : glm::ivec2(x) {}
        constexpr Vector2i(int x, int y)          : glm::ivec2(x, y) {}

        // Utilities

        float Length() const;
        float LengthSquared() const;

        static float Distance(const Vector2i& from, const Vector2i& to);
        static float DistanceSquared(const Vector2i& from, const Vector2i& to);

        static Vector2i Min(const Vector2i& vec0, const Vector2i& vec1);
        void            Min(const Vector2i& vec);
        static Vector2i Max(const Vector2i& vec0, const Vector2i& vec1);
        void            Max(const Vector2i& vec);
        static Vector2i Clamp(const Vector2i& vec, const Vector2i& min, const Vector2i& max);
        void            Clamp(const Vector2i& min, const Vector2i& max);
        static Vector2i Lerp(const Vector2i& from, const Vector2i& to, float alpha);
        void            Lerp(const Vector2i& to, float alpha);
        static Vector2i Transform(const Vector2i& vec, const Matrix& mat);
        void            Transform(const Matrix& mat);
        static Vector2i Translate(const Vector2i& vec, const Vector2& dir, float dist);
        void            Translate(const Vector2& dir, float dist);

        // Converters

        Vector2 ToVector2(int shift = NO_VALUE) const;

        const glm::ivec2& ToGlmVec2i() const;
        glm::ivec2&       ToGlmVec2i();

        // Operators

        bool      operator==(const Vector2i& vec) const;
        bool      operator!=(const Vector2i& vec) const;
        Vector2i& operator=(const Vector2i& vec) = default;
        Vector2i& operator+=(const Vector2i& vec);
        Vector2i& operator-=(const Vector2i& vec);
        Vector2i& operator*=(const Vector2i& vec);
        Vector2i& operator*=(float scalar);
        Vector2i& operator/=(const Vector2i& vec);
        Vector2i& operator/=(float scalar);
        Vector2i  operator+(const Vector2i& vec) const;
        Vector2i  operator-(const Vector2i& vec) const;
        Vector2i  operator*(const Vector2i& vec) const;
        Vector2i  operator*(float scalar) const;
        Vector2i  operator/(const Vector2i& vec) const;
        Vector2i  operator/(float scalar) const;
        Vector2i  operator-() const;
    };
}
