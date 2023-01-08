#ifndef WORD_HPP
#define WORD_HPP

#include <string>
#include <vector>
#include<bits/stdc++.h>
#include <cmath>
#include "Constants.h"
#include  "WRandGen.hpp"
#include "LangSeed.hpp"

class Word {

    public:
        Word(std::string value, std::string meaning, std::vector<std::string> vowels);


        //check the equality of another word to this one
        bool Equal(Word otherWord);

        bool isDummyWord();


        //getter methods
        std::string getMeaning() {return meaning;}
        std::string getValue() {return value;}
        std::string getBaseWord() {return baseWord;}
        std::vector<std::string> getVowels() {return vowels;}

        //Lexicology mutation methods (change the spelling of the words)
        Word LengthenVowel(std::vector<std::string> vowelPool);
        Word ShortenVowel(std::vector<std::string> vowelPool);
        Word DeleteVowel();

        Word AddSuffix(std::vector<std::string> suffixPool);
        Word AddPreffix(std::vector<std::string> preffixPool);

        void Kill();

        Word Shrink(int start, int end);

        Word Mix(int start, int end, int otherWordStart, int otherWordEnd, Word otherWord);

        Word Negate(std::vector<std::string> negatePool);

        Word Subsitute(int start, int end, Word otherWord, bool replace);

        Word CreateNew(int size, bool structuredGeneration);

        Word ClipEnd(int start);
        Word ClipFront(int end);

        Word Compound(Word otherWord);

        //meaning mutation (will need to connect to an API for this so it might be a much later feature)

        //increasing the meaning of the word
        Word Broadening();

        //decreasing the meaning of the word
        Word Narrowing();

        //100% different meaning
        Word ChangeMeaning();

        //increase the status of the word, lose its more negative connotations
        Word Ameliorate();

        //decrease the status of the word, lose its more positive connotations
        Word Pejorate();

        //change the word meaning to its complete opposite
        Word OppositeMeaning();

    private:

        std::string meaning;
        std::string value;
        std::vector<std::string> vowels;
        std::string baseWord;

        //random generator
        //std::mt19937 gen = WRandGen::generator();

        //take out the vowels that are no longer in the word
        std::vector<std::string> rebuildVowelList(std::vector<std::string> vowelList);

        //flags for debugging
        bool Rand_added_vowels = false;

};

#endif 