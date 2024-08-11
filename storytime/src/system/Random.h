#pragma once

#include <random>

namespace Storytime
{
    class Random
    {
    private:
        std::mt19937 generator;
        std::uniform_int_distribution<std::mt19937::result_type> distribution;

        Random();

        static Random& Get();

    public:
        Random(Random const&) = delete;

        void operator=(Random const&) = delete;

        static float Float(float min = 0, float max = 1);
    };
}