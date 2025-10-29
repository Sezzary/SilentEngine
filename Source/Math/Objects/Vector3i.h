#pragma once

#include "Math/Constants.h"

namespace Silent::Math
{
    class Matrix;
    class Vector3;

    class Vector3i : public glm::ivec3
    {
    public:
        // Constants

        static constexpr uint AXIS_COUNT = 3;

        // Presets

        static const Vector3i Zero;
        static const Vector3i One;
        static const Vector3i UnitX;
        static const Vector3i UnitY;
        static const Vector3i UnitZ;

        // Constructors

        constexpr Vector3i() = default;
        constexpr Vector3i(const glm::ivec3& vec) : glm::ivec3(vec) {}
        constexpr Vector3i(int x)                 : glm::ivec3(x) {}
        constexpr Vector3i(int x, int y, int z)   : glm::ivec3(x, y, z) {}

        // Utilities

        float Length() const;
        float LengthSquared() const;

        static float Distance(const Vector3i& from, const Vector3i& to);
        static float DistanceSquared(const Vector3i& from, const Vector3i& to);

        static Vector3i Min(const Vector3i& vec0, const Vector3i& vec1);
        void            Min(const Vector3i& vec);
        static Vector3i Max(const Vector3i& vec0, const Vector3i& vec1);
        void            Max(const Vector3i& vec);
        static Vector3i Clamp(const Vector3i& vec, const Vector3i& min, const Vector3i& max);
        void            Clamp(const Vector3i& min, const Vector3i& max);
        static Vector3i Lerp(const Vector3i& from, const Vector3i& to, float alpha);
        void            Lerp(const Vector3i& to, float alpha);
        static Vector3i Transform(const Vector3i& vec, const Matrix& mat);
        void            Transform(const Matrix& mat);
        static Vector3i Translate(const Vector3i& vec, const Vector3& dir, float dist);
        void            Translate(const Vector3& dir, float dist);

        // ===========
        // Converters
        // ===========

        /** @brief Converts to a `Vector3` with an optional Q shift if the components are in a fixed-point format.
         *
         * @param shift Fixed-point Q shift.
         * @return `Vector3` conversion result.
         */
        Vector3 ToVector3(int shift = NO_VALUE) const;

        /** @brief Converts to the read-only `glm::ivec3` base object.
         *
         * @return Ready-only `glm::ivec3` base object.
         */
        const glm::ivec3& ToGlmVec3i() const;

        /** @brief Converts to the writable `glm::ivec3` base object.
         *
         * @return Writable `glm::ivec3` base object.
         */
        glm::ivec3& ToGlmVec3i();

        // ==========
        // Operators
        // ==========

        bool      operator==(const Vector3i& vec) const;
        bool      operator!=(const Vector3i& vec) const;
        Vector3i& operator=(const Vector3i& vec) = default;
        Vector3i& operator+=(const Vector3i& vec);
        Vector3i& operator-=(const Vector3i& vec);
        Vector3i& operator*=(const Vector3i& vec);
        Vector3i& operator*=(float scalar);
        Vector3i& operator/=(const Vector3i& vec);
        Vector3i& operator/=(float scalar);
        Vector3i  operator+(const Vector3i& vec) const;
        Vector3i  operator-(const Vector3i& vec) const;
        Vector3i  operator*(const Vector3i& vec) const;
        Vector3i  operator*(float scalar) const;
        Vector3i  operator/(const Vector3i& vec) const;
        Vector3i  operator/(float scalar) const;
        Vector3i  operator-() const;
    };
}

namespace std
{
    template <>
    struct hash<Silent::Math::Vector3i>
    {
        size_t operator()(const Silent::Math::Vector3i& vec) const
        {
            size_t seed = 0;
            seed       ^= ((hash<int>()(vec.x) + Silent::Math::GOLDEN_RATIO) + (seed << 6)) + (seed >> 2);
            seed       ^= ((hash<int>()(vec.y) + Silent::Math::GOLDEN_RATIO) + (seed << 6)) + (seed >> 2);
            seed       ^= ((hash<int>()(vec.z) + Silent::Math::GOLDEN_RATIO) + (seed << 6)) + (seed >> 2);
            return seed;
        }
    };
}
