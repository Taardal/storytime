#include "Buffer.h"
#include "system/Log.h"
#include <glad/glad.h>

namespace storytime {

    VertexBuffer::AttributeLayout::AttributeLayout(const std::initializer_list<VertexAttribute>& attributes) : attributes(attributes), stride(0) {
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

    VertexBuffer::VertexBuffer(float* vertices, unsigned int size) : id(0), attributeLayout{} {
        ST_TRACE(ST_TAG, "Creating");
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        ST_TRACE(ST_TAG, "Created");
    }

    VertexBuffer::~VertexBuffer() {
        ST_TRACE(ST_TAG, "Destroying");
        glDeleteBuffers(1, &id);
        ST_TRACE(ST_TAG, "Destroyed");
    }

    const VertexBuffer::AttributeLayout& VertexBuffer::getAttributeLayout() const {
        return attributeLayout;
    }

    void VertexBuffer::setAttributeLayout(const AttributeLayout& attributeLayout) {
        this->attributeLayout = attributeLayout;
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        ST_DEBUG(ST_TAG, "Bound id [{0}]", id);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        ST_DEBUG(ST_TAG, "Unbound id [{0}]", id);
    }

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
        ST_DEBUG(ST_TAG, "Bound id [{0}]", id);
    }

    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        ST_DEBUG(ST_TAG, "Unbound id [{0}]", id);
    }

}