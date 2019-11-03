#include "Buffer.h"
#include "Log.h"
#include <glad/glad.h>

namespace Darkle {

    VertexBuffer::Layout::Layout(const std::initializer_list<VertexAttribute>& attributes) : attributes(attributes), stride(0) {
        for (VertexAttribute& attribute : this->attributes) {
            attribute.offset = stride;
            stride += attribute.size;
        }
    }

    VertexBuffer::VertexBuffer(float* vertices, unsigned int size, Layout  layout) : id(0), layout(std::move(layout)) {
        LOG_TRACE(TAG, "Creating");
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        LOG_TRACE(TAG, "Created");
    }

    VertexBuffer::~VertexBuffer() {
        LOG_TRACE(TAG, "Destroying");
        glDeleteBuffers(1, &id);
        LOG_TRACE(TAG, "Destroyed");
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    const VertexBuffer::Layout& VertexBuffer::getLayout() const {
        return layout;
    }

    IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int count) : id(0), count(count) {
        LOG_TRACE(TAG, "Creating");
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        LOG_TRACE(TAG, "Created");
    }

    IndexBuffer::~IndexBuffer() {
        LOG_TRACE(TAG, "Destroying");
        glDeleteBuffers(1, &id);
        LOG_TRACE(TAG, "Destroyed");
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