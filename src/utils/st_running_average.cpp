#include "st_running_average.h"

namespace Storytime {
    f64 smooth_average(f64 value, f64 current_average, f64 smoothing_factor) {
        return smoothing_factor * value + (1.0 - smoothing_factor) * current_average;
    }
}
