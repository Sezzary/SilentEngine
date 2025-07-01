#pragma once

namespace Silent::Math
{
    class Vector4 : public glm::vec4
    {
    public:
        // Constants

        static constexpr uint AXIS_COUNT = 4;

        // Presets

        static const Vector4 Zero;
        static const Vector4 One;
        static const Vector4 UnitX;
        static const Vector4 UnitY;
        static const Vector4 UnitZ;
        static const Vector4 UnitW;

        // Constructors

        constexpr Vector4()                                   = default;
        constexpr Vector4(const glm::vec4& vec)               : glm::vec4(vec) {}
        constexpr Vector4(float x)                            : glm::vec4(x) {}
        constexpr Vector4(float x, float y, float z, float w) : glm::vec4(w, x, y, z) {}

        const glm::vec4& ToGlmVec4() const;
        glm::vec4&       ToGlmVec4();
    };
}
