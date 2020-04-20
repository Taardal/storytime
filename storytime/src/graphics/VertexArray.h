#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "system/Core.h"
#include <glad/glad.h>

namespace storytime
{
    class VertexArray
    {
    private:
        uint32_t id;
        std::vector<Ref<VertexBuffer>> vertexBuffers;
        Ref<IndexBuffer> indexBuffer;

    public:
        VertexArray();

        ~VertexArray();

        void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);

        void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

        void Bind() const;

        void Unbind() const;

    private:
        [[nodiscard]] uint32_t GetOpenGLType(GLSLType glslType) const;
    };

}


