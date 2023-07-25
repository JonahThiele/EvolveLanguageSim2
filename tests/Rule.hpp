#ifndef RULES_HPP
#define RULES_HPP
#pragma once 
#include<vector>
#include<string>
#include<map>
#include<memory>
#include<algorithm>
#include<iterator>
#include<random>
#include"Word.hpp"
#include"Speaker.hpp"
#include"MeaningLoader.hpp"
#include<cstdlib>
#include<iostream>
#include<time.h>



class Rule {
    public:
        
        //those with the highest prestige point are going to be mixed and subbed
        
        Rule(Speaker speaker ,MeaningLoader meaningLoader, int suffixCount, int prefixCount, int negAmt);
            

        std::vector<std::shared_ptr<Word>> get_prestigeMix();
        void addNewPrestigeMix();

        std::vector<std::shared_ptr<Word>> get_compoundableWords(){ return compoundWords;}
        void addNewCompoundableWords();

        std::vector<std::string> get_suffixes(){
            std::vector<std::string> suffixVec;
            for(const auto &suffix : suffixes)
                suffixVec.push_back(suffix.first);
            return suffixVec;
        }

        void addNewSuffixes(int amount);
        
        std::vector<std::string> get_prefixes(){
            std::vector<std::string> prefixVec;
            for(const auto &prefix : prefixes)
                prefixVec.push_back(prefix.first);
            return prefixVec;
        }
        void addNewPrefixes(int amount);
        
        std::vector<std::string> get_vowels(){ return vowels;}
        void addNewVowels();

        std::vector<std::string> get_negatePrefix(){ return negatePrefixes; };
        void addNewNegatePrefix(int amount, bool intializing);


    private:
            std::unique_ptr<Speaker> speaker;
            std::unique_ptr<MeaningLoader> meaningLoader;
            std::vector<std::string> vowels;
            std::map<std::string, std::string> suffixes;
            std::map<std::string, std::string> prefixes;
            std::vector<std::string> negatePrefixes;
            std::vector<std::shared_ptr<Word>> compoundWords;
            std::vector<std::shared_ptr<Word>> prestigeWords;
};

#endif
