#include "Framework.h"
#include "Engine/Renderer/Backends/OpenGl/Framebuffer.h"

#include "Engine/Application.h"
#include "Engine/Renderer/Renderer.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Renderer
{
    static auto FRAMEBUFFER_QUAD_VERTICES = std::vector<float>
    {
        // Positions | Texture coords
        -1.0f,  1.0f,  0.0f, 1.0f, // Top-left.
         1.0f,  1.0f,  1.0f, 1.0f, // Top-right.
        -1.0f, -1.0f,  0.0f, 0.0f, // Bottom-left.
         1.0f, -1.0f,  1.0f, 0.0f  // Bottom-right.
    };

    void FramebufferObject::Initialize()
    {
        const auto& renderer = g_App.GetRenderer();

        // Generate and bind framebuffer.
        glGenFramebuffers(1, &_frameBufferId);
        Bind();

        // Generate texture.
        _texture.Initialize(renderer.GetScreenResolution(), GL_RGBA, GL_UNSIGNED_BYTE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("Failed to initialize framebuffer.");
        }

        // Generate and bind VAO.
        _vao.Initialize();
        _vao.Bind();

        // Generate VBO.
        _vbo.Initialize(ToSpan(FRAMEBUFFER_QUAD_VERTICES));
        _vao.LinkAttrib(_vbo, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
        _vao.LinkAttrib(_vbo, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // Unbind framebuffer, VAO, VBO, and texture.
        Unbind();
    }

    void FramebufferObject::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);
        _vao.Bind();
        _vbo.Bind();
        _texture.Bind();
    }

    void FramebufferObject::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        _vao.Unbind();
        _vbo.Unbind();
        _texture.Unbind();
    }

    void FramebufferObject::Delete()
    {
        glDeleteFramebuffers(1, &_frameBufferId);
        _vao.Delete();
        _vbo.Delete();
        _texture.Delete();
    }
}
