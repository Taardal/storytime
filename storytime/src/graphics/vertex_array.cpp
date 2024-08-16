#include "vertex_array.h"
#include "graphics/open_gl_log.h"

namespace Storytime {
    VertexArray::VertexArray() {
        ST_GL_CALL(glGenVertexArrays(1, &id));
    }

    VertexArray::~VertexArray() {
        for (const Shared<VertexBuffer>& vertex_buffer : vertex_buffers) {
            vertex_buffer->unbind();
        }
        index_buffer->unbind();
        ST_GL_CALL(glDeleteVertexArrays(1, &id));
    }

    void VertexArray::add_vertex_buffer(const Shared<VertexBuffer>& vertex_buffer) {
        ST_GL_CALL(glBindVertexArray(id));
        vertex_buffer->bind();
        u32 index = 0;
        const auto& attribute_layout = vertex_buffer->get_attribute_layout();
        for (const VertexAttribute& attribute : attribute_layout) {
            ST_GL_CALL(glEnableVertexAttribArray(index));
            ST_GL_CALL(glVertexAttribPointer(
                index,
                attribute.length,
                get_open_gl_type(attribute.glsl_type),
                attribute.normalized ? GL_TRUE : GL_FALSE,
                attribute_layout.stride,
                (const void*) (uintptr_t) attribute.offset
            ));
            index++;
        }
        vertex_buffers.push_back(vertex_buffer);
    }

    void VertexArray::set_index_buffer(const Shared<IndexBuffer>& index_buffer) {
        ST_GL_CALL(glBindVertexArray(id));
        index_buffer->bind();
        this->index_buffer = index_buffer;
    }

    void VertexArray::bind() const {
        ST_GL_CALL(glBindVertexArray(id));
    }

    void VertexArray::unbind() const {
        ST_GL_CALL(glBindVertexArray(0));
    }

    u32 VertexArray::get_open_gl_type(GLSLType glsl_type) {
        switch (glsl_type) {
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
