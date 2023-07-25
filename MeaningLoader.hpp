#ifndef MEANING_LOADER_HPP
#define MEANING_LOADER_HPP
//remember to set up headers
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <bits/stdc++.h>
#include "LangSeed.hpp"
#include "Constants.h"

class MeaningLoader {
    public:
    MeaningLoader();
    //grab a completely random word
    std::string randMeaning();

    //grab a frequent word
    std::string frequentMeaning();

    //grab a less frequent word
    std::string exoticWord();

    std::string randPrefix();
    std::string randSuffix();

    std::vector<std::string> getPrefixMeanings(){ return prefixMeanings; }
    std::vector<std::string> getSuffixMeanings(){ return suffixMeanings; }

    private:
    std::string meaningList[10000];
    //use vectors for now as we might increase the size of the list
    std::vector<std::string> prefixMeanings;
    std::vector<std::string> suffixMeanings;

};

#endif
