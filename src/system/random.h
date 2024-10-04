#pragma once

#include <random>

namespace Storytime {
    class Random {
    private:
        static std::mt19937 generator;
        static std::uniform_int_distribution<std::mt19937::result_type> distribution;

    public:
        static f32 get_float(f32 min = 0.0f, f32 max = 1.0f);

        static i32 get_int(i32 min = 0, i32 max = 1);
    };
}
