#pragma once

namespace Storytime {
    typedef std::chrono::high_resolution_clock::time_point TimePoint;
    typedef std::chrono::high_resolution_clock::duration Duration;

    typedef std::chrono::seconds sec;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::microseconds mc;
    typedef std::chrono::nanoseconds ns;

    class Time {
    public:
        static TimePoint now();

        template<typename T = Duration>
        static T duration(Duration duration) {
            return std::chrono::duration_cast<T>(duration);
        }

        template<typename T = Duration>
        static T duration(const TimePoint& from, const TimePoint& to) {
            return duration<T>(to - from);
        }
    };

    class Clock {
    private:
        TimePoint start_time = TimePoint::min();
        std::vector<Duration> intervals;
        bool running = false;

    public:
        void start();

        void stop();

        void reset();

        void interval();

        template<typename T = Duration>
        T elapsed() const {
            return Time::duration<T>(Time::now() - start_time);
        }
    };

    class Clock1 {
    private:
        static f32 cycles_per_second; // The frequency of the clock

    private:
        u64 time_cycles; // The time elapsed since start represented in CPU cycles
        f32 time_scale; // The scale of time. Can be used to speed up or slow down the passage of time
        bool paused;

    public:
        static void initialize();

        explicit Clock1(f32 start_time_seconds = 0.0f);

        // Return the current time in cycles
        u64 get_time_cycles() const;

        // Determine the difference between this clock's absolute time and that of
        // another clock, in seconds. We only return time deltas as floating-point
        // seconds, due to the precision limitations of a 32-bit float.
        f32 calc_delta_seconds(const Clock1& other) const;

        // This function should be called once per frame, with the real measured frame time
        // delta in seconds
        void update(f64 timestep_sec);

        f32 get_time_scale() const;

        void set_time_scale(f32 time_scale);

        bool is_paused() const;

        void set_paused(bool paused);

        void single_step();

    private:
        static u64 get_high_resolution_clock_frequency();

        static u64 seconds_to_cycles(f32 time_seconds);

        // WARNING: Dangerous -- only use to convert small durations into seconds
        static f32 cycles_to_seconds(u64 time_cycles);
    };
}