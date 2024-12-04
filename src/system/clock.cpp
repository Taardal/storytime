#include "clock.h"

namespace Storytime {
    TimePoint Time::now() {
        return std::chrono::high_resolution_clock::now();
    }

    TimePoint Time::zero() {
        return std::chrono::high_resolution_clock::time_point(std::chrono::high_resolution_clock::duration::zero());
    }
}

namespace Storytime {
    Clock::Clock(f64 seconds) : elapsed_ns(Time::as<Nanoseconds>(Seconds(seconds))) {
    }

    Nanoseconds Clock::elapsed() const {
        return elapsed_ns;
    }

    Nanoseconds Clock::delta(const Clock& other) const {
        return Nanoseconds(elapsed_ns.count() - other.elapsed_ns.count());
    }

    void Clock::tick(f64 timestep) {
        tick(Seconds(timestep));
    }

    f32 Clock::get_time_scale() const {
        return time_scale;
    }

    void Clock::set_time_scale(f32 time_scale) {
        this->time_scale = time_scale;
    }

    bool Clock::is_paused() const {
        return paused;
    }

    void Clock::set_paused(bool paused) {
        this->paused = paused;
    }
}
