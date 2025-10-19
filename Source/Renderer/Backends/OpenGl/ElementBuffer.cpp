#include "Framework.h"
#include "Renderer/Backends/OpenGl/ElementBuffer.h"

namespace Silent::Renderer
{
    void ElementBufferObject::Initialize(const std::span<uint>& idxs, GLenum usage)
    {
        glGenBuffers(1, &_bufferId);
        Bind();

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxs.size_bytes(), idxs.data(), usage);
    }

    void ElementBufferObject::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
    }

    void ElementBufferObject::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void ElementBufferObject::Delete()
    {
        glDeleteBuffers(1, &_bufferId);
    }
}
