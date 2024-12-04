#include "command_line_arguments.h"

namespace Storytime {
    const char* CommandLineArguments::operator[](int index) const {
        ST_ASSERT(args != nullptr, "Arguments must exist");
        ST_ASSERT(count > 0, "No arguments: Count [" << count << "] must be greater than zero");
        ST_ASSERT(index < count, "Not enough arguments: Index [" << index << "] must be less than count [" << count << "]");
        return args[index];
    }

    bool CommandLineArguments::exists(const char* key) const {
        ST_ASSERT(args != nullptr, "Arguments must exist");
        ST_ASSERT(count > 0, "No arguments: Count [" << count << "] must be greater than zero");
        for (int i = 0; i < count; ++i) {
            if (strcmp(key, args[i]) == 0) {
                return true;
            }
        }
        return false;
    }
}