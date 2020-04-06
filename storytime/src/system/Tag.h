#pragma once

#include <string>
#include <typeinfo>

namespace storytime
{
    const char* Demangle(const char* typeName);

    template<class T>
    const char* Tag()
    {
        return Demangle(typeid(T).name());
    }

    template<class T>
    const char* Tag(const T& t)
    {
        return Demangle(typeid(t).name());
    }
}