#pragma once

namespace Storytime {
    typedef std::chrono::high_resolution_clock::time_point Time;
    typedef std::chrono::high_resolution_clock::duration Duration;

    typedef std::chrono::seconds sec;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::nanoseconds ns;

    class Clock {
    private:
        Time start_time;

    public:
        void start();

        template<typename T = Duration>
        f64 get_time() {
            Time now = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<T>(now - start_time).count();
        }
    };
}