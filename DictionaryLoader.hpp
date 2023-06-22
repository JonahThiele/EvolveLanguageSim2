#ifndef DICTIONARY_LOADER_HPP
#define DICTIONARY_LOADER_HPP

#include <vector>
#include "Word.hpp"
#include "pugixml.hpp"
#include "Word.hpp"
#include "Constants.h"
#include <regex>
#include <memory>

class DictionaryLoader {

    public:
        //DictionaryLoader(std::string dictionaryFile);
        // use default constructor 
        //pass the dictionary to to a speaker

        std::vector<std::shared_ptr<Word>> getDictionary(){return dictionary;}

        //outputs the dictionary as an xml file
        void OutputDictionary(std::vector<std::shared_ptr<Word>> Speakerdictionary);

        //inputs the xml files to dictionaries
        void InputDictionary(std::string dictionaryFile);

    private:
        
        std::vector<std::shared_ptr<Word>> dictionary;
};

#endif
