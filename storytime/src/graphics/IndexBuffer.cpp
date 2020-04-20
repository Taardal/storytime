#include "system/Log.h"
#include "IndexBuffer.h"
#include <glad/glad.h>

namespace storytime
{
    IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
            : id(0), count(count)
    {
        ST_GL_CALL(ST_TAG, glGenBuffers(1, &id));
        ST_GL_CALL(ST_TAG, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
        ST_GL_CALL(ST_TAG, glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer()
    {
        ST_GL_CALL(ST_TAG, glDeleteBuffers(1, &id));
    }

    void IndexBuffer::Bind() const
    {
        ST_GL_CALL(ST_TAG, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    }

    void IndexBuffer::Unbind() const
    {
        ST_GL_CALL(ST_TAG, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

}


