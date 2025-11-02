#pragma once

#include "st_log.h"

/// Create an Error with a formatted string
#define ST_ERROR(message, ...)\
    ::Storytime::Error(std::format(message, ##__VA_ARGS__), ST_TAG)

/// Create an Error with a formatted string, and another error as its previous error.
#define ST_ERROR_FROM(error, message, ...)\
    ::Storytime::Error(std::format(message, ##__VA_ARGS__), ST_TAG, std::make_shared<::Storytime::Error>(error))

/// Throw an Error with a formatted string.
#define ST_THROW(message)\
    {\
        std::stringstream ss;\
        ss << message;\
        std::string message_string = ss.str();\
        throw ::Storytime::Error(message_string, ST_TAG);\
    }

/// Assert an expression and throw an Error if it is false.
#define ST_ASSERT_THROW(expression, message)\
    if (expression) {\
        /* Continue */\
    } else {\
        ST_THROW("Assertion failed: " << #expression << ", " << message);\
    }

/// @brief Try to excecute an expression and catch and rethrow any Error, or std::exception, as a new Error with the
/// caught error as its previous error.
#define ST_TRY_THROW(expression, message)\
    try {\
        expression;\
    } catch (const ::Storytime::Error& e) {\
        std::stringstream ss;\
        ss << message << ", " << #expression;\
        std::string message_string = ss.str();\
        throw ::Storytime::Error(message_string, ST_TAG, std::make_shared<::Storytime::Error>(e));\
    } catch (const std::exception& e) {\
        std::stringstream message_ss;\
        message_ss << message << ", " << #expression;\
        std::string message_string = message_ss.str();\
        std::stringstream exception_ss;\
        exception_ss << e.what() << ", " << ::Storytime::demangle(typeid(e).name());\
        std::string exception_string = exception_ss.str();\
        throw ::Storytime::Error(message_string, ST_TAG, std::make_shared<::Storytime::Error>(exception_string));\
    }

namespace Storytime {
    /// A single line in an Error's stacktrace.
    struct StacktraceEntry {
        std::string message;
        std::string tag;
    };

    ///
    /// A custom error type with its own stacktrace handling and printing that can be thrown as an exception or
    /// returned as a std::unexpected<Error>.
    /// @code{.cpp}
    /// try {
    ///     do_something_that_throws();
    /// } catch (const std::exception& e) {
    ///     throw Error(std::format("Something failed: {}", e.what()), ST_TAG);
    /// }
    /// @endcode
    ///
    /// The stacktrace is formed by chaining together errors as a linked list by constructing it with another error as
    /// its previous error.
    /// @code{cpp}
    /// Error a = Error("Error A", "Tag A");
    /// Error b = Error("Error B", "Tag B", std::make_shared<Error>(a));
    /// b.print_stacktrace(); // Will print both b.message and a.message
    /// @endcode
    ///
    class Error final : public std::exception {
    private:
        std::string message;
        std::string tag;
        std::shared_ptr<Error> previous_error = nullptr;

    public:
        Error(
            const std::string& message,
            const std::string& tag = "",
            const std::shared_ptr<Error>& previous_error = nullptr
        );

        const char* what() const noexcept override;

        std::vector<StacktraceEntry> get_stacktrace() const;

        void print_stacktrace() const;

        static void print_stacktrace(const Error& error);

        ///
        /// @brief Create a new error from this error with the other error as the previous error.
        /// @code{.cpp}
        /// try {
        ///     do_something_that_throws();
        /// } catch (const Error& e) {
        ///     auto e1 = Error("Something failed");
        ///     auto e2 = e1.with_previous(e);
        ///     e2.print_stacktrace(); // Will print both e1.message and e.message
        /// }
        /// @endcode
        /// @return A copy of this error with the other error as its previous error.
        ///
        Error with_previous(const Error& other) const;

        Error with_previous(Error&& other) const;

        ///
        /// @brief Create a new error from this error with the other error as the previous error.
        /// @code{.cpp}
        /// try {
        ///     do_something_that_throws();
        /// } catch (const Error& e) {
        ///     auto e1 = Error("Something failed");
        ///     auto e2 = e1 + e;
        ///     e2.print_stacktrace(); // Will print both e1.message and e.message
        /// }
        /// @endcode
        /// @return A copy of this error with the other error as its previous error.
        ///
        Error operator+(const Error& other) const;

        Error operator+(Error&& other) const;
    };

    std::ostream& operator<<(std::ostream& os, const Error& error);
}