#include "Timer.h"

namespace storytime
{
    void Timer::Reset()
    {
        start = HighResolutionClock::now();
    }

    float Timer::ElapsedMs()
    {
        return NsToMs(HighResolutionClock::now() - start);
    }

    float Timer::ElapsedSec()
    {
        return ElapsedMs() / 1000.0f;
    }

    float Timer::NsToMs(Timer::Nanoseconds nanoseconds)
    {
        return std::chrono::duration_cast<Milliseconds>(nanoseconds).count();
    }

}
