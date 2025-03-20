#include "st_random.h"

namespace Storytime {
    std::mt19937 Random::generator = std::mt19937(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> Random::distribution = std::uniform_int_distribution<std::mt19937::result_type>();

    f32 Random::get_float(f32 min, f32 max) {
        f32 x = (f32) distribution(generator) / (f32) std::numeric_limits<uint32_t>::max();
        return x * (max - min) + min;
    }

    i32 Random::get_int(i32 min, i32 max) {
        std::uniform_int_distribution distribution(min, max);
        return distribution(generator);
    }
}
