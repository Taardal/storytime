#pragma once

#define ST_DEREF(pointer)\
    ([&]() -> decltype(auto) {\
        ST_ASSERT(pointer != nullptr, "Pointer [" << #pointer << "] cannot be null");\
        return *pointer;\
    }())

namespace Storytime {
    template<typename T>
    class NotNull {
    private:
        T* pointer;
        std::source_location source_location;

    public:
        NotNull(T* pointer, std::source_location source_location = std::source_location::current())
            : pointer(pointer), source_location(source_location)
        {
            ST_ASSERT(this->pointer != nullptr, "Pointer cannot be null at " << ::Storytime::tag(this->source_location));
        }

        T* operator->() const {
            return pointer;
        }

        T& operator*() const {
            return *pointer;
        }
    };
}