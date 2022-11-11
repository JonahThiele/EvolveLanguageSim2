#include "DictionaryLoader.hpp"

DictionaryLoader::DictionaryLoader(std::string dictionaryFile)
{
    pugi::xml_document inputDictionary;

    //add ability to change input of this file
    inputDictionary.load_file(dictionaryFile.c_str());
    
    pugi::xml_node DictRoot = inputDictionary.child("Dictionary");


    for (pugi::xml_node word  = DictRoot.first_child(); word; word = word.next_sibling())
    {

        std::vector<std::string> tempVowels;
        std::string tempMeaning;
        std::string tempValue;

        for (pugi::xml_node wordValues = word.first_child(); wordValues; wordValues = wordValues.next_sibling())
        {
            if(wordValues.name() == XML_WORD_MEANING)
            {
                //handle for wchar mode, does that even make sense bc I'm not using the Win API?
                tempMeaning = wordValues.text().as_string();

            } else if( wordValues.name() == XML_WORD_STR)
            {
                tempValue = wordValues.text().as_string();

            } else if( wordValues.name() == XML_WORD_VOWELS)
            {

            // figure out stringing the string based on the ,'s found in the string

                std::string tempCommaSeperatedStr = wordValues.text().as_string();
                std::regex regexz("[^,]+ ");
                std::sregex_token_iterator start(tempCommaSeperatedStr.begin(), tempCommaSeperatedStr.end(), regexz, -1);
                std::sregex_token_iterator end;

                tempVowels.insert(tempVowels.begin(), start, end);
                
            } else 
            {
                // write some error that handles a poorly written xml file
            }
        }

        Word newWord = Word(tempValue, tempMeaning, tempVowels);
        dictionary.push_back(newWord);
    }
}

void DictionaryLoader::OutputDictionary()
{
    //figure out how to add header for xml that states version, etc

    pugi::xml_document outputDictionary;

    pugi::xml_node DictionaryNode = outputDictionary.append_child("Dictionary");

    for(Word word : dictionary)
    {
        //add every word in the dictionary as a node
        pugi::xml_node dictionaryWord = DictionaryNode.append_child("Word");

        //take the value from the word class and add it as a node of the word in the xml
        pugi::xml_node wordValue = dictionaryWord.append_child("Value");
        wordValue.append_child(pugi::node_pcdata).set_value(word.getValue().c_str());

        pugi::xml_node wordVowels = dictionaryWord.append_child("Vowels");

        //concatenate the vector of strings to a string to set in the xml
        std::string xmlStr;
        for (const auto &vowel : word.getVowels()) xmlStr += vowel;
        wordValue.append_child(pugi::node_pcdata).set_value(word.getValue().c_str());

        pugi::xml_node wordMeaning = dictionaryWord.append_child("Meaning");
        wordValue.append_child(pugi::node_pcdata).set_value(word.getMeaning().c_str());

    }
    
    //save and close the dictionary as a new xml file
    outputDictionary.save_file("outDictionary.xml");

}