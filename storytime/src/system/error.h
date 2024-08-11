#pragma once

#include "log.h"

#include <iostream>

#define ST_THROW(message) throw Error(message, ST_TAGZ())

#define ST_ASSERT_THROW(expression) \
    if (expression) {\
        /* Continue */\
    } else {\
        throw Error(std::string("Assertion failed: ") + #expression, ST_TAGZ());\
    }

#define ST_EXECUTE_THROW(expression) \
    try {\
        expression;\
    } catch (const Error& e) { \
        throw Error(#expression, ST_TAGZ(), std::make_shared<Error>(e));\
    } catch (const std::exception& e) {\
        throw Error(#expression, ST_TAGZ(), std::make_shared<Error>(e.what()));\
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
        std::shared_ptr<Error> previousError = nullptr;

    public:
        Error(const std::string& message, const std::string& tag = "", const std::shared_ptr<Error>& previousError = nullptr);

        const char* what() const noexcept override;

        std::vector<StacktraceEntry> getStacktrace() const;

        void printStacktrace() const;

        static void printStacktrace(const Error& error);
    };

    std::ostream& operator<<(std::ostream& os, const Error& error);
}