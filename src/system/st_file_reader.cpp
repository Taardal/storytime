#include "st_file_reader.h"

#include <fstream>

namespace Storytime {
    std::string FileReader::read_string(const std::filesystem::path& path) {
        ST_LOG_TRACE("Reading file [{0}]", path.c_str());
        std::string result;
        std::ifstream input_stream(path, std::ios::in | std::ios::binary);
        if (!input_stream) {
            ST_LOG_ERROR("Could not open file [{0}]", path.c_str());
            return "";
        }
        input_stream.seekg(0, std::ios::end);
        size_t length = input_stream.tellg();
        if (length == -1) {
            ST_LOG_ERROR("Could not read from file [{0}]", path.c_str());
            return "";
        }
        result.resize(length);
        input_stream.seekg(0, std::ios::beg);
        input_stream.read(&result[0], length);
        input_stream.close();
        ST_LOG_TRACE("Read file [{}]", path.c_str());
        return result;
    }

    std::vector<char> FileReader::read_bytes(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) {
            ST_THROW("Could not find file [" << path << "]");
        }

        // ate: Start reading at the end of the file
        // binary: Read the file as binary file (avoid text transformations)
        std::ifstream file{path.c_str(), std::ios::ate | std::ios::binary};
        if (!file.is_open()) {
            ST_THROW("Could not open file with path [" << path << "]");
        }

        // Starting to read at the end of the file means we can use the read position to determine the size of the file.
        auto file_size = (u32) file.tellg();
        std::vector<char> buffer(file_size);

        // Seek back to the beginning of the file and read all bytes.
        file.seekg(0);
        file.read(buffer.data(), file_size);

        file.close();
        return buffer;
    }
}
