#pragma once

#include "VertexAttribute.h"
#include <vector>

namespace Darkle {

    class VertexBuffer {
    public:
        struct Layout {
            std::vector<VertexAttribute> attributes;
            unsigned int stride;

            Layout(const std::initializer_list<VertexAttribute>& attributes);
        };

    private:
        unsigned int id;
        Layout layout;

    public:
        VertexBuffer(float* vertices, unsigned int size, Layout layout);
        ~VertexBuffer();

        void bind() const;

        void unbind() const;

        [[nodiscard]] const Layout& getLayout() const;

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


