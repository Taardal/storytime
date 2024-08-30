#include "memory_allocator.h"

// --------------------------------------------------------------------------------------------------------------
// StackAllocator
// --------------------------------------------------------------------------------------------------------------

namespace Storytime {
    StackAllocator::StackAllocator(const StackAllocatorConfig& config)
        : memory_block((char*) malloc(config.bytes)),
          memory_block_head(memory_block),
          memory_block_tail(memory_block + config.bytes) {
        ST_ASSERT_THROW(config.bytes > 0);
    }

    StackAllocator::~StackAllocator() {
        free(memory_block);
    }

    void* StackAllocator::allocate(size_t bytes) {
        if (memory_block_head + bytes > memory_block_tail) {
            ST_THROW("Could not allocate [" << bytes << "] bytes: Not enough memory");
        }

        // Store the size just before the actual memory block
        *(size_t*) memory_block_head = bytes;

        // Calculate the pointer to return (just after the size metadata)
        void* pointer = memory_block_head + sizeof(size_t);

        // Move the memory_block_head pointer forward by the size of the allocation and the metadata
        memory_block_head += bytes + sizeof(size_t);

        ST_LOG_TRACE("Allocated memory: {} -> {} bytes", pointer, bytes);
        return pointer;
    }

    void StackAllocator::deallocate(void* pointer) {
        // Retrieve the size stored just before the pointer
        size_t size = *((size_t*) pointer - 1);

        // Only deallocate if this is the most recent allocation
        if (pointer != memory_block_head - size) {
            ST_THROW(
                "Could not delete pointer [" << pointer << "] of [" << size << "] bytes: " <<
                "Deallocation is out of sequence"
            );
        }
        ST_LOG_TRACE("Deleted memory: {} -> {} bytes", pointer, size);
        memory_block_head -= size + sizeof(size_t);
    }
}

// --------------------------------------------------------------------------------------------------------------
// PoolAllocator
// --------------------------------------------------------------------------------------------------------------

namespace Storytime {
    PoolAllocator::PoolAllocator(PoolAllocatorConfig config)
        : config(std::move(config)),
          memory_blocks{allocate_memory_block()},
          memory_block_head(memory_blocks[0]) {
    }

    PoolAllocator::~PoolAllocator() {
        for (auto memory_block : memory_blocks) {
            free(memory_block);
        }
    }

    void* PoolAllocator::allocate(size_t bytes) {
        if (bytes > config.chunk_bytes) {
            ST_THROW(
                "Could not allocate [" << bytes << "] bytes: " <<
                "Byte size is larger than the chunk size: [" << bytes << "] > [" << config.chunk_bytes << "]"
            );
        }

        bool memory_block_is_full = memory_block_head == nullptr;
        if (memory_block_is_full) {
            if (config.automatic_resize_enabled) {
                memory_block_head = allocate_memory_block();
                memory_blocks.push_back(memory_block_head);
            } else {
                ST_THROW("Could not allocate [" << bytes << "] bytes: Not enough memory");
            }
        }

        // The return value is the current position of the allocation pointer
        Chunk* chunk = memory_block_head;

        // Advance (bump) the allocation pointer to the next chunk. When no chunks left, the head will be set
        // to `nullptr`, and this will cause allocation of a new block on the next request.
        memory_block_head = memory_block_head->next;

        ST_LOG_TRACE("Allocated memory: {} -> {} bytes", as<void*>(chunk), bytes);
        return chunk;
    }

    void PoolAllocator::deallocate(void* pointer) {
        auto chunk = as<Chunk*>(pointer);

        // The freed chunk's next pointer points to the current allocation pointer
        chunk->next = memory_block_head;

        // And the allocation pointer is now set to the returned (free) chunk
        memory_block_head = chunk;

        ST_LOG_TRACE("Deleted memory: {} -> {} bytes", pointer, config.chunk_bytes);
    }

    Chunk* PoolAllocator::allocate_memory_block() const {
        // Allocate the memory block
        size_t block_size = config.chunk_count * config.chunk_bytes;
        auto memory_block_head = as<Chunk*>(malloc(block_size));

        // Chain all the chunks in the block
        Chunk* chunk = memory_block_head;
        for (int i = 0; i < config.chunk_count - 1; ++i) {
            auto next_chunk_head = as<char*>(chunk) + config.chunk_bytes;
            chunk->next = as<Chunk*>(next_chunk_head);
            chunk = chunk->next;
        }
        chunk->next = nullptr;

        // Return the first chunk (start) of the memory block
        return memory_block_head;
    }
}
