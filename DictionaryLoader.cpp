#include "DictionaryLoader.hpp"

void DictionaryLoader::InputDictionary(std::string dictionaryFile)
{
    pugi::xml_document inputDictionary;

    dictionary.clear();

    //add ability to change input of this file
    inputDictionary.load_file(dictionaryFile.c_str());
    
    pugi::xml_node DictRoot = inputDictionary.child("Dictionary");


    for (pugi::xml_node word  = DictRoot.first_child(); word; word = word.next_sibling())
    {

        std::vector<std::string> tempVowels;
        std::string tempMeaning;
        std::string tempValue;
        std::string tempBaseWord;
        int prestige;

        for (pugi::xml_node wordValues = word.first_child(); wordValues; wordValues = wordValues.next_sibling())
        {
            std::string name = wordValues.name();
            if(name == XML_WORD_MEANING)
            {
                //handle for wchar mode, does that even make sense bc I'm not using the Win API?
                tempMeaning = wordValues.text().as_string();

            } else if( name == XML_WORD_STR)
            {
                tempValue = wordValues.text().as_string();

            } else if(name == XML_WORD_BASE) {
                tempBaseWord = wordValues.text().as_string();
            } else if( name == XML_PRESTIGE)
            {
                prestige = wordValues.text().as_int();
            }else if( name == XML_WORD_VOWELS)
            {

            // figure out stringing the string based on the ,'s found in the string

                std::stringstream sstr(wordValues.text().as_string());
                while(sstr.good())
                {
                    std::string substr;
                    getline(sstr, substr, ',');
                    tempVowels.push_back(substr);
                }
                
            } else 
            {
                // write some error that handles a poorly written xml file
                
            }
        }

        std::shared_ptr<Word> newWord = std::make_shared<Word>(Word(tempValue, tempMeaning, tempVowels, tempBaseWord, prestige));
        dictionary.emplace_back(newWord);
    }
}

void DictionaryLoader::OutputDictionary(std::vector<std::shared_ptr<Word>> Speakerdictionary)
{
    //figure out how to add header for xml that states version, etc

    pugi::xml_document outputDictionary;

    pugi::xml_node DictionaryNode = outputDictionary.append_child("Dictionary");

    for(auto &word : Speakerdictionary)
    {
        //add every word in the dictionary as a node
        pugi::xml_node dictionaryWord = DictionaryNode.append_child("Word");

        //take the value from the word class and add it as a node of the word in the xml
        pugi::xml_node wordValue = dictionaryWord.append_child("Value");
        wordValue.append_child(pugi::node_pcdata).set_value(word->getValue().c_str());

        pugi::xml_node wordVowels = dictionaryWord.append_child("Vowels");

        //concatenate the vector of strings to a string to set in the xml
        std::string xmlStr;
        for (const auto &vowel : word->getVowels()) xmlStr += (vowel + ",");
        wordVowels.append_child(pugi::node_pcdata).set_value(xmlStr.c_str());

        pugi::xml_node wordMeaning = dictionaryWord.append_child("Meaning");
        wordMeaning.append_child(pugi::node_pcdata).set_value(word->getMeaning().c_str());

    }
    
    //save and close the dictionary as a new xml file
    outputDictionary.save_file("outDictionary.xml");

}
