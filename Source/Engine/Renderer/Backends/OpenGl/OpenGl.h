#pragma once

#include "Engine/Renderer/Common/Objects/Primitive2d.h"
#include "Engine/Renderer/Backends/OpenGl/ElementBuffer.h"
#include "Engine/Renderer/Backends/OpenGl/Framebuffer.h"
#include "Engine/Renderer/Backends/OpenGl/ShaderProgram.h"
#include "Engine/Renderer/Backends/OpenGl/Texture.h"
#include "Engine/Renderer/Backends/OpenGl/VertexArray.h"
#include "Engine/Renderer/Backends/OpenGl/VertexBuffer.h"
#include "Engine/Renderer/Backends/OpenGl/View.h"
#include "Engine/Renderer/Renderer.h"

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

        FramebufferObject                              _2dframebuffer        = FramebufferObject();
        FramebufferObject                              _3dframebuffer        = FramebufferObject();

        VertexArrayObject                              _vertexArray          = VertexArrayObject();
        VertexArrayObject                              _vertexCubeArray      = VertexArrayObject();
        VertexBufferObject                             _vertexPositionBuffer = VertexBufferObject();
        VertexBufferObject                             _vertexColorBuffer    = VertexBufferObject();
        VertexBufferObject                             _vertexTexCoordBuffer = VertexBufferObject();
        VertexBufferObject                             _vertexCubeBuffer     = VertexBufferObject();
        ElementBufferObject                            _elementBuffer        = ElementBufferObject();

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
        void LogError(const std::string& msg) const override;

        void SubmitPrimitive2d(const Primitive2d& prim) override;
        void SubmitScreenSprite(int assetIdx, const Vector2& pos, short rot, const Vector2& scale, const Color& color,
                                int depth, ScreenSpriteAlignMode alignMode, ScreenSpriteScaleMode scaleMode, BlendMode blendMode) override;
    private:
        // Utilities
    
        void UpdateViewport();

        void Draw2dScene();
        void Draw3dScene();
        void DrawDebugObjects();
        void DrawDebugGui() override;

        void CreateShaderProgram();
        void CreateDebugGui() override;
    };
}
