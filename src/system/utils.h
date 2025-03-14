#pragma once

#define ST_COUTLN(message)\
    std::cout << message << std::endl

#define ST_CERRLN(message)\
    std::cerr << message << std::endl

#define ST_TO_STRING(value)\
    #value

#define ST_STRING_CONCAT(string_variable, string_content)\
    std::stringstream ss;\
    ss << string_content;\
    string_variable = ss.str()

bool streq(const char* a, const char* b);

namespace Storytime {
    bool string_ends_with(const std::string& str, const std::string& suffix);

    std::vector<std::string> string_split(const std::string& str, char delimiter);

    void string_replace_first(std::string& str, const std::string& target, const std::string& replacement);

    void string_replace_all(std::string& str, const std::string& target, const std::string& replacement);

    void string_to_upper(std::string& str);

    void string_to_lower(std::string& str);

    /// Calculate a running average, that is "smoothed out" to provide a more stable value,
    /// using the _Exponential Moving Average_ formula. Useful for FPS counters.
    ///
    /// The smoothing factor (alpha) determines how quickly the average reacts to changes (0 < alpha ≤ 1):
    /// - A higher value makes it more reactive.
    /// - A lower value makes it more stable.
    ///
    /// @param value The new value to incorporate into the running average
    /// @param current_average The current value of the running average
    /// @param smoothing_factor The factor by which to smooth the running average (higher = less volatile)
    /// @return The new running average
    f64 smooth_average(f64 value, f64 current_average, f64 smoothing_factor = 0.1);

    /// Demangle (i.e. decode) a string to make it human-readable.
    ///
    /// "Mangling" is a technique used by compilers to encode additional information
    /// into symbol names in compiled code.
    ///
    /// This additional information includes things like function names, namespaces,
    /// argument types, and template parameters.
    ///
    /// We want to "demangle" (i.e. decode) this information to make it human-readable.
    ///
    /// @param value The string to demangle
    /// @return A new demangled string. The original string is not modified.
    std::string demangle(const std::string& value);

#ifdef ST_PLATFORM_UNIX
    std::string demangle_unix(const std::string& value);
#endif

    /// Get the name of a type as a human-readable string.
    /// @tparam T The type whose name will be returned
    /// @return The name of the type
    template<typename T>
    std::string type_name() {
        return ::Storytime::demangle(typeid(T).name());
    }
}

/// Get the name of a type as a human-readable string.
/// @param T The type whose name will be returned
/// @return The name of the type
#define ST_TYPE_NAME(T) ::Storytime::demangle(typeid(T).name())

