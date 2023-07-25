#include"Rule.hpp"

Rule::Rule(Speaker speaker,MeaningLoader meaningLoader, int suffixCount, int prefixCount, int negAmt):
speaker(std::make_unique<Speaker>(speaker)), meaningLoader(std::make_unique<MeaningLoader>(meaningLoader)){
    //set up the pointer to the speaker so it can be used by the other methods  
    
    addNewNegatePrefix(negAmt, true);
    addNewVowels();
    //set a starting constant for th prefixes and suffixes
    addNewPrefixes(10);
    addNewPrefixes(10);
    addNewCompoundableWords();
    addNewPrestigeMix();


}



void Rule::addNewNegatePrefix(int amount, bool intializing){
    
    //read through current list and take the prefix if the meaning has the not or negate flag


}


void Rule::addNewPrestigeMix(){

//get entire dictionary
prestigeWords = speaker->getDict();

//sort by descending order
sort( prestigeWords.begin( ), prestigeWords.end( ), [ ]( const Word& lhs, const Word& rhs )
{
   return lhs.getPrestige() > rhs.getPrestige();
});


//remove on the top 20 of prestige words 
std::vector<std::shared_ptr<Word>> modifiedPrestigeWords = { prestigeWords.begin(), prestigeWords.begin() + 20};

//set the 20 to the prestige words list
prestigeWords = modifiedPrestigeWords;

//perhaps remove to unique in the future?
}


void Rule::addNewVowels(){

    for(auto const word : speaker->getDict()){
   
            std::vector<std::string> tempVowelList =  word->getVowels();
   
            //reserve the size for the expansion
            this->vowels.reserve(vowels.size() + tempVowelList.size());
   
            vowels.insert(vowels.end(), tempVowelList.begin(), tempVowelList.end());
   
        }
   
       //remove the non-unique vowels
   
        std::sort(vowels.begin(), vowels.end());
   
       std::vector<std::string>::iterator it;
   
       it = std::unique(vowels.begin(), vowels.end());
     vowels.resize(std::distance(vowels.begin(),it));

}

void Rule::addNewCompoundableWords(){
    
    for(auto const word : speaker->getDict()){
   
                //replace with header constant
                if(word->getValue().size() < 7){
                    
                 compoundWords.push_back(std::move(word));
                
            }
   
            }
   
           //remove the non-unique vowels
   
            std::sort(compoundWords.begin(), compoundWords.end());
   
            auto it = std::unique(compoundWords.begin(), compoundWords.end());
         vowels.resize(std::distance(compoundWords.begin(),it));

}



void Rule::addNewPrefixes(int prefixCount){
        std::vector<std::shared_ptr<Word>> pickedPreWords = speaker->getDict();
        std::shuffle(pickedPreWords.begin(), pickedPreWords.end(), LangSeed::rng);
        for(int i = 0; i < prefixCount; i++){
   
            std::string wordStr =  pickedPreWords[i]->getValue();
  
           //use the random generator to grab first letters
               std::uniform_int_distribution<uint_least32_t> prefixSize( 1, 4);
   
            int prefixsize = prefixSize(LangSeed::rng);
  
            if(wordStr.size() <= prefixsize){
   
               prefixCount++;
                continue;
   
            } else {
                //needs to grab the from the suffix and prefix list for the final value
                prefixes.insert(std::pair<std::string, std::string>(wordStr.substr(0, prefixsize),meaningLoader->randPrefix()));
   
            }
   
   
        }
}


void Rule::addNewSuffixes(int suffixCount){
    
    std::vector<std::shared_ptr<Word>> pickedSufWords = speaker->getDict();
    std::shuffle(pickedSufWords.begin(), pickedSufWords.end(), LangSeed::rng);

         for(int i = 0; i < suffixCount; i++){
                std::string wordStr =  pickedSufWords[i]->getValue();
   
               //use the random generator to grab last  letters
                std::uniform_int_distribution<uint_least32_t> suffixSize( 1, 4);
   
                int suffixSizeGen = suffixSize(LangSeed::rng);
   
               if(wordStr.size() <= suffixSizeGen){
   
                    suffixCount++;
                    continue;
  
               } else {
                    //needs to grab the from the suffix and prefix list for the final value
                  suffixes.insert(std::pair<std::string, std::string>(wordStr.substr(wordStr.size() - suffixSizeGen),meaningLoader->randSuffix()));
            }
       }
}

