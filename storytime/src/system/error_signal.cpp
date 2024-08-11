#include "error_signal.h"

namespace Storytime {
    void initialize_error_signal_handlers() {
        signal(SIGHUP, handle_error_signal);  // Hangup
        signal(SIGILL, handle_error_signal);  // Illegal instruction
        signal(SIGTRAP, handle_error_signal); // Trace trap
        signal(SIGABRT, handle_error_signal); // Abort
        signal(SIGEMT, handle_error_signal);  // EMT instruction, system-specific
        signal(SIGFPE, handle_error_signal);  // Floating-point exception
        signal(SIGKILL, handle_error_signal); // Kill, cannot be caught or ignored
        signal(SIGBUS, handle_error_signal);  // Bus error
        signal(SIGSEGV, handle_error_signal); // Segmentation violation
        signal(SIGSYS, handle_error_signal);  // Bad argument to system call
        signal(SIGPIPE, handle_error_signal); // Write on a pipe with no reader
        signal(SIGALRM, handle_error_signal); // Alarm clock
        signal(SIGXCPU, handle_error_signal); // Exceeded CPU time limit
        signal(SIGXFSZ, handle_error_signal); // Exceeded file size limit
    }

    void handle_error_signal(int signal) {
        print_stacktrace(signal);
        exit(signal);
    }

    void print_stacktrace(int signal) {
        std::string name = get_signal_name(signal);
        std::string description = get_signal_description(signal);
        fprintf(stderr, "--------------------------------------------------------------------------------------------------------------\n");
        fprintf(stderr, "[%s] %s\n", name.c_str(), description.c_str());
        fprintf(stderr, "--------------------------------------------------------------------------------------------------------------\n");
#if defined(ST_PRINT_UNIX_STACKTRACE)
        print_unix_stacktrace();
#elif defined(ST_PRINT_WINDOWS_STACKTRACE)
        fprintf(stderr, "Could not print stacktrace for Windows, not implemented\n");
#else
        fprintf(stderr, "Could not print stacktrace, unsupported platform\n");
#endif
    }

    std::string get_signal_name(const int signal) {
        switch (signal) {
            case SIGHUP:
                return "SIGHUP";
            case SIGINT:
                return "SIGINT";
            case SIGQUIT:
                return "SIGQUIT";
            case SIGILL:
                return "SIGILL";
            case SIGTRAP:
                return "SIGTRAP";
            case SIGABRT:
                return "SIGABRT";
            case SIGFPE:
                return "SIGFPE";
            case SIGKILL:
                return "SIGKILL";
            case SIGBUS:
                return "SIGBUS";
            case SIGSEGV:
                return "SIGSEGV";
            case SIGSYS:
                return "SIGSYS";
            case SIGPIPE:
                return "SIGPIPE";
            case SIGALRM:
                return "SIGALRM";
            case SIGTERM:
                return "SIGTERM";
            case SIGXCPU:
                return "SIGXCPU";
            case SIGXFSZ:
                return "SIGXFSZ";
            default:
                return "";
        }
    }

    std::string get_signal_description(int signal) {
        switch (signal) {
            case SIGHUP:
                return "Hangup. Typically sent to a process when its controlling terminal is closed. Often used to reload configurations.";
            case SIGINT:
                return "Interrupt. Sent when the user types the interrupt character (usually Ctrl+C).";
            case SIGQUIT:
                return "Quit. Sent when the user types the quit character (usually Ctrl+).";
            case SIGILL:
                return "Illegal instruction. Indicates that the process has attempted to execute an illegal, malformed, or privileged instruction.";
            case SIGTRAP:
                return "Trace trap. Used by debuggers to handle breakpoint traps and other traps.";
            case SIGABRT:
                return "Abort. Sent by the abort() function.";
            case SIGFPE:
                return "Floating-point exception. Indicates an erroneous arithmetic operation, such as division by zero or an overflow.";
            case SIGKILL:
                return "Kill. This signal cannot be caught or ignored.";
            case SIGBUS:
                return "Bus error. Indicates an access to an invalid address.";
            case SIGSEGV:
                return "Segmentation violation. Indicates an invalid access to storage.";
            case SIGSYS:
                return "Bad system call. Indicates an invalid system call.";
            case SIGPIPE:
                return "Broken pipe. Sent when a process writes to a pipe without a reader.";
            case SIGALRM:
                return "Alarm clock. Sent when a timer set by alarm() expires.";
            case SIGTERM:
                return "Termination. Sent to request termination.";
            case SIGXCPU:
                return "CPU time limit exceeded.";
            case SIGXFSZ:
                return "File size limit exceeded.";
            default:
                return "";
        }
    }

#ifdef ST_PRINT_UNIX_STACKTRACE

#include <cxxabi.h>
#include <execinfo.h>
#include <unistd.h>

    void print_unix_stacktrace() {
        // Number of stacktrace lines to be printed
        constexpr int maxStackSize = 20;

        // Linux man page: backtrace, backtrace_symbols, backtrace_symbols_fd
        // https://linux.die.net/man/3/backtrace_symbols

        // backtrace() returns a backtrace for the calling program.
        // A backtrace is the series of currently active function calls for the program.
        // Each item is of type void*, and is the return address from the corresponding stack frame
        void* stack[maxStackSize];
        int stackSize = backtrace(stack, maxStackSize);

        // Given the set of addresses returned by backtrace() in buffer,
        // backtrace_symbols() translates the addresses into an array of strings that describe the addresses symbolically
        char** stacktrace = backtrace_symbols(stack, stackSize);
        if (stacktrace == nullptr) {
            fprintf(stderr, "Could not resolve stacktrace\n");
            return;
        }

        // Iterate over the backtrace and...
        // 1. Convert the line to a std::string
        // 2. Demangle the line
        // 3. Print the line
        //
        // At this point, the top of the stack will look something like this
        // 0 [...] printUnixStacktrace()
        // 1 [...] printStacktrace(int)
        // 2 [...] handleErrorSignal(int)
        // 3 [...] _sigtramp
        // 4 [...] functionThatCausedTheSignal()
        //
        // The "_sigtramp" line means that the application has received a Unix signal. We are only interested in what
        // happened _before_ the signal occurred, so we want to ignore that line and any lines above it on the stack.
        //
        bool sigtrampLineFound = false;
        for (int i = 0; i < stackSize; i++) {
            ::std::string stacktraceLine(stacktrace[i]);
            demangle_unix_stacktrace_line(&stacktraceLine);
            if (sigtrampLineFound) {
                fprintf(stderr, "%s\n", stacktraceLine.c_str());
            } else if (stacktraceLine.find("_sigtramp") != ::std::string::npos) {
                sigtrampLineFound = true;
            }
        }

        // This array is malloced by backtrace_symbols(), and must be freed by the caller
        free(stacktrace);
    }

    // "Mangling" is a technique used by compilers to encode additional information into symbol names in compiled code.
    // This additional information includes things like function names, namespaces, argument types, and template parameters.
    // We want to "demangle" (i.e. decode) this information to make it human-readable.
    //
    // Mangled stacktrace line:
    //   10  blink  0x0000000102cbfe5c  _ZN5Storytime3App10initializeEv + 1960
    //
    // Demangled stacktrace line:
    //   10  blink  0x00000001028b3e50  Storytime::App::initialize() + 1960
    //
    // Segments:
    //   [stack index]  [binary name]  [return address (in hexadecimal)]  [function name] + [offset into the function (in hexadecimal)]
    void demangle_unix_stacktrace_line(::std::string* stacktraceLine) {

        // Find segment before the mangled function name
        size_t returnAddressStartIndex = stacktraceLine->find("0x");
        size_t returnAddressEndIndex = returnAddressStartIndex + 18; // 18 = return address length

        // Find function name start and end
        size_t functionNameStartIndex = returnAddressEndIndex + 1; // 1 = space
        size_t functionNameEndIndex = stacktraceLine->find(" + ");
        size_t functionNameLength = functionNameEndIndex - functionNameStartIndex;

        // Extract the mangled function name
        ::std::string functionName = stacktraceLine->substr(functionNameStartIndex, functionNameLength);

        // Demangling status:
        // [0] The demangling operation succeeded
        // [-1] A memory allocation failiure occurred
        // [-2] mangled_name is not a valid name under the C++ ABI mangling rules
        // [-3] One of the arguments is invalid
        int demanglingStatus = -1;

        // A region of memory, allocated with malloc, of *length bytes, into which the demangled name is stored.
        // May be NULL; in that case, the demangled name is placed in a region of memory allocated with malloc.
        char* outputBuffer = nullptr;

        // If length is non-NULL, the length of the buffer containing the demangled name is placed in *length.
        size_t* length = nullptr;

        // https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
        char* demangledFunctionName = abi::__cxa_demangle(functionName.c_str(), outputBuffer, length, &demanglingStatus);

        // Replace the mangled function name with the demangled one in the stacktrace line
        bool successfullyDemangled = demanglingStatus == 0 && demangledFunctionName != nullptr;
        if (successfullyDemangled) {
            stacktraceLine->replace(functionNameStartIndex, functionNameLength, demangledFunctionName);
        }

        // If the output buffer is NULL, the demangled name is placed in a region of memory allocated with malloc.
        // The caller is responsible for deallocating this memory using free.
        free(demangledFunctionName);
    }
#endif
}