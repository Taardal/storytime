#include "system/Log.h"
#include "VertexArray.h"

namespace storytime
{
    VertexArray::VertexArray()
            : id(0), vertexBuffers{}, indexBuffer(nullptr)
    {
        ST_LOG_TRACE(ST_TAG, "Creating");
        glGenVertexArrays(1, &id);
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    VertexArray::~VertexArray()
    {
        ST_LOG_TRACE(ST_TAG, "Destroying");
        for (const Ref<VertexBuffer>& vertexBuffer : vertexBuffers)
        {
            vertexBuffer->Unbind();
        }
        indexBuffer->Unbind();
        glDeleteVertexArrays(1, &id);
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        glBindVertexArray(id);
        vertexBuffer->Bind();
        uint32_t index = 0;
        const auto& attributeLayout = vertexBuffer->GetAttributeLayout();
        for (const VertexAttribute& attribute : attributeLayout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                    index,
                    attribute.Length,
                    GetOpenGLType(attribute.GlslType),
                    attribute.Normalized ? GL_TRUE : GL_FALSE,
                    attributeLayout.stride,
                    (const void*) (uintptr_t) attribute.Offset
            );
            index++;
        }
        vertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(id);
        indexBuffer->Bind();
        this->indexBuffer = indexBuffer;
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(id);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    uint32_t VertexArray::GetOpenGLType(GLSLType glslType) const
    {
        switch (glslType)
        {
            case GLSLType::Bool:
                return GL_BOOL;
            case GLSLType::Int:
                return GL_INT;
            case GLSLType::Float:
                return GL_FLOAT;
            case GLSLType::Vec2:
                return GL_FLOAT;
            case GLSLType::Vec3:
                return GL_FLOAT;
            case GLSLType::Vec4:
                return GL_FLOAT;
            case GLSLType::Mat2:
                return GL_FLOAT;
            case GLSLType::Mat3:
                return GL_FLOAT;
            case GLSLType::Mat4:
                return GL_FLOAT;
            default:
                return 0;
        }
    }
}
