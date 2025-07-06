#include "st_command_line_arguments.h"

namespace Storytime {
    const char* CommandLineArguments::get(int index) const {
        ST_ASSERT(args != nullptr, "Arguments must exist");
        ST_ASSERT(count > 0, "No arguments: Count [" << count << "] must be greater than zero");
        ST_ASSERT(index < count, "Not enough arguments: Index [" << index << "] must be less than count [" << count << "]");
        return args[index];
    }

    std::filesystem::path CommandLineArguments::exe() const {
        return get(0);
    }

    const char* CommandLineArguments::operator[](int index) const {
        return get(index);
    }

    std::ostream& operator<<(std::ostream& os, const CommandLineArguments& arguments) {
        os << "Args:" << std::endl;
        for (int i = 0; i < arguments.count; ++i) {
            os << "[" << i << "] " << arguments.args[i];
            if (i < arguments.count - 1) {
                os << std::endl;
            }
        }
        return os;
    }
}
