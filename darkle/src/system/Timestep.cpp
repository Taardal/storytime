#include "Timestep.h"

namespace Darkle {

    Timestep::Timestep(double time): time(time) {
    }

    double Timestep::getSeconds() const {
        return time;
    }

    double Timestep::getMilliseconds() const {
        return time / 1000.0;
    }

    Timestep::operator double() const {
        return time;
    }

}