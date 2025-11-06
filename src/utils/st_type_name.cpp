#include "st_type_name.h"

namespace Storytime {
    std::string demangle(const std::string& value) {
#if defined(ST_PRINT_UNIX_STACKTRACE)
        return demangle_unix(value);
#elif defined(ST_PRINT_WINDOWS_STACKTRACE)
        fprintf(stderr, "Could not demangle value for Windows, not implemented\n");
        ST_BREAK();
#else
        fprintf(stderr, "Could not demangle value, unsupported platform\n");
        ST_BREAK();
#endif
    }

#ifdef ST_PLATFORM_UNIX
    std::string demangle_unix(const std::string& value) {
        std::string result;

        // Demangling status:
        // [0] The demangling operation succeeded
        // [-1] A memory allocation failiure occurred
        // [-2] mangled_name is not a valid name under the C++ ABI mangling rules
        // [-3] One of the arguments is invalid
        int demangling_status = -1;

        // A region of memory, allocated with malloc, of *length bytes, into which the demangled name is stored.
        // May be NULL; in that case, the demangled name is placed in a region of memory allocated with malloc.
        char* output_buffer = nullptr;

        // If length is non-NULL, the length of the buffer containing the demangled name is placed in *length.
        size_t* length = nullptr;

        // https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
        char* demangled_value = abi::__cxa_demangle(value.c_str(), output_buffer, length, &demangling_status);

        // Replace the mangled function name with the demangled one in the stacktrace line
        bool successfully_demangled = demangling_status == 0 && demangled_value != nullptr;
        if (successfully_demangled) {
            result = demangled_value;
        }

        // If the output buffer is NULL, the demangled name is placed in a region of memory allocated with malloc.
        // The caller is responsible for deallocating this memory using free.
        free(demangled_value);

        return result;
    }
#endif
}