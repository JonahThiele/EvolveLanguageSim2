#ifndef W_RANDOM_GEN_HPP
#define W_RANDOM_GEN_HPP

#include <random>
#include <cctype>

class WRandGen {
    public:
        static std::uniform_int_distribution<uint_least32_t> distribute(int start, int end);

        static std::mt19937 generator(); 
};

#endif