#pragma once

namespace Storytime {
    class FileReader {
    public:
        static std::string read_string(const std::filesystem::path& path);

        static std::vector<char> read_bytes(const std::filesystem::path& path);
    };
}
