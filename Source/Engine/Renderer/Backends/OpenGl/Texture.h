#pragma once

namespace Silent::Renderer
{
    class ShaderProgram;

    class Texture
    {
    private:
        // Fields

        uint   _textureId   = 0;
        GLenum _textureUnit = 0;

    public:
        // Constructors

        Texture() = default;

        // Utilities

        void Initialize(const std::filesystem::path& filename, GLenum texUnitId, GLenum format, GLenum pixelType);
        void Initialize(const Vector2i& res, GLenum format, GLenum pixelType);
        void Bind();
        void Unbind();
        void Delete();

        void SetUnit(ShaderProgram& shaderProg, const std::string& uniName, GLenum texUnitId);
        void Resize(const Vector2& res, GLenum format, GLenum pixelType);
        void RefreshFilter();

    private:
        // Helpers

        void SetNearestFilter();
        void SetBilinearFilter();
    };
}
