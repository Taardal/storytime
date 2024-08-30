#include "module.h"
#include "memory_allocator.h"

namespace Storytime {
    StackAllocator module_allocator({
        .bytes = 1024
    });

    void* Module::operator new(size_t size, const char* file_name, int line_number) {
        void* pointer = module_allocator.allocate(size);
#ifdef ST_TRACK_MEMORY
        MemoryAllocation allocation{
            .byte_size = size,
            .file_name = file_name,
            .line_number = line_number,
        };
        MemoryTracker::track(pointer, allocation);
#endif
        return pointer;
    }

    void Module::operator delete(void* pointer) noexcept {
#ifdef ST_TRACK_MEMORY
        MemoryTracker::untrack(pointer);
#endif
        module_allocator.deallocate(pointer);
    }
}
