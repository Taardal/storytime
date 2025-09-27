bool streq(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

namespace Storytime {
    bool string_ends_with(const std::string& str, const std::string& suffix) {
        // Check if the string is at least as long as the suffix
        if (str.size() < suffix.size()) {
            return false;
        }
        // Compare the end of the string with the suffix
        return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
    }

    std::vector<std::string> string_split(const std::string& str, char delimiter) {
        // If the delimiter is not found, return a vector containing the original string
        if (str.find(delimiter) == std::string::npos) {
            return {str};
        }
        // Split the string by the delimiter and return the tokens/segments in a vector
        std::string token;
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    void string_replace_first(std::string& str, const std::string& target, const std::string& replacement) {
        size_t start = str.find(target);
        if (start == std::string::npos) {
            return;
        }
        str.replace(start, target.length(), replacement);
    }

    void string_replace_all(std::string& str, const std::string& target, const std::string& replacement) {
        size_t start = 0;
        while ((start = str.find(target)) != std::string::npos) {
            str.replace(start, target.length(), replacement);
        }
    }

    void string_to_upper(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

    void string_to_lower(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    }

    f64 smooth_average(f64 value, f64 current_average, f64 smoothing_factor) {
        return smoothing_factor * value + (1.0 - smoothing_factor) * current_average;
    }

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

    glm::vec2 to_vec2(const glm::vec3& vec) {
        return { vec.x, vec.y };
    }

    glm::vec2 to_vec2(const glm::vec4& vec) {
        return { vec.x, vec.y };
    }

    glm::vec3 to_vec3(const glm::vec2& vec) {
        return { vec.x, vec.y, 0.0f };
    }

    glm::vec3 to_vec3(const glm::vec4& vec) {
        return { vec.x, vec.y, vec.z };
    }

    glm::vec4 to_vec4(const glm::vec2& vec) {
        return { vec.x, vec.y, 0.0f, 0.0f };
    }

    glm::vec4 to_vec4(const glm::vec3& vec) {
        return { vec.x, vec.y, vec.z, 0.0f };
    }
}
