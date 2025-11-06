#include "memory.h"

#ifdef ST_TRACK_MEMORY
void* operator new(const size_t size, const std::source_location& source_location) {
    void* pointer = malloc(size);
    Storytime::MemoryAllocation allocation{
        .byte_size = size,
        .source_location = source_location,
    };
    Storytime::track_memory(pointer, allocation);
    return pointer;
}

void* operator new(size_t size, void* pointer, const std::source_location& source_location) {
    Storytime::MemoryAllocation allocation{
        .byte_size = size,
        .source_location = source_location,
    };
    Storytime::track_memory(pointer, allocation);
    return pointer;
}

void* operator new[](const size_t size, const std::source_location& source_location) {
    void* pointer = malloc(size);
    Storytime::MemoryAllocation allocation{
        .byte_size = size,
        .source_location = source_location,
    };
    Storytime::track_memory(pointer, allocation);
    return pointer;
}

void* operator new[](size_t size, void* pointer, const std::source_location& source_location) {
    Storytime::MemoryAllocation allocation{
        .byte_size = size,
        .source_location = source_location,
    };
    Storytime::track_memory(pointer, allocation);
    return pointer;
}

void operator delete(void* pointer) noexcept {
    Storytime::untrack_memory(pointer);
    free(pointer);
}

void operator delete[](void* pointer) noexcept {
    Storytime::untrack_memory(pointer);
    free(pointer);
}

namespace Storytime {
    std::map<void*, MemoryAllocation>* allocations = nullptr;

    void initialize_memory_tracking() {
        allocations = new std::map<void*, MemoryAllocation>();
    }

    void terminate_memory_tracking() {
        ST_ASSERT(allocations != nullptr, "Allocations map has already been terminated or was never initialized");
        if (!allocations->empty()) {
            std::cerr << "--------------------------------------------------------------------------------------------------------------" << std::endl;
            std::cerr << "[Storytime] Unfreed memory" << std::endl;
            std::cerr << "--------------------------------------------------------------------------------------------------------------" << std::endl;
            u32 i = 0;
            for (auto& [pointer, allocation] : *allocations) {
                std::string allocation_string = allocation.to_string();
                std::cerr << "- [" << i << "] " << pointer << ", " << allocation_string << std::endl;
                i++;
            }
            fprintf(stderr, "\n");
        }
        free(allocations);
        allocations = nullptr;
    }

    void track_memory(void* pointer, const MemoryAllocation& allocation) {
        if (allocations == nullptr) {
            return;
        }
        allocations->emplace(pointer, allocation);
        std::string allocation_string = allocation.to_string();
        ST_LOG_TRACE("Allocated memory: {}, {}", pointer, allocation_string);
    }

    void untrack_memory(void* pointer) {
        if (allocations == nullptr) {
            return;
        }
        auto it = allocations->find(pointer);
        if (it == allocations->end()) {
            return;
        }
        std::string allocation_string = it->second.to_string();
        allocations->erase(it);
        ST_LOG_TRACE("Deleted memory: {}, {}", pointer, allocation_string);
    }
}

namespace Storytime {
    std::string MemoryAllocation::to_string() const {
        std::stringstream ss;
        ss << byte_size << " bytes at " << ::Storytime::tag(source_location);
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const MemoryAllocation& allocation) {
        return os << allocation.to_string();
    }
}
#endif
