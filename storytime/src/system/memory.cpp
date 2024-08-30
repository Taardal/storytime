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

void* operator new[](const size_t size, const char* file_name, const int line_number) {
    void* pointer = malloc(size);
    Storytime::MemoryAllocation allocation{
        .byte_size = size,
        .file_name = file_name,
        .line_number = line_number,
    };
    Storytime::MemoryTracker::track(pointer, allocation);
    return pointer;
}

void* operator new[](size_t size, const char* file_name, int line_number, void* pointer) {
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

void operator delete[](void* pointer) noexcept {
    Storytime::MemoryTracker::untrack(pointer);
    free(pointer);
}

namespace Storytime {
    std::string MemoryAllocation::to_string() const {
        std::stringstream ss;
        ss << byte_size << " bytes";
        bool has_file_name = file_name.size() > 0;
        bool has_line_number = line_number > 0;
        if (has_file_name || has_line_number) {
            ss << " ";
            ss << "(";
            if (has_file_name) {
                ss << file_name;
            }
            if (has_line_number) {
                if (has_file_name) {
                    ss << ":";
                }
                ss << line_number;
            }
            ss << ")";
        }
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
            std::cerr << "[Storytime] Memory leaks" << std::endl;
            std::cerr << "--------------------------------------------------------------------------------------------------------------" << std::endl;
            for (auto& [pointer, allocation] : *allocations) {
                std::string allocation_string = allocation.to_string();
                std::cerr << "- " << pointer << ", " << allocation_string << std::endl;
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
        std::string allocation_string = allocation.to_string();
        ST_LOG_TRACE("Allocated memory: {}, {}", pointer, allocation_string);
    }

    void MemoryTracker::untrack(void* pointer) {
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
#endif
