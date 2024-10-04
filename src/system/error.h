#pragma once

#include "log.h"

#define ST_THROW(message)\
    std::stringstream ss;\
    ss << message;\
    std::string message_string = ss.str();\
    throw Storytime::Error(message_string, ST_TAG)

#ifdef ST_ENABLE_ASSERT
    #define ST_ASSERT_THROW(expression)\
        if (expression) {\
            /* Continue */\
        } else {\
            throw Storytime::Error(std::string("Assertion failed: ") + #expression, ST_TAG);\
        }

    #define ST_ASSERT_THROW_MSG(expression, message)\
        if (expression) {\
            /* Continue */\
        } else {\
            ST_THROW("Assertion failed: " << #expression << ", " << message);\
        }

    #define ST_EXECUTE_THROW(expression)\
        try {\
            expression;\
        } catch (const Error& e) {\
            throw Storytime::Error(#expression, ST_TAG, std::make_shared<Error>(e));\
        } catch (const std::exception& e) {\
            throw Storytime::Error(#expression, ST_TAG, std::make_shared<Error>(e.what()));\
        }

    #define ST_EXECUTE_THROW_MSG(expression, message)\
        try {\
            expression;\
        } catch (const Error& e) {\
            std::stringstream ss;\
            ss << #expression << ", " << message;\
            std::string message_string = ss.str();\
            throw Storytime::Error(message_string, ST_TAG, std::make_shared<Error>(e));\
        } catch (const std::exception& e) {\
            std::stringstream ss;\
            ss << #expression << ", " << message;\
            std::string message_string = ss.str();\
            throw Storytime::Error(message_string, ST_TAG, std::make_shared<Error>(e.what()));\
        }
#else
    #define ST_ASSERT_THROW(expression)
    #define ST_ASSERT_THROW_MSG(expression, message)
    #define ST_EXECUTE_THROW(expression)
    #define ST_EXECUTE_THROW_MSG(expression, message)
#endif

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