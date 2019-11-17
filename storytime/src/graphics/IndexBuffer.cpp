#include "system/Log.h"
#include "IndexBuffer.h"
#include <glad/glad.h>

namespace storytime {

    IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int count) : id(0), count(count) {
        ST_TRACE(ST_TAG, "Creating");
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        ST_TRACE(ST_TAG, "Created");
    }

    IndexBuffer::~IndexBuffer() {
        ST_TRACE(ST_TAG, "Destroying");
        glDeleteBuffers(1, &id);
        ST_TRACE(ST_TAG, "Destroyed");
    }

    unsigned int IndexBuffer::getCount() const {
        return count;
    }

    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}


