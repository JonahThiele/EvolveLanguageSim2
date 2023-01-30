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

    private:
    std::string meaningList[10000];

};