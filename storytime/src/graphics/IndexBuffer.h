#pragma once

namespace storytime
{
    class IndexBuffer
    {
    private:
        uint32_t id;
        uint32_t count;

    public:
        IndexBuffer(uint32_t* indices, uint32_t count);

        ~IndexBuffer();

        void Bind() const;

        void Unbind() const;
    };
}
