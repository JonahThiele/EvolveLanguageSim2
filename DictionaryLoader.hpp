#ifndef DICTIONARY_LOADER_HPP
#define DICTIONARY_LOADER_HPP

#include <vector>
#include "Word.hpp"
#include "pugixml.hpp"
#include "Word.hpp"
#include "Constants.h"
#include <regex>

class DictionaryLoader {

    public:
        DictionaryLoader(std::string dictionaryFile);

        //pass the dictionary to to a speaker
        std::vector<Word> getDictionary(){return dictionary;}

        //outputs the dictionary as an xml file
        void OutputDictionary();

    private:
        
        std::vector<Word> dictionary;
};

#endif