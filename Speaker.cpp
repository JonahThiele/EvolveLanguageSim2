#include "Speaker.hpp"

Speaker::~Speaker(){
    
    for(unsigned int i = 0; i < dictionary.size(); i++){
       dictionary[i].reset();
    } 

    meaningLoader.reset();
}


std::vector<std::string> Speaker::getPrefixes(){

    std::vector<std::string> prefixVals;
    for( auto const& val : prefixes)
        prefixVals.push_back(val.first);

    return prefixVals;
}


std::vector<std::string> Speaker::getSuffixes(){
    
    std::vector<std::string> suffixVals;
    for( auto const& val : suffixes)
        suffixVals.push_back(val.first);

    return suffixVals;

}
 void Speaker::addNewNegatePrefix(int amount, bool intializing){

      //read through current list and take the prefix if the meaning has the not or negate flag
     for( auto prefix : prefixes){
         if(prefix.second == "not"){
             std::string str = prefix.first;
            negatePrefixes.push_back(str);
         }
     }         

  }

void Speaker::addNewPrestigeMix(){

  //get entire dictionary
  prestigeWords = dictionary;

  //sort by descending order
  std::sort( prestigeWords.begin( ), prestigeWords.end( ), [ ]( auto lhs, auto rhs )
  {
     return lhs->getPrestige() > rhs->getPrestige();
  });


  //remove on the top 20 of prestige words
  std::vector<std::shared_ptr<Word>> modifiedPrestigeWords = { prestigeWords.begin(), prestigeWords.begin() + 20};

  //set the 20 to the prestige words list
  prestigeWords = modifiedPrestigeWords;

  //perhaps remove to unique in the future?
  }

void Speaker::addNewVowels(){

    for(auto word : dictionary){ 

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

void Speaker::addNewCompoundableWords(){

   for(auto word : dictionary){ 

                  //replace with header constant
                  if(word->getValue().size() < 7){
                  
                      auto comWord = word;
                   compoundWords.push_back(comWord);

              }

              }

             //remove the non-unique vowels

              std::sort(compoundWords.begin(), compoundWords.end());

              auto it = std::unique(compoundWords.begin(), compoundWords.end());
           vowels.resize(std::distance(compoundWords.begin(),it));
}


void Speaker::addNewPrefixes(int prefixCount){
          std::vector<std::shared_ptr<Word>> pickedPreWords = dictionary;
          std::shuffle(pickedPreWords.begin(), pickedPreWords.end(), LangSeed::rng);
          for(int i = 0; i < prefixCount; i++){

              std::string wordStr =  pickedPreWords[i]->getValue();

             //use the random generator to grab first letters
                 std::uniform_int_distribution<uint_least32_t> prefixSize( 1, 2);

              int prefixsize = prefixSize(LangSeed::rng);
           if(wordStr.size() <= prefixsize){
                
               //could write beyond the dictionary size causing issues,
               //need to add a check to the amount to make it not larger than the dictionary it's self
                // prefixCount++;
                  continue;

              } else {
                  //needs to grab the from the suffix and prefix list for the final value
                  prefixes.insert(std::pair<std::string, std::string>(wordStr.substr(0, prefixsize),meaningLoader->randPrefix()));

              }


          }
  }

void Speaker::addNewSuffixes(int suffixCount){

      std::vector<std::shared_ptr<Word>> pickedSufWords = dictionary;
      std::shuffle(pickedSufWords.begin(), pickedSufWords.end(), LangSeed::rng);

           for(int i = 0; i < suffixCount; i++){
                  std::string wordStr =  pickedSufWords[i]->getValue();

                 //use the random generator to grab last  letters
                  std::uniform_int_distribution<uint_least32_t> suffixSize( 1, 2);

                  int suffixSizeGen = suffixSize(LangSeed::rng);

              if(wordStr.size() <= suffixSizeGen){

                     // suffixCount++;
                      continue;

                 } else {
                      //needs to grab the from the suffix and prefix list for the final value
                    suffixes.insert({wordStr.substr(wordStr.size() - suffixSizeGen - 1), meaningLoader->randSuffix()});
              }
         }
  }

 

Speaker::Speaker(int x, int y, std::vector<std::shared_ptr<Word>> dictionary, int dictSize, std::shared_ptr<MeaningLoader> meaningLoaderIn, int tag, int SufCnt, int PreCnt, int NegCnt, std::string nativeDialect)
: meaningLoader(std::move(meaningLoaderIn))
{
    //makes sure the Speaker is within the bounds of the map
    if(x > MAX_X)
    {
        //set x to NULL and throw assert
        this->x = MAX_X;
    }

    if(x > MAX_Y)
    {
        //set y to NULL and throw assert
        this->y = MAX_Y;
    }
    
    this->x = x;
    this->y = y;

    this->dictionary = dictionary;

    this->dictSize = dictSize;

    this->tag = tag;
    this->nativeDialect = nativeDialect;

    addNewVowels();
    //set a starting constant for th prefixes and suffixes
    addNewPrefixes(10);
    addNewSuffixes(10);
    addNewCompoundableWords();
    addNewPrestigeMix();
    addNewNegatePrefix(10, true);
}

bool Speaker::increaseAge()
{
    if( age >= MAX_AGE)
    {
        return false;
    } else 
    {
        age++;
    }
    return true;
}

bool Speaker::compare(Speaker &otherSpeaker)
{
    int unsigned similarityCount = 0;
    if(otherSpeaker.getX() == x && otherSpeaker.getY() == y)
    {
        for(auto const &word : this->dictionary)
        {
            for(auto const &otherWord : otherSpeaker.getDict())
            {
                if(word->getValue() == otherWord->getValue())
                {
                    similarityCount++;
                }
            }
        }
        
        return (similarityCount == dictionary.size())? true : false;

    } else{
        return false;
    }
}

std::vector<std::shared_ptr<Word>> Speaker::speakToOtherPerson(Speaker otherPerson)
{
    std::vector<std::shared_ptr<Word>> sharedDictionary;
    
    //make sure other person is not self, the speakers can't exchange dictionaries and words with themselves
    if(this->compare(otherPerson))
    {
        return sharedDictionary; 
    }

    //set the percent shared with the other speaker
   
    std::uniform_int_distribution<uint_least32_t> distributePercentShare( 0,  99);

    //shuffle the dictionary to share different words each time
   // std::shuffle(std::begin(dictionary), std::end(dictionary), std::default_random_engine());

    std::sort( dictionary.begin( ), dictionary.end( ));
    //remove blank strings words from dictionaries
    for( unsigned int i = 0; i < dictionary.size(); i++)
    {
        if(dictionary[i]->getValue() == "" || dictionary[i]->getVowels().size() < 1)
        {
            dictionary.erase(dictionary.begin() + i);
            i--;
        }
    }
    
    int amntWordShared = (dictionary.size() - 1) * (float)((distributePercentShare(LangSeed::rng) + 1) / 100);

    for(int i = 0; i < amntWordShared; i++)
    {
        std::uniform_int_distribution<uint_least32_t> distPercentMutate( 0,  99);
        
        //prestige increases the chance of being spread
        if(distPercentMutate(LangSeed::rng) + 1 + dictionary[i]->getPrestige() >= PERCENT_SHARED_MUTATION)
        {
            // seed for the random different mutation methods ( 13 are currently defined )
            std::uniform_int_distribution<uint_least32_t> distWhichMutation( 0,  13); 

            //for debugging purposes I will be replacing this with a magic number
            
            int typeMutation = distWhichMutation(LangSeed::rng);
            switch(typeMutation)
            {
                case 0:
                    {
                  
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->LengthenVowel(vowels));

                    sharedDictionary.push_back(std::move(sharedWord));
  

                    }
                    break;
            
                case 1:
                    {
                    
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->ShortenVowel(vowels));
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;

                case 2:
                    {
                    //delete the vowel

                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->DeleteVowel());
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;
            
                case 3:
                    {
                    //add suffix
                     std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->AddSuffix(getSuffixes()));
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;

                case 4:
                    {
                    //add prefix 
                    
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->AddPreffix(getPrefixes()));
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;
            
                case 5:
                    {
                    // Kill
                    //set up a dummy word that indicates a kill val
                    std::vector<std::string> killList = {"KILL"};
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(Word("KILL", "KILL", killList, "KILL", 0));
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;

                case 6:
                    {
                    // shrink
                    //generate start and stop ints 
                    //check if work is less than 3 chars else return the normal word
                    if(dictionary[i]->getValue().size() >= 3)
                    {
                        std::uniform_int_distribution<uint_least32_t> distStart( 0,  dictionary[i]->getValue().size() - 2);
                        int start = distStart(LangSeed::rng);

                        //set it up so it is not inclusive
                        std::uniform_int_distribution<uint_least32_t> distEnd( start + 1,   dictionary[i]->getValue().size() - 1);
                        int end = distEnd(LangSeed::rng);

                        std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Shrink(start, end));
                        sharedDictionary.push_back(std::move(sharedWord));
                    }else 
                    {   
                        //create a new word
                        auto oldWord = dictionary[i];    
                        sharedDictionary.push_back(std::move(oldWord));
                    }
        
                    }
                    break;
            
                case 7:
                    {
                    //mix
                
                    std::uniform_int_distribution<uint_least32_t> distStart( 0,  dictionary[i]->getValue().length() - 2);
                    int start = distStart(LangSeed::rng);

                    std::uniform_int_distribution<uint_least32_t> distEnd( start,   dictionary[i]->getValue().length() - 1);
                    int end = distEnd(LangSeed::rng);

                    //grab random other word from the dictionary
                    std::uniform_int_distribution<uint_least32_t> distRandWord( 0,  dictionary.size() - 1);
                    Word otherWord = *dictionary[distRandWord(LangSeed::rng)];

                    //skip if it is a single or double char character
                    if(dictionary[i]->getValue().length() <= 3 || otherWord.getValue().length() <= 3)
                    {
                        sharedDictionary.push_back(dictionary[i]);
                    } else 
                    {
                        std::uniform_int_distribution<uint_least32_t> distOtherStart( 0, otherWord.getValue().length() - 2);
                        int otherStart = distOtherStart(LangSeed::rng);

                        std::uniform_int_distribution<uint_least32_t> distOtherEnd( start,   otherWord.getValue().length() - 1);
                        int otherEnd = distOtherEnd(LangSeed::rng);

                        std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Mix(start, end, otherStart, otherEnd, otherWord));
                        sharedDictionary.push_back(std::move(sharedWord)); 
                    }
                    }
                    break;
                case 8:
                    {
                    //negate
                    if(negatePrefixes.size() < 1){
                    
                        auto oldWord = dictionary[i];
                        sharedDictionary.push_back(std::move(oldWord));

                    }else{                 
                        std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Negate(negatePrefixes));
                        sharedDictionary.push_back(std::move(sharedWord));
                    }
                    }
                    break;
            
                case 9:
                    {

                        if(dictionary[i]->getValue().length() >= 4)
                        {
                            //subsitute
                            std::uniform_int_distribution<uint_least32_t> distStart( 0,  dictionary[i]->getValue().length() - 2);
                            int start = distStart(LangSeed::rng);

                            std::uniform_int_distribution<uint_least32_t> distEnd( start + 1,   dictionary[i]->getValue().length() - 1);
                            int unsigned end = distEnd(LangSeed::rng);

                            std::vector<std::shared_ptr<Word>> sizedWords; 
                            for(auto word : dictionary)
                            {
                                //sets miniumum size for the word to be subbed in
                                if(word && word->getValue().length() <= end && word->getValue().length() >= 1)
                                {
                                    auto copyWord = word;
                                    sizedWords.push_back(std::move(copyWord));
                                }
                            }

                            bool replaceFlag = true;

                            if(sizedWords.size())
                            {
                                std::uniform_int_distribution<uint_least32_t> distOtherSelect( 0,  sizedWords.size() - 1);
                                int otherWordIndex = distOtherSelect(LangSeed::rng);

                                std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Subsitute(start, end, *sizedWords[otherWordIndex], replaceFlag));
                                sharedDictionary.push_back(std::move(sharedWord)); 
                            }

                        } else 
                        {
                            std::shared_ptr<Word> sharedWord = std::make_shared<Word>( Word(dictionary[i]->getValue(), dictionary[i]->getMeaning(), dictionary[i]->getVowels(), dictionary[i]->getValue(), dictionary[i]->getPrestige()));
                            sharedDictionary.push_back(std::move(sharedWord));
                        }

                    }
                    break;
                case 10:
                    {
                     //create new

                     std::uniform_int_distribution<uint_least32_t> distStart( DEFAULT_NEW_WORD_SIZE_MIN,  DEFAULT_NEW_WORD_SIZE_MAX);
                     
                     std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->CreateNew(distStart(LangSeed::rng), true));
                     sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;
            
                case 11:
                    {
                     //clip end
                     // check if the word is one char then skip
                     if(dictionary[i]->getValue().length() < 2)
                     {
                        auto oldWord = dictionary[i];
                        sharedDictionary.push_back(std::move(oldWord));
                     } else 
                     {
                        std::uniform_int_distribution<uint_least32_t> distStart( dictionary[i]->getValue().length() - (int)(dictionary[i]->getValue().length() * 0.75), dictionary[i]->getValue().length() - 1);
                     
                        std::shared_ptr<Word> sharedWord = std:: make_shared<Word>(dictionary[i]->ClipEnd(distStart(LangSeed::rng)));
                        sharedDictionary.push_back(std::move(sharedWord));
                     }
                    }
                    break;
                case 12:
                    {
                     // make the length 1 if the quartering doesn't work
                     int end = 0;
                     if((int)(dictionary[i]->getValue().length() * 0.25) < 1)
                     {
                        end = 1;
                     } else {
                        end = (int)(dictionary[i]->getValue().length() * 0.25);
                     }
                     //clip front
                     std::uniform_int_distribution<uint_least32_t> distStart( 1, (int)(dictionary[i]->getValue().length() * 0.25));
                     std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->ClipFront(distStart(LangSeed::rng)));
                     sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;
            
                case 13:
                    {
                    //compound
                        std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Compound(*std::const_pointer_cast<const Word>(compoundWords[0])));
                        sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;

                case 14:
                    {
                    //broadening 
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Broadening(meaningLoader));
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;
            
                case 15:
                    {
                    //narrowing
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Narrowing(meaningLoader));
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;

                case 16:
                    {
                    //changing meaning
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->ChangeMeaning(meaningLoader));
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;
            
                case 17:
                    {
                    //Ameliorate
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Ameliorate());
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;

                case 18:
                    {
                    //Pejorate 
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Pejorate());
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;
            
                case 19:
                    {
                    //Opposite meaning
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->OppositeMeaning());
                    sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;

                default:
                    {
                    
                     //create pesudo null object to check in the learning function
                     std::vector<std::string> dummyList = {"DEFAULT"};
                     std::shared_ptr<Word> sharedWord = std::make_shared<Word>(Word("DEFAULT", "DEFAULT", dummyList, "DEFAULT", 0));

                     sharedDictionary.push_back(std::move(sharedWord));
                    }
                    break;
                    dictionary[i]->Ameliorate();
            }

        } else {
            //just return normal
            dictionary[i]->Pejorate();
            auto oldWord = dictionary[i];
            sharedDictionary.push_back(std::move(oldWord));

        }
    }
    addNewVowels();
    return sharedDictionary; 

}

// perhaps remove low prestige words from the dictionary to make room for the new transferred words
void Speaker::cutWords(int dictionaryWordCap)
{
    //order again to make sure the high prestige words are the least likely to become forgotten or not transferred
    std::sort( dictionary.begin( ), dictionary.end( ));

    while(dictionary.size() > (unsigned)dictionaryWordCap)
    {
        std::uniform_int_distribution<uint_least32_t> distCut( 0, dictionary.size());
        dictionary.erase(dictionary.begin() + distCut(LangSeed::rng));
    }
}

void Speaker::move()
{
    std::uniform_int_distribution<uint_least32_t> distMoveX( -24, 24);
    std::uniform_int_distribution<uint_least32_t> distMoveY( -24, 24);
    int addx = distMoveX(LangSeed::rng);
    int addy = distMoveY(LangSeed::rng);
    if(this->x + addx  > 100 or this->x + addx < 100)
    {
        this->x += distMoveX(LangSeed::rng);
    }
    if(this->y + addy > 100 or this->y + addy < 100)
    {
        this->y += distMoveY(LangSeed::rng);
    }
}

void Speaker::learnWords(std::vector<std::shared_ptr<Word>> sharedWords)
{
    if(sharedWords.size() != 0){

    cutWords(dictSize);
    //remove complete repeat words 
    for( unsigned int i = 0; i < dictionary.size(); i++)
    {
        for( unsigned int b = 0; b < sharedWords.size(); b++)
        {
            if(dictionary[i]->Equal(*sharedWords[b]) || sharedWords[b]->isDummyWord())
            {
                
                sharedWords.erase(sharedWords.begin() + b);
                continue;
            }

        }

    } 
    dictionary.insert( dictionary.end(), sharedWords.begin(), sharedWords.end() );
    } 

}
