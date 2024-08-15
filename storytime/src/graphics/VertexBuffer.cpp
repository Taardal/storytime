#include "VertexBuffer.h"
#include "system/log.h"
#include "graphics/GraphicsLog.h"

#include <glad/glad.h>

namespace Storytime
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

    VertexBuffer::VertexBuffer(size_t size)
            : id(0), attributeLayout{}
    {
        ST_GL_CALL(glGenBuffers(1, &id));
        ST_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
        ST_GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    VertexBuffer::~VertexBuffer()
    {
        ST_GL_CALL(glDeleteBuffers(1, &id));
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
        ST_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
        uint32_t offset = 0;
        ST_GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices));
    }

    void VertexBuffer::Bind() const
    {
        ST_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
    }

    void VertexBuffer::Unbind() const
    {
        ST_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}