#include "clock.h"

namespace Storytime {
    void Clock::start() {
        start_time = now();
    }

    Time Clock::now() {
        return std::chrono::high_resolution_clock::now();
    }
}
