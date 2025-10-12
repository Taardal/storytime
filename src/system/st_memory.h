#pragma once

#ifdef ST_DEBUG
    #define ST_TRACK_MEMORY
    #define ST_NEW new(std::source_location::current())
    #define ST_NEW_PTR(pointer) new(pointer, std::source_location::current())
#else
    #define ST_NEW new
    #define ST_NEW_PTR(pointer) new(pointer)
#endif

#ifdef ST_TRACK_MEMORY
void* operator new(size_t size, const std::source_location& source_location);

void* operator new(size_t size, void* pointer, const std::source_location& source_location);

void* operator new[](size_t size, const std::source_location& source_location);

void* operator new[](size_t size, void* pointer, const std::source_location& source_location);

void operator delete(void* pointer) noexcept;

void operator delete[](void* pointer) noexcept;

namespace Storytime {
    struct MemoryAllocation {
        size_t byte_size = 0;
        std::source_location source_location;

        std::string to_string() const;
    };

    std::ostream& operator<<(std::ostream& os, const MemoryAllocation& allocation);

    void initialize_memory_tracking();

    void terminate_memory_tracking();

    void track_memory(void* pointer, const MemoryAllocation& allocation);

    void untrack_memory(void* pointer);
}
#endif
