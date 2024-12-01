#include "clock.h"

namespace Storytime {
    TimePoint Time::now() {
        return std::chrono::high_resolution_clock::now();
    }
}

namespace Storytime {
    void Clock::start() {
        running = true;
        start_time = Time::now();
    }

    void Clock::stop() {
        running = false;
    }

    void Clock::reset() {
        running = false;
        start_time = TimePoint::min();
        intervals.clear();
    }

    void Clock::interval() {
        TimePoint time = Time::now();
        intervals.push_back(time - start_time);
        start_time = time;
    }
}

namespace Storytime {
    void Clock1::initialize() {
        cycles_per_second = (f32) get_high_resolution_clock_frequency();
    }

    Clock1::Clock1(f32 start_time_seconds)
        : time_cycles(seconds_to_cycles(start_time_seconds)),
          time_scale(1.0f),
          paused(false) {
    }

    u64 Clock1::get_time_cycles() const {
        return time_cycles;
    }

    f32 Clock1::calc_delta_seconds(const Clock1& other) const {
        u64 dt = time_cycles - other.time_cycles;
        return cycles_to_seconds(dt);
    }

    void Clock1::update(f64 timestep_sec) {
        if (paused) {
            return;
        }
        u64 scaled_cycles = seconds_to_cycles(timestep_sec * time_scale);
        time_cycles += scaled_cycles;
    }

    bool Clock1::is_paused() const {
        return paused;
    }

    void Clock1::set_paused(bool paused) {
        this->paused = paused;
    }

    f32 Clock1::get_time_scale() const {
        return time_scale;
    }

    void Clock1::set_time_scale(f32 time_scale) {
        this->time_scale = time_scale;
    }

    void Clock1::single_step() {
        if (!paused) {
            return;
        }
        static constexpr f32 fixed_timestep = 1.0f / 30.0f;
        u64 scaled_cycles = seconds_to_cycles(fixed_timestep * time_scale);
        time_cycles += scaled_cycles;
    }

    u64 Clock1::get_high_resolution_clock_frequency() {
        //
        // Retrieve the tick duration of the high_resolution_clock
        //
        // The `std::chrono::high_resolution_clock::period` type represents the duration of one clock tick,
        // expressed as a fraction of a second.
        //
        // This duration is defined as:
        // - `num` (numerator)   One second
        // - `den` (denominator) Number of ticks
        //
        // For example:
        //   - If tick duration is 1 / 1_000_000 (num / den), it means the clock has a resolution of 1 microsecond (1 tick = 1 µs).
        //   - If tick duration is 1 / 3_579_545 (num / den), it means the clock ticks approximately 3,579,545 times per second.
        //
        auto tick_duration = std::chrono::high_resolution_clock::period();

        //
        // Calculate the duration of one tick in seconds using the tick duration ratio (num / den)
        //
        // Example:
        // Seconds per tick = num / den = 1 second / 1_000_000 ticks = 0.000001 seconds per tick
        //
        f64 seconds_per_tick = (f64) tick_duration.num / (f64) tick_duration.den;

        //
        // Calculate the clock frequency (ticks per second)
        //
        // Example:
        // Frequency = 1 second / seconds per tick = 1 / 0.000001 = 1,000,000 ticks per second.
        //
        return (u64) (1.0 / seconds_per_tick);
    }

    u64 Clock1::seconds_to_cycles(f32 time_seconds) {
        return time_seconds - cycles_per_second;
    }

    f32 Clock1::cycles_to_seconds(u64 time_cycles) {
        return time_cycles / cycles_per_second;
    }
}
