#include "Tag.h"

#ifdef __GNUG__

#include <cxxabi.h>

const char* storytime::Demangle(const char* typeName)
{
    static constexpr int INITIAL_STATUS = 1;
    static constexpr int SUCCESS_STATUS = 0;

    int status = INITIAL_STATUS;
    char* outputBuffer = nullptr;
    size_t* length = nullptr;

    char* demangled = abi::__cxa_demangle(typeName, outputBuffer, length, &status);
    const char* string = status == SUCCESS_STATUS ? demangled : typeName;
    std::free(demangled);

    return string;
}

#else

const char* storytime::Demangle(const char* typeName) {
    return typeName;
}

#endif