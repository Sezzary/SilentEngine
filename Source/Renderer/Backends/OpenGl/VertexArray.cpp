#include "Framework.h"
#include "Renderer/Backends/OpenGl/VertexArray.h"

#include "Renderer/Backends/OpenGl/VertexBuffer.h"

namespace Silent::Renderer
{
    void VertexArrayObject::Initialize()
    {
        glGenVertexArrays(1, &_vertexArrayId);
    }

    void VertexArrayObject::Bind()
    {
        glBindVertexArray(_vertexArrayId);
    }

    void VertexArrayObject::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArrayObject::Delete()
    {
        glDeleteVertexArrays(1, &_vertexArrayId);
    }
    
    void VertexArrayObject::LinkAttrib(VertexBufferObject& vbo, uint layoutId, uint componentCount, GLenum type, uint stride, void* offset)
    {
        vbo.Bind();
        glVertexAttribPointer(layoutId, componentCount, type, false, stride, offset);
        glEnableVertexAttribArray(layoutId);
        vbo.Unbind();
    }
}
