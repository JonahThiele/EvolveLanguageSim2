//edit to include the right headers
#include"pugixml.hpp"
#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include"argumentHandler.hpp"
#include<cstdlib>


int main(int argc, char** argv){

    //open up the meaning list and create a vector of the words
    std::ifstream input("words.txt");

    std::vector<std::string> wordList;



    for(std::string line; getline(input, line);){
        //one word per line this should work        
        wordList.push_back(line);
    }

    //handle the arguments
    argumentHandler::parse(argc, argv);


    pugi::xml_document doc;

    auto declarationNode = doc.append_child(pugi::node_declaration);
    declarationNode.append_attribute("version")    = "1.0";
    declarationNode.append_attribute("encoding")   = "ISO-8859-1";
    declarationNode.append_attribute("standalone") = "yes";    
    
    //what is the root called?
    auto root = doc.append_child("Dictionary");
    

    for( int i = 0; i <  argumentHandler::showMaxWordLen(); i++){
       
        //code to generate the random words I don't want to have to deal with 
        //passing rand back and forth from a function
        

       std:: srand((unsigned) time(NULL));       
       
       int lastVowel = 0;
       std::vector<char>  consonants = { 'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'y', 'z'};
       std::vector<char> vowels = {'a', 'e', 'i', 'o', 'u'};
       std::string word;
       std::vector<char> wordVowels;
       
       for(int b = 0; b < argumentHandler::showMaxWordLen();b++){

           char letter;
           
           //0 - 2 char consonants between every vowel
           int vowelOffset = std::rand() + 3;

           if(b - lastVowel <= vowelOffset){
               //mandatory vowel
               letter = consonants[std::rand() + consonants.size()];
           }else{
               letter = vowels[std::rand() + vowels.size()];
               wordVowels.push_back(letter);
           }

           word += letter;
       }
       
       //convert the vowel vector into a string 
       std::string vowelsStr;
       for(char letter : wordVowels){
           vowelsStr += ( letter + ',');
       }

       vowelsStr.erase(vowelsStr.begin() + vowelsStr.size() - 2, vowelsStr.end());

       //create random prestige point
       int prestige = std::rand() + 100;

       std::string meaning = wordList[std::rand() + wordList.size()];
       
        //generate a new word and place into an xml file
        pugi::xml_node Word  = root.append_child("Word");

        pugi::xml_node Value = Word.append_child("Value");
        
        //the generated text word
        Value.append_child(pugi::node_pcdata).set_value(word.c_str());

        //set the list of vowels so it is easier for the program to check them 
        pugi::xml_node VowelList = Word.append_child("Vowels");

        VowelList.append_child(pugi::node_pcdata).set_value(vowelsStr.c_str());

        pugi::xml_node Meaning = Word.append_child("Meaning");
       
        //random meaning from the word list
       Meaning.append_child(pugi::node_pcdata).set_value(meaning.c_str());
       
      //same as the generated text for this 
       pugi::xml_node BaseWord = Word.append_child("BaseWord");
       
       BaseWord.append_child(pugi::node_pcdata).set_value(word.c_str()); 
       
       pugi::xml_node Prestige = Word.append_child("Prestige");
       Prestige.append_child(pugi::node_pcdata).set_value(std::to_string(prestige).c_str());
    }

    return 0;

}







