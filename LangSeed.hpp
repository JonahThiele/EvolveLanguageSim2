// my_rng.hpp
#ifndef SEED_HPP
#define SEED_HPP

#include <random>

namespace LangSeed
{
  extern std::random_device dev;
  extern std::mt19937 rng;
}

#endif 