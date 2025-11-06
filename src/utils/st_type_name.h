#pragma once

/// Get the name of a type as a human-readable string.
/// @param T The type whose name will be returned
/// @return The name of the type
#define ST_TYPE_NAME(T) ::Storytime::demangle(typeid(T).name())

namespace Storytime {
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