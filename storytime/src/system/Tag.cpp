#include "Tag.h"
#include "Core.h"

#if defined(ST_COMPILER_CLANG) || defined(ST_COMPILER_GCC)

#include <cxxabi.h>

const char* storytime::Demangle(const char* typeName)
{
    static constexpr int INITIAL_STATUS = 1;
    static constexpr int SUCCESS_STATUS = 0;

    int status = INITIAL_STATUS;
    char* outputBuffer = nullptr;
    size_t* length = nullptr;
    char* demangled = abi::__cxa_demangle(typeName, outputBuffer, length, &status);

    return status == SUCCESS_STATUS ? demangled : typeName;
}

#else

const char* storytime::Demangle(const char* typeName) {
    return typeName;
}

#endif