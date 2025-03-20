#pragma once

// These macros are split up in multiple steps to properly expand the __COUNTER__ macro before combining the final name
#define ST_DEFER_COMBINE_NAME_AND_NUMBER(number) defer_##number
#define ST_DEFER_CREATE_RANDOM_NAME(number) ST_DEFER_COMBINE_NAME_AND_NUMBER(number)
#define ST_DEFER_CREATE_NAME ST_DEFER_CREATE_RANDOM_NAME(__COUNTER__)
#define ST_DEFER(on_exit) Storytime::Defer ST_DEFER_CREATE_NAME(on_exit)

namespace Storytime {
    class Defer {
    private:
        std::function<void()> on_exit;

    public:
        explicit Defer(const std::function<void()>& on_exit)
            : on_exit(on_exit) {
        }

        ~Defer() {
            on_exit();
        }
    };
}