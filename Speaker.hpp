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
        Speaker(int x, int y, std::vector<std::shared_ptr<Word>>, int dictSize, std::shared_ptr<MeaningLoader> meaningLoaderIn, int tag, int SufCnt, int PreCnt, int NegCnt, std::string nativeDialect);
        
        
        ~Speaker();
        //interact with other person and share % of the dictionary
        std::vector<std::shared_ptr<Word>> speakToOtherPerson(Speaker otherPerson);

        //accept a part of another speaker's dictionary
        void learnWords(std::vector<std::shared_ptr<Word>> sharedWords);
        void cutWords(int dictionaryCap);

        void move();

        //
        std::vector<std::shared_ptr<Word>> getDict(){ return dictionary;}

        int getY(){return y;}
        int getX(){return x;}

        int getTag(){return tag;}
        
        bool increaseAge();
        
        std::string getDialect()const { return nativeDialect;}

    private:

        std::vector<std::string> getPrefixes();
        std::vector<std::string> getSuffixes();
        
        void addNewPrestigeMix();

        void addNewCompoundableWords();

        void addNewSuffixes(int amount);

        void addNewPrefixes(int amount);

        void addNewVowels();

        void addNewNegatePrefix(int amount, bool intializing);


        //word handle or pointer
        int y;
        int x;

        //size of dictionary
        int dictSize;

        //the internal dictionary exchanged by the speakers
        std::vector<std::shared_ptr<Word>> dictionary;
        
        bool compare(Speaker &speaker);
        //random handling

        std::shared_ptr<MeaningLoader> meaningLoader;
        
        std::vector<std::string> vowels;
        
        std::map<std::string, std::string> suffixes;
        std::map<std::string, std::string> prefixes;
        std::vector<std::string> negatePrefixes;
        std::vector<std::shared_ptr<Word>> compoundWords;
        std::vector<std::shared_ptr<Word>> prestigeWords;

        int age = 0;

        //tags isolation and other ideas
        int tag = 0;
        
        std::string nativeDialect;
};

#endif
