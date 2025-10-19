#pragma once

#include "Renderer/Common/Objects/Primitive2d.h"
#include "Renderer/Backends/OpenGl/ElementBuffer.h"
#include "Renderer/Backends/OpenGl/Framebuffer.h"
#include "Renderer/Backends/OpenGl/ShaderProgram.h"
#include "Renderer/Backends/OpenGl/Texture.h"
#include "Renderer/Backends/OpenGl/VertexArray.h"
#include "Renderer/Backends/OpenGl/VertexBuffer.h"
#include "Renderer/Renderer.h"

namespace Silent::Renderer
{
    constexpr char OPEN_GL_VERSION[] = "#version 460";

    class OpenGlRenderer : public RendererBase
    {
    private:
        // Constants

        static constexpr Color DEFAULT_COLOR = Color(0.2f, 0.2f, 0.2f, 1.0f);

        // Fields

        SDL_GLContext                                  _context              = nullptr;
        std::unordered_map<std::string, ShaderProgram> _shaderPrograms       = {};

        FramebufferObject                              _2dframebuffer        = FramebufferObject();
        FramebufferObject                              _3dframebuffer        = FramebufferObject();

        VertexArrayObject                              _vertexArray          = VertexArrayObject();
        VertexArrayObject                              _vertexCubeArray      = VertexArrayObject();
        VertexBufferObject                             _vertexPositionBuffer = VertexBufferObject();
        VertexBufferObject                             _vertexColorBuffer    = VertexBufferObject();
        VertexBufferObject                             _vertexTexCoordBuffer = VertexBufferObject();
        VertexBufferObject                             _vertexCubeBuffer     = VertexBufferObject();
        ElementBufferObject                            _elementBuffer        = ElementBufferObject();

        Texture                                        _texture0             = Texture();
        Texture                                        _texture1             = Texture();
        std::unordered_map<std::string, Texture> _textures     = {}; // Key = texture name, value = texture.

        VertexArrayObject   _fsqVao     = VertexArrayObject();
        VertexBufferObject  _fsqVbo     = VertexBufferObject();
        ElementBufferObject _fsqEbo     = ElementBufferObject();
        Texture             _fsqTexture = Texture();

    public:
        // Constructors

        OpenGlRenderer() = default;

        // Utilities

        void Initialize(SDL_Window& window) override;
        void Deinitialize() override;
        void Update() override;
        void RefreshTextureFilter() override;
        void SaveScreenshot() const override;

    private:
        // Utilities
    
        void UpdateViewport();

        void Draw3dScene() override;
        void DrawFullscreenQuad();
        void Draw2dScene() override;
        void DrawDebugGui() override;

        void CreateShaderProgram();

        void LogError(const std::string& msg) const;
    };
}
