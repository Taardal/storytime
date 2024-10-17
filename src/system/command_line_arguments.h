#pragma once

namespace Storytime {
    struct CommandLineArguments {
        i32 count = 0;
        char** args = nullptr;

        CommandLineArguments() = default;

        CommandLineArguments(int count, char** args) : count(count), args(args) {
        }

        const char* operator[](int index) const {
            ST_ASSERT(count > 0);
            ST_ASSERT(index < count);
            return args[index];
        }
    };
}