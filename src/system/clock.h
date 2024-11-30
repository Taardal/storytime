#pragma once

namespace Storytime {
    typedef std::chrono::high_resolution_clock::time_point Time;
    typedef std::chrono::high_resolution_clock::duration Duration;

    typedef std::chrono::seconds sec;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::microseconds mc;
    typedef std::chrono::nanoseconds ns;

    class Clock {
    private:
        Time start_time;

    public:
        void start();

        static Time now();

        template<typename T = Duration>
        static T duration(Duration duration) {
            return std::chrono::duration_cast<T>(duration);
        }

        template<typename T = Duration>
        static T duration(const Time& from, const Time& to) {
            return duration<T>(to - from);
        }

        template<typename T = Duration>
        T elapsed() const {
            return duration<T>(now() - start_time);
        }
    };
}