#include "Timestep.h"

namespace storytime
{
    Timestep::Timestep(float time)
            : time(time)
    {
    }

    float Timestep::GetSeconds() const
    {
        return time;
    }

    float Timestep::GetMilliseconds() const
    {
        return time / 1000.0f;
    }

    Timestep::operator float() const
    {
        return time;
    }

}