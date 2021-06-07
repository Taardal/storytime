#include "Tag.h"
#include "Core.h"

std::string storytime::Tag(const char* filename, const char* functionName, uint32_t lineNumber)
{
    std::stringstream ss;
    ss << filename << ":" << functionName << ":" << lineNumber;
    return ss.str();
}