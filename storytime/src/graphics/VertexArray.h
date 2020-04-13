#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "system/Core.h"
#include <glad/glad.h>

namespace storytime {

    class VertexArray {
    private:
        unsigned int id;
        std::vector<Ref<VertexBuffer>> vertexBuffers;
        Ref<IndexBuffer> indexBuffer;

    public:
        VertexArray();

        ~VertexArray();

        void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);

        void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

        void bind() const;

        void unbind() const;

    private:
        [[nodiscard]] GLenum getOpenGLType(GLSLType glslType) const;
    };

}


