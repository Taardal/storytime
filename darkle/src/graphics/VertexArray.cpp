#include "Log.h"
#include "VertexArray.h"

namespace Darkle {

    VertexArray::VertexArray() : id(0), vertexBuffers{}, indexBuffer(nullptr) {
        LOG_TRACE(TAG, "Creating");
        glGenVertexArrays(1, &id);
        LOG_TRACE(TAG, "Created");
    }

    VertexArray::~VertexArray() {
        LOG_TRACE(TAG, "Destroying");
        for (const Ref<VertexBuffer>& vertexBuffer : vertexBuffers) {
            vertexBuffer->unbind();
        }
        indexBuffer->unbind();
        glDeleteVertexArrays(1, &id);
        LOG_TRACE(TAG, "Destroyed");
    }

    std::vector<Ref<VertexBuffer>> VertexArray::getVertexBuffers() const {
        return vertexBuffers;
    }

    void VertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
        glBindVertexArray(id);
        vertexBuffer->bind();
        unsigned int index = 0;
        const auto& attributeLayout = vertexBuffer->getAttributeLayout();
        for (const VertexAttribute& attribute : attributeLayout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                    index,
                    attribute.length,
                    getOpenGLType(attribute.glslType),
                    attribute.normalized ? GL_TRUE : GL_FALSE,
                    attributeLayout.stride,
                    (const void*) attribute.offset
            );
            index++;
        }
        vertexBuffers.push_back(vertexBuffer);
    }

    Ref<IndexBuffer> VertexArray::getIndexBuffer() const {
        return indexBuffer;
    }

    void VertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
        glBindVertexArray(id);
        indexBuffer->bind();
        this->indexBuffer = indexBuffer;
    }

    void VertexArray::bind() const {
        glBindVertexArray(id);
        LOG_DEBUG(TAG, "Bound vertex array [{0}]", id);
    }

    void VertexArray::unbind() const {
        glBindVertexArray(0);
        LOG_DEBUG(TAG, "Unbound vertex array [{0}]", id);
    }

    GLenum VertexArray::getOpenGLType(GLSLType glslType) const {
        switch (glslType) {
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
