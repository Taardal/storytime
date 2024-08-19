#include "memory.h"

#ifdef ST_TRACK_MEMORY
void* operator new(const size_t size, const char* file_name, const int line_number) {
    void* pointer = malloc(size);
    Storytime::MemoryAllocation allocation{
        .byte_size = size,
        .file_name = file_name,
        .line_number = line_number,
    };
    Storytime::MemoryTracker::track(pointer, allocation);
    return pointer;
}

void* operator new(size_t size, const char* file_name, int line_number, void* pointer) {
    Storytime::MemoryAllocation allocation{
        .byte_size = size,
        .file_name = file_name,
        .line_number = line_number,
    };
    Storytime::MemoryTracker::track(pointer, allocation);
    return pointer;
}

void operator delete(void* pointer) noexcept {
    Storytime::MemoryTracker::untrack(pointer);
    free(pointer);
}

namespace Storytime {
    std::string MemoryAllocation::to_string() const {
        std::stringstream ss;
        ss << file_name << ":" << line_number << " -> " << byte_size << " bytes";
        return ss.str();
    }
}

namespace Storytime {
    MemoryAllocationMap* MemoryTracker::allocations = new MemoryAllocationMap();

    void MemoryTracker::terminate() {
        if (allocations == nullptr) {
            return;
        }
        if (!allocations->empty()) {
            std::cerr << "--------------------------------------------------------------------------------------------------------------" << std::endl;
            std::cerr << "[Storytime::MemoryTracker] Memory leaks" << std::endl;
            std::cerr << "--------------------------------------------------------------------------------------------------------------" << std::endl;
            for (auto& [pointer, allocation] : *allocations) {
                std::string allocation_string = allocation.to_string();
                std::cerr << "- " << allocation_string << std::endl;
            }
        }
        free(allocations);
        allocations = nullptr;
    }

    void MemoryTracker::track(void* pointer, const MemoryAllocation& allocation) {
        if (allocations == nullptr) {
            return;
        }
        allocations->emplace(pointer, allocation);
    }

    void MemoryTracker::untrack(void* pointer) {
        if (allocations == nullptr) {
            return;
        }
        auto it = allocations->find(pointer);
        if (it == allocations->end()) {
            return;
        }
        allocations->erase(it);
    }
}
#endif
