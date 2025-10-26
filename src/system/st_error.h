#pragma once

#include "st_log.h"

#define ST_THROW(message)\
    {\
        std::stringstream ss;\
        ss << message;\
        std::string message_string = ss.str();\
        throw ::Storytime::Error(message_string, ST_TAG);\
    }

#define ST_ASSERT_THROW(expression, message)\
    if (expression) {\
        /* Continue */\
    } else {\
        ST_THROW("Assertion failed: " << #expression << ", " << message);\
    }

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
    struct StacktraceEntry {
        std::string message;
        std::string tag;
    };

    class Error final : public std::exception {
    private:
        std::string message;
        std::string tag;
        std::shared_ptr<Error> previous_error = nullptr;

    public:
        explicit Error(
            const std::string& message,
            const std::string& tag = "",
            const std::shared_ptr<Error>& previous_error = nullptr
        );

        const char* what() const noexcept override;

        std::vector<StacktraceEntry> get_stacktrace() const;

        void print_stacktrace() const;

        static void print_stacktrace(const Error& error);
    };

    std::ostream& operator<<(std::ostream& os, const Error& error);
}