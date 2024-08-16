#pragma once

namespace Storytime {
    class IndexBuffer {
    private:
        u32 id;
        u32 count;

    public:
        IndexBuffer(const u32* indices, u32 count);

        ~IndexBuffer();

        void bind() const;

        void unbind() const;
    };
}
