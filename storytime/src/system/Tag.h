#pragma once

#include <sstream>
#include <typeinfo>

#define ST_TAG ::Storytime::Tag(*this, __func__, __LINE__)
#define ST_TAG_TYPE(T) ::Storytime::Tag<T>(__func__, __LINE__)

namespace Storytime
{
    const char* DemangleTypeName(const char* typeName);

    std::string FormatTag(const char* typeName, const char* functionName, uint32_t lineNumber);

    template<class T>
    std::string Tag(const char* functionName, uint32_t lineNumber)
    {
        const char* typeName = DemangleTypeName(typeid(T).name());
        return FormatTag(typeName, functionName, lineNumber);
    }

    template<class T>
    std::string Tag(const T& t, const char* functionName, uint32_t lineNumber)
    {
        const char* typeName = DemangleTypeName(typeid(t).name());
        return FormatTag(typeName, functionName, lineNumber);
    }
}