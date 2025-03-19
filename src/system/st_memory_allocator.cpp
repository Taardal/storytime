#include "st_memory_allocator.h"

// --------------------------------------------------------------------------------------------------------------
// StackAllocator
// --------------------------------------------------------------------------------------------------------------

namespace Storytime {
    StackAllocator::StackAllocator(const StackAllocatorConfig& config)
        : memory_block(static_cast<char*>(malloc(config.bytes))),
          memory_block_head(memory_block),
          memory_block_tail(memory_block + config.bytes) {
    }

    StackAllocator::~StackAllocator() {
        free(memory_block);
    }

    //
    // Problem:
    // We need to track the size of each allocation for use during deallocation.
    //
    // Solution:
    // We store the size of the allocation just before the allocated memory in the memory block.
    //
    // | ----------------------------------------------------------------------------- |
    // |                               Memory block                                    |
    // | ----------------------------------------------------------------------------- |
    // | [  size  ] [ allocation ] [  size  ] [ allocation ] [  size  ] [ allocation ] |
    // | ----------------------------------------------------------------------------- |
    //
    void* StackAllocator::allocate(size_t bytes) {
        if (memory_block_head + bytes > memory_block_tail) {
            ST_THROW("Could not allocate [" << bytes << "] bytes: Not enough memory");
        }

        // Store the allocation size just before the allocated memory.
        auto size_pointer = reinterpret_cast<size_t*>(memory_block_head);
        *size_pointer = bytes;

        // The location of the allocated memory is right after the allocation size in the memory block
        void* pointer = memory_block_head + sizeof(size_t);

        // Move the head forward to right after the allocated memory for the next allocation
        memory_block_head += bytes + sizeof(size_t);

        ST_LOG_TRACE("Allocated memory: {}, {} bytes", pointer, bytes);
        return pointer;
    }

    void StackAllocator::deallocate(void* pointer) {
        // Retrieve the allocation size stored just before the allocated memory.
        // Subtract 1 from the pointer to the allocated memory to move it back by sizeof(size_t) bytes.
        size_t bytes = *(static_cast<size_t*>(pointer) - 1);

        if (pointer != memory_block_head - bytes) {
            ST_THROW(
                "Could not delete pointer [" << pointer << "] of [" << bytes << "] bytes: " <<
                "Deallocation is out of sequence"
            );
        }

        // Move the head backwards to just before the allocation size to "clear" the memory for the next allocation
        memory_block_head -= sizeof(size_t) + bytes;

        ST_LOG_TRACE("Deleted memory: {}, {} bytes", pointer, bytes);
    }
}

// --------------------------------------------------------------------------------------------------------------
// PoolAllocator
// --------------------------------------------------------------------------------------------------------------

namespace Storytime {
    PoolAllocator::PoolAllocator(const PoolAllocatorConfig& config)
        : config(config),
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

        // The allocated memory is the chunk at the current head in the memory block
        Chunk* chunk = memory_block_head;

        // Move the head forward to the next chunk. When there are no chunks left the next chunk will be `nullptr`.
        memory_block_head = memory_block_head->next;

        ST_LOG_TRACE("Allocated memory: {}, {} bytes", static_cast<void*>(chunk), bytes);
        return chunk;
    }

    void PoolAllocator::deallocate(void* pointer) {
        // The deallocated chunk will be first in line to be used for the next allocation.
        auto chunk = static_cast<Chunk*>(pointer);

        // Move the current chunk one place down the line after the deallocated chunk
        chunk->next = memory_block_head;

        // Move the head to the deallocated chunk to make it first in line for the next allocation
        memory_block_head = chunk;

        ST_LOG_TRACE("Deleted memory: {}, {} bytes", pointer, config.chunk_bytes);
    }

    Chunk* PoolAllocator::allocate_memory_block() const {
        // Allocate the memory block
        size_t block_size = config.chunk_count * config.chunk_bytes;
        auto memory_block_head = static_cast<Chunk*>(malloc(block_size));

        // Chain all the chunks in the block
        Chunk* chunk = memory_block_head;
        for (int i = 0; i < config.chunk_count - 1; ++i) {
            chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + config.chunk_bytes);
            chunk = chunk->next;
        }
        chunk->next = nullptr;

        // Return the head (first chunk) of the memory block
        return memory_block_head;
    }
}
