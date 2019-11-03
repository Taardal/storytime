#include "VertexArray.h"
#include "Log.h"

namespace Darkle {

    VertexArray::VertexArray() : id(0), vertexBuffers{}, indexBuffer(nullptr) {
        LOG_TRACE(TAG, "Creating");
        glGenVertexArrays(1, &id);
        LOG_TRACE(TAG, "Created");
    }

    VertexArray::~VertexArray() {
        LOG_TRACE(TAG, "Destroying");
        for (VertexBuffer* vertexBuffer : vertexBuffers) {
            vertexBuffer->unbind();
            delete vertexBuffer;
        }
        indexBuffer->unbind();
        delete indexBuffer;
        glDeleteVertexArrays(1, &id);
        LOG_TRACE(TAG, "Destroyed");
    }

    std::vector<VertexBuffer*> VertexArray::getVertexBuffers() const {
        return vertexBuffers;
    }

    void VertexArray::addVertexBuffer(VertexBuffer* vertexBuffer) {
        glBindVertexArray(id);
        vertexBuffer->bind();
        unsigned int index = 0;
        const auto& layout = vertexBuffer->getLayout();
        for (const VertexAttribute& attribute : layout.attributes) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                    index,
                    attribute.length,
                    getOpenGLType(attribute.glslType),
                    attribute.normalized ? GL_TRUE : GL_FALSE,
                    layout.stride,
                    (const void*) attribute.offset
            );
            /*
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
            */
            index++;
        }
        vertexBuffers.push_back(vertexBuffer);
    }

    IndexBuffer* VertexArray::getIndexBuffer() const {
        return indexBuffer;
    }

    void VertexArray::setIndexBuffer(IndexBuffer* indexBuffer) {
        glBindVertexArray(id);
        indexBuffer->bind();
        this->indexBuffer = indexBuffer;
    }

    void VertexArray::bind() const {
        glBindVertexArray(id);
    }

    void VertexArray::unbind() const {
        glBindVertexArray(0);
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
