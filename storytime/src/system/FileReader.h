#pragma once

#include <string>

namespace storytime {

    class FileReader {
    private:
        const char* path;

    public:
        explicit FileReader(const char* path);

        [[nodiscard]] std::string read() const;
    };

}


