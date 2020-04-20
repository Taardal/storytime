#include "VertexBuffer.h"
#include "system/Log.h"
#include <glad/glad.h>

namespace storytime
{
    VertexBuffer::AttributeLayout::AttributeLayout(const std::initializer_list<VertexAttribute>& attributes)
            : attributes(attributes), stride(0)
    {
        for (VertexAttribute& attribute : this->attributes)
        {
            attribute.Offset = stride;
            stride += attribute.Size;
        }
    }

    std::vector<VertexAttribute>::const_iterator VertexBuffer::AttributeLayout::begin() const
    {
        return attributes.begin();
    }

    std::vector<VertexAttribute>::const_iterator VertexBuffer::AttributeLayout::end() const
    {
        return attributes.end();
    }

    VertexBuffer::VertexBuffer(uint32_t size)
            : id(0), attributeLayout{}
    {
        ST_LOG_TRACE(ST_TAG, "Creating");
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    VertexBuffer::~VertexBuffer()
    {
        ST_LOG_TRACE(ST_TAG, "Destroying");
        glDeleteBuffers(1, &id);
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    const VertexBuffer::AttributeLayout& VertexBuffer::GetAttributeLayout() const
    {
        return attributeLayout;
    }

    void VertexBuffer::SetAttributeLayout(const AttributeLayout& attributeLayout)
    {
        this->attributeLayout = attributeLayout;
    }

    void VertexBuffer::SetVertices(const void* vertices, uint32_t size) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        uint32_t offset = 0;
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}