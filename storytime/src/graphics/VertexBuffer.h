#pragma once

#include "VertexAttribute.h"
#include <vector>

namespace storytime
{
    class VertexBuffer
    {
    public:
        struct AttributeLayout
        {
            std::vector<VertexAttribute> attributes;
            uint32_t stride;

            AttributeLayout(const std::initializer_list<VertexAttribute>& attributes);

            [[nodiscard]] std::vector<VertexAttribute>::const_iterator begin() const;

            [[nodiscard]] std::vector<VertexAttribute>::const_iterator end() const;
        };

    private:
        uint32_t id;
        AttributeLayout attributeLayout;

    public:
        explicit VertexBuffer(uint32_t size);

        ~VertexBuffer();

        [[nodiscard]] const AttributeLayout& getAttributeLayout() const;

        void setAttributeLayout(const AttributeLayout& layout);

        void SetVertices(const void* vertices, uint32_t size) const;

        void bind() const;

        void unbind() const;

    };

}


