#pragma once

#define ST_TO_STRING(value) #value

namespace Storytime {
    template<typename T>
    T as(void* pointer) {
        return reinterpret_cast<T>(pointer);
    }
}
