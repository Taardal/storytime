#pragma once

#include "st_vertex_attribute.h"

namespace Storytime {
    class VertexBuffer {
    public:
        struct AttributeLayout {
            std::vector<VertexAttribute> attributes;
            u32 stride;

            AttributeLayout(const std::initializer_list<VertexAttribute>& attributes);

            std::vector<VertexAttribute>::const_iterator begin() const;

            std::vector<VertexAttribute>::const_iterator end() const;
        };

    private:
        u32 id;
        AttributeLayout attribute_layout;

    public:
        explicit VertexBuffer(size_t size);

        ~VertexBuffer();

        const AttributeLayout& get_attribute_layout() const;

        void set_attribute_layout(const AttributeLayout& attribute_layout);

        void set_vertices(const void* vertices, u32 size) const;

        void bind() const;

        void unbind() const;
    };
}
