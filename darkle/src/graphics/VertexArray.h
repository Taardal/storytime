#pragma once

#include "Buffer.h"
#include "system/Core.h"
#include <glad/glad.h>

namespace Darkle {

    class VertexArray {
    private:
        unsigned int id;
        std::vector<Ref<VertexBuffer>> vertexBuffers;
        Ref<IndexBuffer> indexBuffer;

    public:
        VertexArray();

        ~VertexArray();

        [[nodiscard]] std::vector<Ref<VertexBuffer>> getVertexBuffers() const;

        void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);

        [[nodiscard]] Ref<IndexBuffer> getIndexBuffer() const;

        void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

        void bind() const;

        void unbind() const;

    private:
        [[nodiscard]] GLenum getOpenGLType(GLSLType glslType) const;
    };

}


