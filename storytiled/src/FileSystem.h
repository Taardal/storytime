#pragma once

#include <string>

namespace storytiled
{
    class FileSystem
    {
    public:
        static std::string ReadFile(const char* path);
    };
}


