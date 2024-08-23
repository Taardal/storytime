#pragma once

#ifdef ST_DEBUG
    #define ST_TRACK_MEMORY
    #define ST_NEW new(__FILE_NAME__, __LINE__)
    #define ST_NEW_POINTER(pointer) new(__FILE_NAME__, __LINE__, pointer)
#else
    #define ST_NEW new
    #define ST_NEW_POINTER(pointer) new(pointer)
#endif

#ifdef ST_TRACK_MEMORY
void* operator new(size_t size, const char* file_name, int line_number);

void* operator new(size_t size, const char* file_name, int line_number, void* pointer);

void operator delete(void* pointer) noexcept;

namespace Storytime {
    struct MemoryAllocation {
        size_t byte_size;
        std::string file_name;
        int line_number;

        std::string to_string() const;
    };
}

namespace Storytime {
    typedef std::map<void*, MemoryAllocation> MemoryAllocationMap;

    class MemoryTracker {
    private:
        static MemoryAllocationMap* allocations;

    public:
        static void terminate();

        static void track(void* pointer, const MemoryAllocation& allocation);

        static void untrack(void* pointer);
    };
}
#endif
