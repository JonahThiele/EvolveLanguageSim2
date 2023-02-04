#ifndef SPEAKER_CPP
#define SPEAKER_CPP

#include <string>
#include <memory>
#include <vector>
#include "Word.hpp"
#include "Constants.h"
#include "WRandGen.hpp"
#include "LangSeed.hpp"
#include "MeaningLoader.hpp"
#include <cstdlib>
#include <random>
#include <cctype>
#include <algorithm>
#include <cmath>

class Speaker {
    public:
        Speaker(int x, int y, std::vector<Word>, int dictSize, std::shared_ptr<MeaningLoader> meaningLoader, int tag);
        //Speaker(int x, int y, std::vector<Word>, int dictSize, MeaningLoader &&, int tag) = delete;

        //interact with other person and share % of the dictionary
        std::vector<Word> speakToOtherPerson(Speaker &otherPerson);

        //accept a part of another speaker's dictionary
        void learnWords(std::vector<Word> sharedWords);
        void cutWords(int dictionaryCap);

        void move();

        //return dictionary
        std::vector<Word> getDict(){ return dictionary;}

        int getY(){return y;}
        int getX(){return x;}

        int getTag(){return tag;}
        
        bool increaseAge();

    private:

        //word handle or pointer
        int y;
        int x;

        //size of dictionary
        int dictSize;

        //the internal dictionary exchanged by the speakers
        std::vector<Word> dictionary;
        
        bool compare(Speaker &speaker);
        //random handling
        //std::mt19937 gen = WRandGen::generator();

        std::shared_ptr<MeaningLoader> meaningLoader;

        int age = 0;

        //tags isolation and other ideas
        int tag = 0;


};

#endif