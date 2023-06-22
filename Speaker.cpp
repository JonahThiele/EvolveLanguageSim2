#include "Speaker.hpp"


Speaker::Speaker(int x, int y, std::vector<std::shared_ptr<Word>> dictionary, int dictSize, std::shared_ptr<MeaningLoader> meaningLoaderIn, int tag)
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

    //WRandGen::setUpgenerator();

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

std::vector<std::shared_ptr<Word>> Speaker::speakToOtherPerson(Speaker & otherPerson)
{
    std::vector<std::shared_ptr<Word>> sharedDictionary;
    
    //make sure other person is not self, the speakers can't exchange dictionaries and words with themselves
    if(this->compare(otherPerson))
    {
        return sharedDictionary; 
        std::cout << "returning nothing\n";
    }

    //set the percent shared with the other speaker
   
    std::uniform_int_distribution<uint_least32_t> distributePercentShare( 0,  99);

    int amntWordShared = dictionary.size() * (float)((distributePercentShare(LangSeed::rng) + 1) / 100);

    //shuffle the dictionary to share different words each time
    std::shuffle(std::begin(dictionary), std::end(dictionary), std::default_random_engine());

    std::vector<std::shared_ptr<Word>> tempDictionary = dictionary;
    /*
    for(auto const &word : tempDictionary)
    {
        if(word->getPrestige() > PRESTIGE_THRESHOLD)
            dictionary.insert(dictionary.begin(), word);

    }*/

    //remove blank strings words from dictionaries
    for( unsigned int i = 0; i < dictionary.size(); i++)
    {
        if(dictionary[i]->getValue() == "")
        {
            dictionary.erase(dictionary.begin() + i);
        }
    }

    for(int i = 0; i < amntWordShared; i++)
    {
        std::uniform_int_distribution<uint_least32_t> distPercentMutate( 0,  99);

        if(distPercentMutate(LangSeed::rng) + 1 <= PERCENT_SHARED_MUTATION)
        {
            // seed for the random different mutation methods ( 13 are currently defined )
            std::uniform_int_distribution<uint_least32_t> distWhichMutation( 0,  13); 

            //for debugging purposes I will be replacing this with a magic number
            
            int typeMutation = distWhichMutation(LangSeed::rng);
            switch(typeMutation)
            {
                case 0:
                    {
                    //lengthen the vowels
                    std::vector<std::string> vowelList = {};
                    //grab the vowel pool from the dictionary
                    for(auto const &word : dictionary)
                    {
                        
                        //vowelList.insert(vowelList.end(), word.getVowels().begin(), word.getVowels().begin() + word.getVowels().size());
                        //insert was broken so I reverted to the naive way and just used a simple loop
                        for(std::string vowel : word->getVowels())
                        {
                            vowelList.push_back(vowel);
                        }
                    }

                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->LengthenVowel(vowelList));
                    sharedDictionary.emplace_back(std::move(sharedWord));
  

                    }
                    break;
            
                case 1:
                    {
                    
                    //shorten the vowels 
                    std::vector<std::string> vowelList = {};
                    //grab the vowel pool from the dictionary
                    for(auto const &word : dictionary)
                    {
                        //vowelList.insert(vowelList.end(), word.getVowels().begin(), word.getVowels().begin() + word.getVowels().size());
                        
                        for(std::string vowel : word->getVowels())
                        {
                            vowelList.push_back(vowel);
                        }
                    }

                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->ShortenVowel(vowelList));
                    sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;

                case 2:
                    {
                    //delete the vowel
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->DeleteVowel());
                    sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;
            
                case 3:
                    {
                    //add suffix
                    std::vector<std::string> suffixList = {};
                    //grab the suffix pool from the dictionary

                    for(auto const &word : dictionary)
                    {
                        //check if the word exists at all and is not empty before creating
                        //suffix should not be greater than 25% of the word
                        if(word->getValue().length())
                        {
                            std::uniform_int_distribution<uint_least32_t> distSuffix((int)(word->getValue().length() * 0.75), (int)(word->getValue().length() - 1));
                            int lengthOfSuffix = distSuffix(LangSeed::rng);
                            //the substr reads until the end of the string
                            suffixList.push_back(word->getValue().substr(word->getValue().length() - lengthOfSuffix));
                        }
                    }

                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->AddSuffix(suffixList));
                    sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;

                case 4:
                    {
                    //add prefix 
                    std::vector<std::string> prefixList = {};
                    //grab the suffix pool from the dictionary

                    for(auto &word : dictionary)
                    {
                        if(word->getValue().length())
                        {
                            //suffix should not be greater than 25% of the word
                            int bParam = ceil((int)(word->getValue().length() * 0.25));
                            std::uniform_int_distribution<uint_least32_t> distPrefix( 0,  bParam);
                            int lengthOfPrefix = distPrefix(LangSeed::rng);
                            //the substr reads until the end of the string
                            prefixList.push_back(word->getValue().substr(word->getValue().length() - lengthOfPrefix));
                        }
                       
                    }

                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->AddPreffix(prefixList));
                    sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;
            
                case 5:
                    {
                    // Kill
                    //set up a dummy word that indicates a kill val
                    std::vector<std::string> killList = {"KILL"};
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(Word("KILL", "KILL", killList, "KILL", 0));
                    sharedDictionary.emplace_back(std::move(sharedWord));
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

                        std::uniform_int_distribution<uint_least32_t> distEnd( start,   dictionary[i]->getValue().size() - 1);
                        int end = distEnd(LangSeed::rng);

                        std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Shrink(start, end));
                        sharedDictionary.emplace_back(std::move(sharedWord));
                    }else 
                    {   
                        //create a new word
                        
                        sharedDictionary.emplace_back(std::move(dictionary[i]));
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
                        sharedDictionary.emplace_back(dictionary[i]);
                    } else 
                    {
                        std::uniform_int_distribution<uint_least32_t> distOtherStart( 0, otherWord.getValue().length() - 2);
                        int otherStart = distOtherStart(LangSeed::rng);

                        std::uniform_int_distribution<uint_least32_t> distOtherEnd( start,   otherWord.getValue().length() - 1);
                        int otherEnd = distOtherEnd(LangSeed::rng);

                        std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Mix(start, end, otherStart, otherEnd, otherWord));
                        sharedDictionary.emplace_back(std::move(sharedWord)); 
                    }
                    }
                    break;
                case 8:
                    {
                    //negate
                     std::vector<std::string> negateList = {};
                    //grab the negation prefix pool from the dictionary

                    for(auto const &word : dictionary)
                    {
                        //prefixfix should not be greater than three chars
                        std::uniform_int_distribution<uint_least32_t> distNegPrefix( 0,  2);
                        int lengthOfNegPrefix = distNegPrefix(LangSeed::rng);
                        //the substr reads until the end of the string
                        int wordLen = word->getValue().length();
                        if((wordLen - lengthOfNegPrefix) > 0){

                            negateList.push_back(word->getValue().substr(word->getValue().length() - lengthOfNegPrefix -1));
                        }
                        
                    }

                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Negate(negateList));
                    sharedDictionary.emplace_back(std::move(sharedWord));

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
                            for(auto const &word : dictionary)
                            {
                                //sets miniumum size for the word to be subbed in
                                if(word->getValue().length() <= end && word->getValue().length() >= 1)
                                {
                                    sizedWords.push_back(std::move(word));
                                }
                            }

                            bool replaceFlag = true;

                            if(sizedWords.size())
                            {
                                std::uniform_int_distribution<uint_least32_t> distOtherSelect( 0,  sizedWords.size() - 1);
                                int otherWordIndex = distOtherSelect(LangSeed::rng);

                                std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Subsitute(start, end, *sizedWords[otherWordIndex], replaceFlag));
                                sharedDictionary.emplace_back(std::move(sharedWord)); 
                            }

                        } else 
                        {
                            std::shared_ptr<Word> sharedWord = std::make_shared<Word>( Word(dictionary[i]->getValue(), dictionary[i]->getMeaning(), dictionary[i]->getVowels(), dictionary[i]->getValue(), dictionary[i]->getPrestige()));
                            sharedDictionary.emplace_back(std::move(sharedWord));
                        }

                    }
                    break;
                case 10:
                    {
                     //create new

                     std::uniform_int_distribution<uint_least32_t> distStart( DEFAULT_NEW_WORD_SIZE_MIN,  DEFAULT_NEW_WORD_SIZE_MAX);
                     
                     std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->CreateNew(distStart(LangSeed::rng), true));
                     sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;
            
                case 11:
                    {
                     //clip end
                     // check if the word is one char then skip
                     if(dictionary[i]->getValue().length() < 2)
                     {
                        sharedDictionary.emplace_back(std::move(dictionary[i]));
                     } else 
                     {
                        std::uniform_int_distribution<uint_least32_t> distStart( dictionary[i]->getValue().length() - (int)(dictionary[i]->getValue().length() * 0.75), dictionary[i]->getValue().length() - 1);
                     
                        std::shared_ptr<Word> sharedWord = std:: make_shared<Word>(dictionary[i]->ClipEnd(distStart(LangSeed::rng)));
                        sharedDictionary.emplace_back(std::move(sharedWord));
                     }
                    }
                    break;
                case 12:
                    {
                     // make the length 1 if the quartering doesn't work
                     int end;
                     if((int)(dictionary[i]->getValue().length() * 0.25) < 1)
                     {
                        end = 1;
                     } else {
                        end = (int)(dictionary[i]->getValue().length() * 0.25);
                     }
                     //clip front
                     std::uniform_int_distribution<uint_least32_t> distStart( 1, (int)(dictionary[i]->getValue().length() * 0.25));
                     std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->ClipFront(distStart(LangSeed::rng)));
                     sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;
            
                case 13:
                    {
                    //compound
                        std::uniform_int_distribution<uint_least32_t> distRandWord( 0,  dictionary.size() - 1);
                        Word otherWord = *dictionary[distRandWord(LangSeed::rng)];

                        std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Compound(otherWord));
                        sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;

                case 14:
                    {
                    //broadening 
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Broadening(meaningLoader));
                    sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;
            
                case 15:
                    {
                    //narrowing
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Narrowing(meaningLoader));
                    sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;

                case 16:
                    {
                    //changing meaning
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->ChangeMeaning(meaningLoader));
                    sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;
            
                case 17:
                    {
                    //Ameliorate
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Ameliorate());
                    sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;

                case 18:
                    {
                    //Pejorate 
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->Pejorate());
                    sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;
            
                case 19:
                    {
                    //Opposite meaning
                    std::shared_ptr<Word> sharedWord = std::make_shared<Word>(dictionary[i]->OppositeMeaning());
                    sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;

                default:
                    {
                    
                     //create pesudo null object to check in the learning function
                     std::vector<std::string> dummyList = {"DEFAULT"};
                     std::shared_ptr<Word> sharedWord = std::make_shared<Word>(Word("DEFAULT", "DEFAULT", dummyList, "DEFAULT", 0));

                     sharedDictionary.emplace_back(std::move(sharedWord));
                    }
                    break;
            }

        } else {
            //just return normal
            sharedDictionary.emplace_back(std::move(dictionary[i]));
        }
    }

    return sharedDictionary; 

}

void Speaker::cutWords(int dictionaryWordCap)
{
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
    cutWords(dictSize);
    //remove complete repeat words 
    for( unsigned int i = 0; i < dictionary.size(); i++)
    {
        for( unsigned int b = 0; b < sharedWords.size(); b++)
        {
            if(dictionary[i]->Equal(*sharedWords[b]) || sharedWords[b]->isDummyWord())
            {
                //b--;
                sharedWords.erase(sharedWords.begin() + b);
                continue;
            }

        }

    }
    
    if(sharedWords.size() > 0)
    {
        //put filtered words into the dictionary
        dictionary.insert( dictionary.end(), sharedWords.begin(), sharedWords.end() ); 
        
    }

}
