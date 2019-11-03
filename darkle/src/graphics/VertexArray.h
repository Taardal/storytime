#pragma once

#include "Buffer.h"
#include <glad/glad.h>

namespace Darkle {

    class VertexArray {
    private:
        unsigned int id;
        std::vector<VertexBuffer*> vertexBuffers;
        IndexBuffer* indexBuffer;

    public:
        VertexArray();

        ~VertexArray();

        [[nodiscard]] std::vector<VertexBuffer*> getVertexBuffers() const;

        void addVertexBuffer(VertexBuffer* vertexBuffer);

        [[nodiscard]] IndexBuffer* getIndexBuffer() const;

        void setIndexBuffer(IndexBuffer* indexBuffer);

        void bind() const;

        void unbind() const;

    private:
        [[nodiscard]] GLenum getOpenGLType(GLSLType glslType) const;
    };

}


