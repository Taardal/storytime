#include "Random.h"


namespace storytime
{
    Random::Random()
            : generator(std::mt19937(std::random_device()())),
              distribution(std::uniform_int_distribution<std::mt19937::result_type>())
    {}

    Random& Random::Get()
    {
        static Random random;
        return random;
    }

    float Random::Float(float min, float max)
    {
        Random& random = Get();
        float x = (float) random.distribution(random.generator) / (float) std::numeric_limits<uint32_t>::max();
        return (x * (max - min)) + min;
    }

}
