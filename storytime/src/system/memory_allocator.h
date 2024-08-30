#pragma once

// --------------------------------------------------------------------------------------------------------------
// MemoryAllocator
// --------------------------------------------------------------------------------------------------------------

namespace Storytime {
    class MemoryAllocator {
    public:
        virtual ~MemoryAllocator() = default;

        virtual void* allocate(size_t bytes) = 0;

        virtual void deallocate(void* pointer) = 0;
    };
}

// --------------------------------------------------------------------------------------------------------------
// StackAllocator
// --------------------------------------------------------------------------------------------------------------

namespace Storytime {
    struct StackAllocatorConfig {
        size_t bytes = 0;
    };

    class StackAllocator : public MemoryAllocator {
    private:
        char* memory_block;
        char* memory_block_head;
        char* memory_block_tail;

    public:
        explicit StackAllocator(const StackAllocatorConfig& config);

        ~StackAllocator() override;

        void* allocate(size_t bytes) override;

        void deallocate(void* pointer) override;
    };
}

// --------------------------------------------------------------------------------------------------------------
// PoolAllocator
// --------------------------------------------------------------------------------------------------------------

namespace Storytime {
    struct Chunk {
        Chunk* next = nullptr;
    };

    struct PoolAllocatorConfig {
        size_t chunk_bytes = 0;
        size_t chunk_count = 0;
        bool automatic_resize_enabled = false;
    };

    class PoolAllocator : public MemoryAllocator {
    private:
        PoolAllocatorConfig config;
        std::vector<Chunk*> memory_blocks;
        Chunk* memory_block_head;

    public:
        explicit PoolAllocator(PoolAllocatorConfig config);

        ~PoolAllocator() override;

        void* allocate(size_t bytes) override;

        void deallocate(void* pointer) override;

    private:
        Chunk* allocate_memory_block() const;
    };
}
