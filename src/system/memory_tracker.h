#pragma once

#ifdef ST_DEBUG
    #define ST_TRACK_MEMORY
    #define ST_NEW new(ST_FILE_NAME, ST_LINE_NUMBER)
    #define ST_NEW_POINTER(pointer) new(ST_FILE_NAME, ST_LINE_NUMBER, pointer)
#else
    #define ST_NEW new
    #define ST_NEW_POINTER(pointer) new(pointer)
#endif

#ifdef ST_TRACK_MEMORY
void* operator new(size_t size, const char* file_name, int line_number);

void* operator new(size_t size, const char* file_name, int line_number, void* pointer);

void* operator new[](size_t size, const char* file_name, int line_number);

void* operator new[](size_t size, const char* file_name, int line_number, void* pointer);

void operator delete(void* pointer) noexcept;

void operator delete[](void* pointer) noexcept;

namespace Storytime {
    struct MemoryAllocation {
        size_t byte_size = 0;
        std::string file_name = "";
        int line_number = 0;

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
