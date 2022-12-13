#include <random>
#include "LangSeed.hpp"
namespace LangSeed
{
  std::random_device dev;
  std::mt19937 rng(dev());
}