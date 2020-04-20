#include "system/Log.h"
#include "IndexBuffer.h"
#include <glad/glad.h>

namespace storytime
{
    IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
            : id(0), count(count)
    {
        ST_LOG_TRACE(ST_TAG, "Creating");
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    IndexBuffer::~IndexBuffer()
    {
        ST_LOG_TRACE(ST_TAG, "Destroying");
        glDeleteBuffers(1, &id);
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}


