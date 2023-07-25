#ifndef WORD_HPP
#define WORD_HPP

#include <string>
#include <vector>
#include<bits/stdc++.h>
#include <cmath>
#include <memory>
#include "Constants.h"
#include  "WRandGen.hpp"
#include "LangSeed.hpp"
#include "MeaningLoader.hpp"

class Word {

    public:
        Word(const std::string &value, const std::string &meaning, const std::vector<std::string> &vowels, const std::string &baseWord, int prestige, bool negWord = false, std::string dialect = "Standard");


        //check the equality of another word to this one
        bool Equal(const Word &otherWord) const;

        bool isDummyWord() const;


        //getter methods one copy for const and other for local current object
        const std::string& getMeaning() const {return meaning;}
        std::string getMeaning() {return meaning;}
        const std::string& getValue() const {return value;}
        std::string getValue() {return value;}
        const std::string& getBaseWord() const {return baseWord;}
        std::string getBaseWord() {return baseWord;}
        const std::vector<std::string>& getVowels() const {return vowels;}
        std::vector<std::string> getVowels() {return vowels;}
        
        const int getPrestige() const{return prestigePoint;}
        int getPrestige() {return prestigePoint;}

        //Lexicology mutation methods (change the spelling of the words)
        Word LengthenVowel(const std::vector<std::string> &vowelPool) const;
        Word ShortenVowel(const std::vector<std::string> &vowelPool) const;
        Word DeleteVowel() const;

        Word AddSuffix(const std::vector<std::string> &suffixPool) const;
        Word AddPreffix(const std::vector<std::string> &preffixPool) const;

        Word Shrink(int start, int end) const;

        Word Mix(int start, int end, int otherWordStart, int otherWordEnd, const Word &otherWord) const;

        Word Negate(const std::vector<std::string> &negatePool) const;

        Word Subsitute(int start, int end, Word &otherWord, bool replace) const;

        Word CreateNew(int size, bool structuredGeneration) const;

        Word ClipEnd(int start) const;
        Word ClipFront(int end) const;

        Word Compound(const Word &otherWord) const;

        //meaning mutation (will need to connect to an API for this so it might be a much later feature)

        //increasing the meaning of the word
        Word Broadening(std::shared_ptr<MeaningLoader> meaningLoader) const;

        //decreasing the meaning of the word
        Word Narrowing(std::shared_ptr<MeaningLoader> meaningLoader) const;

        //100% different meaning
        Word ChangeMeaning(std::shared_ptr<MeaningLoader> meaningLoader) const;

        //increase the status of the word, lose its more negative connotations
        Word Ameliorate() const;

        //decrease the status of the word, lose its more positive connotations
        Word Pejorate() const;

        //change the word meaning to its complete opposite
        Word OppositeMeaning() const;
        
        std::string getDialect()const{ return dialect;}
        //overload operators for sorting purposes
        bool operator< (const Word &other) const {
            return prestigePoint < other.getPrestige();
        }

    private:

        mutable std::string meaning;
        mutable std::string value;
        mutable std::vector<std::string> vowels;
        mutable std::string baseWord;

        int prestigePoint;

        bool NegWord = false;
        std::string dialect;

        //take out the vowels that are no longer in the word
        std::vector<std::string> rebuildVowelList(const std::vector<std::string> &vowelList) const;

        //flags for debugging
        bool Rand_added_vowels = false;

};

#endif 
