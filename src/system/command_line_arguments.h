#pragma once

namespace Storytime {
    struct CommandLineArguments {
        i32 count = 0;
        char** args = nullptr;

        CommandLineArguments() = default;

        CommandLineArguments(int count, char** args) : count(count), args(args) {
        }

        const char* operator[](int index) const {
            ST_ASSERT(count > 0, "No arguments: Count [" << count << "] must be greater than 0 (zero)");
            ST_ASSERT(index < count, "Not enough arguments: Index [" << index << "] must be less than count [" << count << "]");
            return args[index];
        }
    };
}