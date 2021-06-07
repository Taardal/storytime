#pragma once

#include <sstream>
#include <typeinfo>

#ifdef ST_PLATFORM_MACOS
    #define ST_FILENAME __FILE_NAME__
#else
    #define ST_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define ST_FUNCTION_NAME __func__
#define ST_LINE_NUMBER __LINE__

#define ST_TAG ::storytime::Tag(ST_FILENAME, ST_FUNCTION_NAME, ST_LINE_NUMBER)

namespace storytime
{
    std::string Tag(const char* filename, const char* functionName, uint32_t lineNumber);
}