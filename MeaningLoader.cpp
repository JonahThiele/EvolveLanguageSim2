#include "MeaningLoader.hpp"

MeaningLoader::MeaningLoader()
{
  //open file read to a vector then place in array var
  std::ifstream meanings("../InputDictionaries/words.txt");
  std::istream_iterator<std::string> start(meanings), end;
  std::vector<std::string> meaningsV(start, end);
  copy(meaningsV.begin(),meaningsV.end(),this->meaningList);

  //add to suffix and prefix list maybe later add ability to have the english meanings idk?
  std::ifstream prefixFile("./prefix.txt");

  if(prefixFile.is_open()){
    std::string line;
    while(std::getline(prefixFile, line)){
        std::size_t index = line.find(":");
        std::string meaning = line.substr(index - 1);
        //trim the line
        meaning.erase(std::remove_if(meaning.begin(), meaning.end(), ::isspace),
        meaning.end());

        prefixMeanings.push_back(meaning);
    }

    prefixFile.close();

  }else {

    //some assert for not being able to open the file
  
  }

   std::ifstream suffixFile("./suffix.txt");

    if(suffixFile.is_open()){
      std::string line;
      while(std::getline(suffixFile, line)){
          std::size_t index = line.find(":");
         std::string meaning = line.substr(index - 1);
         //trim the line
          meaning.erase(std::remove_if(meaning.begin(), meaning.end(), ::isspace),
         meaning.end());

         suffixMeanings.push_back(meaning);
 }

     suffixFile.close();

   }else {

     //some assert for not being able to open the file

 }



}

std::string MeaningLoader::randPrefix(){
    std::uniform_int_distribution<uint_least32_t> distRandPrefix(0, prefixMeanings.size());
    return prefixMeanings[distRandPrefix(LangSeed::rng)];
}

std::string MeaningLoader::randSuffix(){
    std::uniform_int_distribution<uint_least32_t> distRandSuffix(0, suffixMeanings.size());
    return suffixMeanings[distRandSuffix(LangSeed::rng)];
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
