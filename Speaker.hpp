#ifndef SPEAKER_CPP
#define SPEAKER_CPP

#include <string>
#include <vector>
#include "Word.hpp"
#include "Constants.h"
#include "WRandGen.hpp"
#include "LangSeed.hpp"
#include <cstdlib>
#include <random>
#include <cctype>
#include <algorithm>
#include <cmath>

class Speaker {
    public:
        Speaker(int x, int y, std::vector<Word>);

        //interact with other person and share % of the dictionary
        std::vector<Word> speakToOtherPerson(Speaker &otherPerson);

        //accept a part of another speaker's dictionary
        void learnWords(std::vector<Word> sharedWords);

        //return dictionary
        std::vector<Word> getDict(){ return dictionary;}

        int getY(){return y;}
        int getX(){return x;}

    private:

        //word handle or pointer
        int y;
        int x;

        //the internal dictionary exchanged by the speakers
        std::vector<Word> dictionary;
        
        bool compare(Speaker &speaker);
        //random handling
        //std::mt19937 gen = WRandGen::generator();


};

#endif