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
        //DictionaryLoader(std::string dictionaryFile);
        // use default constructor 
        //pass the dictionary to to a speaker

        std::vector<Word> getDictionary(){return dictionary;}

        //outputs the dictionary as an xml file
        void OutputDictionary(std::vector<Word> Speakerdictionary);

        //inputs the xml files to dictionaries
        void InputDictionary(std::string dictionaryFile);

    private:
        
        std::vector<Word> dictionary;
};

#endif