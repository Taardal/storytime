#include "random.h"

namespace Storytime {
    Random::Random()
        : generator(std::mt19937(std::random_device()())),
          distribution(std::uniform_int_distribution<std::mt19937::result_type>()) {
    }

    Random& Random::get() {
        static Random random;
        return random;
    }

    float Random::get_float(float min, float max) {
        Random& random = get();
        float x = static_cast<float>(random.distribution(random.generator)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
        return x * (max - min) + min;
    }
}
