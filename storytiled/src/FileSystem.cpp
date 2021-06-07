#include "FileSystem.h"
#include <fstream>

namespace storytiled
{
    std::string FileSystem::ReadFile(const char* path)
    {
        std::string result;
        std::ifstream inputStream(path, std::ios::in | std::ios::binary);
        if (inputStream)
        {
            inputStream.seekg(0, std::ios::end);
            size_t length = inputStream.tellg();
            if (length != -1)
            {
                result.resize(length);
                inputStream.seekg(0, std::ios::beg);
                inputStream.read(&result[0], length);
                inputStream.close();
            }
        }
        return result;
    }
}
