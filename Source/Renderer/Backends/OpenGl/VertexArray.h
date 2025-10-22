#pragma once

#include "Renderer/Backends/OpenGl/VertexBuffer.h"

namespace Silent::Renderer
{
    class VertexArrayObject
    {
    private:
        // Fields

        uint _vertexArrayId = 0;

    public:
        // Constructors

        VertexArrayObject() = default;

        // Utilities

        void Initialize();
        void Bind();
        void Unbind();
        void Delete();

        void LinkAttrib(VertexBufferObject& vbo, uint layoutId, uint componentCount, GLenum type, uint stride, void* offset);
    };
}
