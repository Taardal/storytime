#pragma once

#include <string>

namespace Storytime
{
    class FileSystem
    {
    public:
        FileSystem();

        virtual ~FileSystem();

        std::string ReadFile(const char* path) const;
    };
}


