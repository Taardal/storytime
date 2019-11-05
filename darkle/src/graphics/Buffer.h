#pragma once

#include "VertexAttribute.h"
#include <vector>

namespace Darkle {

    class VertexBuffer {
    public:
        struct AttributeLayout {
            std::vector<VertexAttribute> attributes;
            unsigned int stride;

            AttributeLayout(const std::initializer_list<VertexAttribute>& attributes);
        };

    private:
        unsigned int id;
        AttributeLayout attributeLayout;

    public:
        VertexBuffer(float* vertices, unsigned int size);

        ~VertexBuffer();

        [[nodiscard]] const AttributeLayout& getAttributeLayout() const;

        void setAttributeLayout(const AttributeLayout& layout);

        void bind() const;

        void unbind() const;

    };

    class IndexBuffer {
    private:
        unsigned int id;
        unsigned int count;

    public:
        IndexBuffer(unsigned int* indices, unsigned int count);

        ~IndexBuffer();

        [[nodiscard]] unsigned int getCount() const;

        void bind() const;

        void unbind() const;
    };

}


