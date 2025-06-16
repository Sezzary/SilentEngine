#pragma once

#include "Engine/Renderer/Backends/OpenGl/Texture.h"
#include "Engine/Renderer/Backends/OpenGl/VertexArray.h"
#include "Engine/Renderer/Backends/OpenGl/VertexBuffer.h"

namespace Silent::Renderer
{
    class FrameBufferObject
    {
    private:
        // Fields

        uint               _id      = 0;
        VertexArrayObject  _vao     = VertexArrayObject();
        VertexBufferObject _vbo     = VertexBufferObject();
        Texture            _texture = Texture();

    public:
        // Constructors

        FrameBufferObject() = default;

        // Utilities

        void Initialize();
        void Update();
        void Bind();
        void Unbind();
        void Delete();
    };
}
