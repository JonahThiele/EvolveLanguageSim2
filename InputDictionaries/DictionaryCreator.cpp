//edit to include the right headers
#include"pugixml.hpp"
#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<algorithm>


char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    
    char ** itr = std::find(begin, end, option);
    
    if (itr != end && ++itr != end)
    {
        return *itr;
    }    
   
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
       return std::find(begin, end, option) != end;
}


int parseInt(const std::string &s) { std::size_t pos; int result = std::stoi(s, &pos); if (pos != s.size()) throw std::runtime_error("can't parse integer"); return result; }



int main(int argc, char** argv){
          
   //if -h flag is selected the program doesn't try to run 
    if(cmdOptionExists(argv, argv+argc, "-h")){
        std::cout << "This is the intial dictionary creator:\n";
        std::cout << "-m Max Word Length\n";
        std::cout << "-a Amount of Words\n";
        std::cout << "-f Filename for output dictionary\n";
    
    }else{
    
        int MaxWordLen, AmountofWords;
        std::string filename;
        
        //setting defaults if no flag is specificed, need to handle none number here so it doesn't crash
        if(cmdOptionExists(argv, argv+argc, "-m")){
            
            MaxWordLen = parseInt(getCmdOption(argv, argv + argc, "-m"));

        }else{
        
            MaxWordLen = 10;
        
        }

        if(cmdOptionExists(argv, argv+argc, "-a")){
            
            AmountofWords = parseInt(getCmdOption(argv, argv+argc, "-a"));

        
        }else{

            AmountofWords = 20;
        }

        
        if(cmdOptionExists(argv, argv+argc, "-f")){

            filename = getCmdOption(argv, argv+argc, "-f");
        }else{

            filename = "Dict.xml";
        }


    //handling the options through popt

    //open up the meaning list and create a vector of the words
    std::ifstream input("./InputDictionaries/words.txt");

    std::vector<std::string> wordList;



    for(std::string line; getline(input, line);){
        //one word per line this should work        
        wordList.push_back(line);
    }
    
    //cut down the word list to the top 500 words so less obscure words are used
    wordList = {wordList.begin(), wordList.begin() + 500};
    //handle the arguments

    //hard coding the different dialect options right now and will rewrite them later to generate on the fly
    std::string dialects[] = {"Nokain", "Churst", "Barql", "Pemvost"};
    
    pugi::xml_document doc;

    auto declarationNode = doc.append_child(pugi::node_declaration);
    declarationNode.append_attribute("version")    = "1.0";
    declarationNode.append_attribute("encoding")   = "ISO-8859-1";
    declarationNode.append_attribute("standalone") = "yes";    
    
    //what is the root called?
    auto root = doc.append_child("Dictionary");
    
    std:: srand((unsigned) time(NULL));   
    
    for( int i = 0; i <  AmountofWords; i++){
       
        //code to generate the random words I don't want to have to deal with 
        //passing rand back and forth from a function    
       
       int lastVowel = 0;
       std::vector<char>  consonants = { 'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'y', 'z'};
       std::vector<char> vowels = {'a', 'e', 'i', 'o', 'u'};
       std::string word;
       std::vector<char> wordVowels;
        
       int randLen = 1 + (std::rand() % MaxWordLen);   
       for(int b = 0; b < randLen;b++){

           char letter;
           
           //0 - 2 char consonants between every vowel
           int vowelOffset = std::rand() % 8;

           if(b - lastVowel <= vowelOffset){
               //mandatory vowel
               letter = consonants[std::rand() % consonants.size()];
           }else{
               letter = vowels[std::rand() %  vowels.size()];
               wordVowels.push_back(letter);
           }

           word += letter;
       }
      
      std ::sort( wordVowels.begin(), wordVowels.end() );
      wordVowels.erase(std::unique(wordVowels.begin(), wordVowels.end()), wordVowels.end()); 
     
      //convert the vowel vector into a string 
       std::string vowelsStr;
       for(char letter : wordVowels){
           vowelsStr += letter;
           vowelsStr += ',';
       }
        
      


       vowelsStr.erase(vowelsStr.begin() + vowelsStr.size() - 2, vowelsStr.end());

       //create random prestige point
       int prestige = std::rand() %  100;

       std::string meaning = wordList[std::rand() % wordList.size()];
       
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
       
       pugi::xml_node Dialect = Word.append_child("Dialect");
       Dialect.append_child(pugi::node_pcdata).set_value(dialects[std::rand() & 3].c_str());

       doc.save_file(filename.c_str());
    
    }
    
  }

    return 0;

}

