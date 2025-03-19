#include "st_file_reader.h"

#include <fstream>

namespace Storytime {
    std::string FileReader::read(const char* path) {
        ST_LOG_TRACE("Reading file [{0}]", path);
        std::string result;
        std::ifstream input_stream(path, std::ios::in | std::ios::binary);
        if (!input_stream) {
            ST_LOG_ERROR("Could not open file [{0}]", path);
            return "";
        }
        input_stream.seekg(0, std::ios::end);
        size_t length = input_stream.tellg();
        if (length == -1) {
            ST_LOG_ERROR("Could not read from file [{0}]", path);
            return "";
        }
        result.resize(length);
        input_stream.seekg(0, std::ios::beg);
        input_stream.read(&result[0], length);
        input_stream.close();
        ST_LOG_TRACE("Read file [{}]", path);
        return result;
    }
}
