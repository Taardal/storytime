#pragma once

#define ST_TO_STRING(value) #value

#define ST_STRING(message)\
    std::stringstream ss;\
    ss << message;\
    std::string message_string = ss.str()

namespace Storytime {
    template<typename T>
    T as(void* pointer) {
        return reinterpret_cast<T>(pointer);
    }
}
