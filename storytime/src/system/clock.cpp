#include "clock.h"

namespace Storytime {
    void Clock::start() {
        start_time = std::chrono::high_resolution_clock::now();
    }
}
