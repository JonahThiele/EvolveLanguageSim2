#include "MeaningLoader.hpp"

MeaningLoader::MeaningLoader()
{
  //open file read to a vector then place in array var
  std::ifstream meanings("../InputDictionaries/words.txt");
  std::istream_iterator<std::string> start(meanings), end;
  std::vector<std::string> meaningsV(start, end);
  copy(meaningsV.begin(),meaningsV.end(),this->meaningList);
   
}

std::string MeaningLoader::randMeaning()
{
    std::uniform_int_distribution<uint_least32_t> distRandWord( 0,  MAX_MEANINGS);
    return meaningList[distRandWord(LangSeed::rng)];

}

std::string MeaningLoader::exoticWord()
{
    std::uniform_int_distribution<uint_least32_t> distLow( 0,  1);
    int  index = std::floor(0 + (MAX_MEANINGS - 0) * std::pow(distLow(LangSeed::rng), MIN_MEANING_DIST));
    return meaningList[index];
}

std::string MeaningLoader::frequentMeaning()
{
    std::uniform_int_distribution<uint_least32_t> distLow( 0,  1);
    int  index = std::floor(0 + (MAX_MEANINGS - 0) * std::pow(distLow(LangSeed::rng), MAX_MEANING_DIST));
    return meaningList[index];
}