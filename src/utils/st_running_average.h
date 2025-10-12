#pragma once

namespace Storytime {
    /// Calculate a running average, that is "smoothed out" to provide a more stable value,
    /// using the _Exponential Moving Average_ formula. Useful for FPS counters.
    ///
    /// The smoothing factor (alpha) determines how quickly the average reacts to changes (0 < alpha ≤ 1):
    /// - A higher value makes it more reactive.
    /// - A lower value makes it more stable.
    ///
    /// @param value The new value to incorporate into the running average
    /// @param current_average The current value of the running average
    /// @param smoothing_factor The factor by which to smooth the running average (higher = less volatile)
    /// @return The new running average
    f64 smooth_average(f64 value, f64 current_average, f64 smoothing_factor = 0.1);
}
