#include "Timestep.h"

namespace storytime {

    Timestep::Timestep(float time): time(time) {
    }

    float Timestep::getSeconds() const {
        return time;
    }

    float Timestep::getMilliseconds() const {
        return time / 1000.0f;
    }

    Timestep::operator float() const {
        return time;
    }

}