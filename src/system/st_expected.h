#pragma once

#include "system/st_error.h"

#include <expected>

/// Create a std::unexpected with Error as value.
#define ST_UNEXPECTED(message, ...)\
    std::unexpected(ST_ERROR(message, ##__VA_ARGS__))

/// Create a std::unexpected with Error as value, and another error as its previous error.
#define ST_UNEXPECTED_FROM(error, message, ...)\
    std::unexpected(ST_ERROR_FROM(error, message, ##__VA_ARGS__))

namespace Storytime {
    /// Obtain the underlying type T by removing its top-level const, volatile, and reference (& or &&) qualifiers.
    template<typename T>
    using CleanType = std::remove_cvref_t<T>;

    /// Ensure type T is Error
    template<typename T>
    concept IsError = std::same_as<CleanType<T>, Error>;

    /// Ensure type T is std::unexpected<Error>
    template<typename T>
    concept IsUnexpectedOfError = std::same_as<CleanType<T>, std::unexpected<Error>>;

    ///
    /// @brief Overload operator '+' to chain the error of a std::unexpected<Error> together with another Error into a linked list.
    /// @code{.cpp}
    /// std::expected<void, Error> result = do_something();
    /// if (!result) {
    ///     return ST_UNEXPECTED("Error code: {}", 123) + result.error();
    /// }
    /// @endcode
    ///
    /// @return A new std::unexpected<Error> whose value is a copy of the LEFT unexpected's error, with the RIGHT error as its previous error.
    ///
    template<typename Left, typename Right>
    requires IsUnexpectedOfError<Left> && IsError<Right> // std::unexpected<Error> + Error
    std::unexpected<Error> operator+(Left&& unexpected, Right&& error) {
        // Create a new error created from the LEFT unexpected's error, with the RIGHT error as its previous error, and
        // return a new unexpected object with the new error object as its value.
        return std::unexpected<Error>(
            std::forward<Left>(unexpected).error().with_previous(std::forward<Right>(error))
        );
    }

    ///
    /// @brief Overload operator '+' to chain the error of a std::unexpected<Error> together with the error of another std::unexpected<Error> into a linked list.
    /// @code{.cpp}
    /// std::expected<void, Error> result = do_something();
    /// if (!result) {
    ///     auto a = ST_UNEXPECTED("Something failed with error code: {}", 123);
    ///     std::expected<void, Error> other_result = do_something_else();
    ///     if (!other_result) {
    ///         auto b = ST_UNEXPECTED("Something else failed with error code: {}", 456);
    ///         a = a + b;
    ///     }
    ///     return a;
    /// }
    /// @endcode
    ///
    /// @return A new std::unexpected<Error> whose value is a copy of the LEFT unexpected's error, with the RIGHT unexpected's error as its previous error.
    ///
    template<typename Left, typename Right>
    requires IsUnexpectedOfError<Left> && IsUnexpectedOfError<Right> // std::unexpected<Error> + std::unexpected<Error>
    std::unexpected<Error> operator+(Left&& unexpected_a, Right&& unexpected_b) {
        // Create a new error created from the LEFT unexpected's error, with the RIGHT unexpected's error as its
        // previous error, and return a new unexpected object with the new error object as its value.
        return std::unexpected<Error>(
            std::forward<Left>(unexpected_a).error().with_previous(std::forward<Right>(unexpected_b).error())
        );
    }
}