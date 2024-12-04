#pragma once

namespace Storytime {
    struct CommandLineArguments {
        i32 count = 0;
        char** args = nullptr;

        CommandLineArguments() = default;

        CommandLineArguments(int count, char** args) : count(count), args(args) {
        }

        const char* operator[](int index) const;

        bool exists(const char* key) const;
    };
}