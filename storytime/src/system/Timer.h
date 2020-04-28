#pragma once

#include <chrono>

namespace storytime
{
    class Timer
    {
    private:
        typedef std::chrono::high_resolution_clock HighResolutionClock;
        typedef std::chrono::duration<long, std::nano> Nanoseconds;
        typedef std::chrono::duration<float, std::milli> Milliseconds;

        template<typename T>
        using TimePoint = std::chrono::time_point<T>;

        TimePoint<HighResolutionClock> start;

    public:
        void Reset();

        float ElapsedMs();

        float ElapsedSec();

    private:
        static float NsToMs(Nanoseconds nanoseconds);
    };
}


