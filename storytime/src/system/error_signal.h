#pragma once

#include "environment.h"
#include <string>

#if defined(ST_PLATFORM_MACOS) || defined(ST_PLATFORM_LINUX)
    #define ST_PRINT_UNIX_STACKTRACE
#elif defined(ST_PLATFORM_WINDOWS)
    #define ST_PRINT_WINDOWS_STACKTRACE
#endif

namespace Storytime {
    void initialize_error_signal_handlers();

    void handle_error_signal(int signal);

    void print_stacktrace(int signal);

    std::string get_signal_name(int signal);

    std::string get_signal_description(int signal);

#ifdef ST_PRINT_UNIX_STACKTRACE
    void print_unix_stacktrace();

    void demangle_unix_stacktrace_line(std::string* stacktrace_line);
#endif
}