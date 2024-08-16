#include "index_buffer.h"
#include "graphics/open_gl_log.h"

namespace Storytime {
    IndexBuffer::IndexBuffer(const u32* indices, u32 count) : id(0), count(count) {
        ST_GL_CALL(glGenBuffers(1, &id));
        ST_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
        ST_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer() {
        ST_GL_CALL(glDeleteBuffers(1, &id));
    }

    void IndexBuffer::bind() const {
        ST_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    }

    void IndexBuffer::unbind() const {
        ST_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}
