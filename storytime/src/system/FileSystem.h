#pragma once

#include <string>

namespace storytime
{
    class FileSystem
    {
    public:
        FileSystem();

        virtual ~FileSystem();

        std::string ReadFile(const char* path) const;
    };
}


