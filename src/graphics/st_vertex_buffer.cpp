#include "st_vertex_buffer.h"
#include "graphics/st_open_gl.h"

namespace Storytime {
    VertexBuffer::AttributeLayout::AttributeLayout(const std::initializer_list<VertexAttribute>& attributes)
        : attributes(attributes), stride(0)
    {
        for (VertexAttribute& attribute : this->attributes) {
            attribute.offset = stride;
            stride += attribute.size;
        }
    }

    std::vector<VertexAttribute>::const_iterator VertexBuffer::AttributeLayout::begin() const {
        return attributes.begin();
    }

    std::vector<VertexAttribute>::const_iterator VertexBuffer::AttributeLayout::end() const {
        return attributes.end();
    }

    VertexBuffer::VertexBuffer(size_t size) : id(0), attribute_layout{} {
        ST_GL_CALL(glGenBuffers(1, &id));
        ST_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
        ST_GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    VertexBuffer::~VertexBuffer() {
        ST_GL_CALL(glDeleteBuffers(1, &id));
    }

    const VertexBuffer::AttributeLayout& VertexBuffer::get_attribute_layout() const {
        return attribute_layout;
    }

    void VertexBuffer::set_attribute_layout(const AttributeLayout& attribute_layout) {
        this->attribute_layout = attribute_layout;
    }

    void VertexBuffer::set_vertices(const void* vertices, u32 size) const {
        ST_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
        u32 offset = 0;
        ST_GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices));
    }

    void VertexBuffer::bind() const {
        ST_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
    }

    void VertexBuffer::unbind() const {
        ST_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}
