#include "Tag.h"
#include "Core.h"

#if defined(ST_COMPILER_CLANG) || defined(ST_COMPILER_GCC)

#include <cxxabi.h>

const char* storytime::DemangleTypeName(const char* typeName)
{
    static constexpr int32_t INITIAL_STATUS = 1;
    static constexpr int32_t SUCCESS_STATUS = 0;
    int32_t status = INITIAL_STATUS;
    char* outputBuffer = nullptr;
    size_t* length = nullptr;
    char* demangled = abi::__cxa_demangle(typeName, outputBuffer, length, &status);
    return status == SUCCESS_STATUS ? demangled : typeName;
}

#else

const char* storytime::DemangleTypeName(const char* typeName)
{
    return typeName;
}

#endif

std::string storytime::FormatTag(const char* typeName, const char* functionName, uint32_t lineNumber)
{
    std::stringstream ss;
    ss << typeName << ":" << functionName << ":" << lineNumber;
    return ss.str();
}