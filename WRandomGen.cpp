#include "WRandGen.hpp"

std::mt19937 WRandGen::generator() 
{
    std::random_device os_seed;
    const uint_least32_t seed = os_seed();
    std::mt19937 generator(seed);
    return generator;
}

std::uniform_int_distribution<uint_least32_t> WRandGen::distribute(int start, int end)
{
    std::uniform_int_distribution<uint_least32_t> distribute( start, end);
    return distribute;
}