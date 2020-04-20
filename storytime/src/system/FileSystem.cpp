#include "Log.h"
#include "FileSystem.h"
#include <fstream>

namespace storytime
{
    FileSystem::FileSystem()
    {
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    FileSystem::~FileSystem()
    {
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    std::string FileSystem::ReadFile(const char* path) const
    {
        ST_LOG_TRACE(ST_TAG, "Reading file [{0}]", path);
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
            else
            {
                ST_LOG_ERROR(ST_TAG, "Could not read from file [{0}]", path);
            }
        }
        else
        {
            ST_LOG_ERROR(ST_TAG, "Could not open file [{0}]", path);
        }
        return result;
    }
}
