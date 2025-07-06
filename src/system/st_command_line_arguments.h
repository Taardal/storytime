#pragma once

namespace Storytime {
    struct CommandLineArguments {
        i32 count = 0;
        char** args = nullptr;

        CommandLineArguments() = default;

        CommandLineArguments(int count, char** args) : count(count), args(args) {
        }

        const char* get(int index) const;

        std::filesystem::path exe() const;

        const char* operator[](int index) const;
    };

    std::ostream& operator<<(std::ostream& os, const CommandLineArguments& arguments);
}
