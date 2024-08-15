#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "system/environment.h"
#include <glad/glad.h>

namespace Storytime
{
    class VertexArray
    {
    private:
        uint32_t id;
        std::vector<Shared<VertexBuffer>> vertexBuffers;
        Shared<IndexBuffer> indexBuffer;

    public:
        VertexArray();

        ~VertexArray();

        void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer);

        void SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer);

        void Bind() const;

        void Unbind() const;

    private:
        [[nodiscard]] uint32_t GetOpenGLType(GLSLType glslType) const;
    };

}


