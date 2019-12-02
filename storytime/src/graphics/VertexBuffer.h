#pragma once

#include "VertexAttribute.h"
#include <vector>

namespace storytime {

    class VertexBuffer {
    public:
        struct AttributeLayout {
            std::vector<VertexAttribute> attributes;
            unsigned int stride;

            AttributeLayout(const std::initializer_list<VertexAttribute>& attributes);

            [[nodiscard]] std::vector<VertexAttribute>::const_iterator begin() const;

            [[nodiscard]] std::vector<VertexAttribute>::const_iterator end() const;
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

}


