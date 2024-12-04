#pragma once

namespace Storytime {
    typedef std::chrono::high_resolution_clock::duration Duration;
    typedef std::chrono::high_resolution_clock::time_point TimePoint;

    class Time {
    public:
        static TimePoint now();

        static TimePoint zero();

        template<typename ToDuration, typename FromDuration>
        static ToDuration as(FromDuration from_duration) {
            return std::chrono::duration_cast<ToDuration>(from_duration);
        }
    };
}

namespace Storytime {
    typedef std::chrono::duration<i64, std::nano> Nanoseconds;
    typedef std::chrono::duration<i64, std::micro> Microseconds;
    typedef std::chrono::duration<i64, std::milli> Milliseconds;
    typedef std::chrono::duration<f64> Seconds;

    typedef Nanoseconds ns;
    typedef Microseconds us; typedef Microseconds μs;
    typedef Milliseconds ms;
    typedef Seconds sec;

    class Clock {
    private:
        Nanoseconds elapsed_ns;
        f32 time_scale = 1.0f;
        bool paused = false;

    public:
        explicit Clock(f64 seconds = 0.0);

        Nanoseconds elapsed() const;

        template<typename Duration>
        Duration elapsed() const {
            return Time::as<Duration>(elapsed());
        }

        Nanoseconds delta(const Clock& other) const;

        template<typename Duration>
        Duration delta(const Clock& other) const {
            return Time::as<Duration>(delta(other));
        }

        void tick(f64 timestep);

        template<typename Duration = Nanoseconds>
        void tick(Duration duration) {
            if (paused) {
                return;
            }
            elapsed_ns += Time::as<Nanoseconds>(duration * time_scale);
        }

        f32 get_time_scale() const;

        void set_time_scale(f32 time_scale);

        bool is_paused() const;

        void set_paused(bool paused);
    };
}
