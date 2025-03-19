#pragma once

#include "st_vertex_buffer.h"
#include "st_index_buffer.h"

namespace Storytime {
    class VertexArray {
    private:
        u32 id = 0;
        std::vector<Shared<VertexBuffer>> vertex_buffers;
        Shared<IndexBuffer> index_buffer = nullptr;

    public:
        VertexArray();

        ~VertexArray();

        void add_vertex_buffer(const Shared<VertexBuffer>& vertex_buffer);

        void set_index_buffer(const Shared<IndexBuffer>& index_buffer);

        void bind() const;

        void unbind() const;

    private:
        static u32 get_open_gl_type(GLSLType glsl_type);
    };
}
