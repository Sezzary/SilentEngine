#pragma once

#include "Engine/Renderer/Backends/OpenGl/ElementBuffer.h"
#include "Engine/Renderer/Backends/OpenGl/ShaderProgram.h"
#include "Engine/Renderer/Backends/OpenGl/Texture.h"
#include "Engine/Renderer/Backends/OpenGl/VertexArray.h"
#include "Engine/Renderer/Backends/OpenGl/VertexBuffer.h"
#include "Engine/Renderer/Backends/OpenGl/View.h"
#include "Engine/Renderer/Base.h"
#include "Engine/Renderer/Objects/Primitive2d.h"

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
        View                                           _view                 = View();
        std::unordered_map<std::string, ShaderProgram> _shaderPrograms       = {};

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

        void Draw2dScene();
        void Draw3dScene();
        void DrawGui();
        void DrawDebugGui();

        void DrawDebugObjects();

        void CreateShaderProgram();
        void CreateDebugGui();
    };
}
