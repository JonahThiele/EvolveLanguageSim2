#include "Word.hpp"

Word::Word(std::string value, std::string meaning, std::vector<std::string> InVowels)
{
    this->value = value;
    this->meaning = meaning;
    this->vowels = rebuildVowelList(InVowels);
}


bool Word::Equal(Word otherWord)
{
    if(otherWord.getValue() == this->value && otherWord.getMeaning() == this->meaning && otherWord.getVowels() == this->vowels)
    {
        return true;
    }
    
    return false;
}

bool Word::isDummyWord()
{
    std::string IgnoreList[4] = {"KILL", "DEFAULT", "", " "};
    for(int i = 0; i < 4; i++)
    {
        if(value == IgnoreList[i] || meaning == IgnoreList[i] || vowels[0] == IgnoreList[i])
        {
            return true;
        }
    }

    return false;
}

Word Word::LengthenVowel(std::vector<std::string> vowelPool)
{
    std::vector<std::string> LongVowelPool;
    for( std::string vowel : vowels)
    {
        for(std::string poolVowel : vowelPool)
        {
            if(poolVowel.size() >= vowel.size())
            {
                LongVowelPool.push_back(poolVowel);
            } else if( poolVowel.size() <= vowel.size() && vowel.size() == 1)
            {
                //figure out how to handle if all the pool and the vowels are one char
            }
        }
    }

    //find the index of the vowel in the word and replace it with a random one from the vowel pool.
    std::uniform_int_distribution<uint_least32_t> distVowel = WRandGen::distribute( 0,  vowels.size()-1);
    std::vector<std::string>::iterator it = std::find(vowels.begin(), vowels.end(), vowels[distVowel(LangSeed::rng)]);
    int indexOfVowel = it - vowels.begin();

    std::uniform_int_distribution<uint_least32_t> distVowelPool = WRandGen::distribute( 0,  LongVowelPool.size() -1);

    std::string tempVal = value.erase(indexOfVowel, 1).insert(indexOfVowel, LongVowelPool[distVowelPool(LangSeed::rng)]);
    return Word(tempVal, meaning, vowels);
}

Word Word::ShortenVowel(std::vector<std::string> vowelPool)
{
    //check if the word is only one char
    if(value.size() < 2)
    {
        return Word(value, meaning, vowels); 
    } else {
       std::vector<std::string> ShortVowelPool;
        for( std::string vowel : vowels)
        {
            for(std::string poolVowel : vowelPool)
            {
                //if the vowel is only 1 char just replace the char
                if(poolVowel.size() <= vowel.size())
                {
                    ShortVowelPool.push_back(poolVowel);
                } else if( poolVowel.size() >= vowel.size() && vowel.size() == 1)
                {
                //figure out how to handle if all the pool and the vowels are 2 char
                }
            }
        }

        //find the index of the vowel in the word and replace it with a random one from the vowel pool.
        std::uniform_int_distribution<uint_least32_t> distVowel = WRandGen::distribute( 0,  vowels.size() - 1);
        std::string selectedVowel = vowels[distVowel(LangSeed::rng)];
        size_t indexOfVowel = value.find(selectedVowel);

        std::uniform_int_distribution<uint_least32_t> distVowelPool = WRandGen::distribute( 0,  ShortVowelPool.size() -1);
        
        std::string tempVal = value.replace(indexOfVowel, selectedVowel.size(), ShortVowelPool[distVowelPool(LangSeed::rng)]);
        
        return Word(tempVal, meaning, vowels); 
    }
    
}

Word Word::DeleteVowel()
{
    std::uniform_int_distribution<uint_least32_t> distVowel = WRandGen::distribute( 0,  (vowels.size()-1));
    std::string selectedVowel = vowels[distVowel(LangSeed::rng)];
    size_t indexOfVowel = value.find(selectedVowel);

    std::string tempVal = value.erase(indexOfVowel, selectedVowel.size());
    return Word(tempVal, meaning, vowels);
}

Word Word::AddSuffix(std::vector<std::string> suffixPool)
{
    std::uniform_int_distribution<uint_least32_t> distSuffix = WRandGen::distribute( 0,  (suffixPool.size() -1));

    std::string selectedSuffix = suffixPool[distSuffix(LangSeed::rng)];
    std::string tempVal = value + selectedSuffix;

    return Word(tempVal, meaning, vowels);

}

Word Word::AddPreffix(std::vector<std::string> preffixPool)
{
    std::uniform_int_distribution<uint_least32_t> distPreffix = WRandGen::distribute( 0,  preffixPool.size());

    std::string selectedPreffix = preffixPool[distPreffix(LangSeed::rng)];
    std::string tempVal = selectedPreffix + value;
    
    return Word(tempVal, meaning, vowels);

}

void Word::Kill()
{
    //call deconstructor maybe, this isn't really c++ kosher, but I will  call this already in the simHandler to just
    //delete this word from the dictionary
    //~Word();
    
}

Word Word::Shrink(int start, int end)
{   
    if(start - end == 0)
    {
        Word word = *this;
        return word;
    }
    std::string tempVal = value.substr(start, start - end);

    return Word(tempVal, meaning, vowels);
}

//idk how to set up MIX yet, I don't know what I want the mixed word to look like yet
Word Word::Mix(int start, int end, int otherWordStart, int otherWordEnd, Word otherWord)
{
    if(start - end == 0)
    {
        Word word = *this;
        return word;
    }
    std::string tempVal = value.replace(start, end - start, otherWord.getValue().substr(otherWordStart, otherWordEnd - otherWordStart));
    return Word(tempVal, meaning, vowels);

}

Word Word::Negate(std::vector<std::string> negatePool)
{
    //I want this to be a bit different, but currently it will be very similar to the prefix
    //adding a negating prefix and changing the meaning to the opposite of the meaning
    //after a bit of research I will come back and fix this

    std::uniform_int_distribution<uint_least32_t> distNegate = WRandGen::distribute( 0,  negatePool.size());

    std::string selectedNegation = negatePool[distNegate(LangSeed::rng)];
    std::string tempVal = selectedNegation + value;

    std::string tempMeaning = "the opposite of " + meaning;
    
    return Word(tempVal, tempMeaning, vowels);

}

Word Word::Subsitute(int start, int end, Word otherWord, bool replace)
{   
    std::string tempVal;
    if(replace)
    {
        tempVal = value.replace(start, end - start, otherWord.getValue());
    }else{
      tempVal = value.replace(start, end - start, otherWord.getValue().substr(start, end - start));  
    }
    return Word(tempVal, meaning, vowels);
}

Word Word::CreateNew(int size, bool structuredCreation)
{
    std::string tempVal = "";
    std::string tempMeaning;
    std::vector<std::string> vowels;

    if(!structuredCreation)
    {
        std::uniform_int_distribution<uint_least32_t> dist = WRandGen::distribute( 0,  DEFAULT_CHAR_SET_LENGTH - 1);
        for(int i = 0; i < size; i++)
        {
            tempVal += DEFAULT_CHAR_SET[dist(LangSeed::rng)];
        }

        //create a random bit mask of the word to create where the vowels are
        std::vector<int> bitMask;

        std::uniform_int_distribution<uint_least32_t> maskDist = WRandGen::distribute( 0,  PERCENT_CHANCE_VOWEL - 1);
        for(auto character : tempVal)
        {
            bitMask.push_back((maskDist(LangSeed::rng) < 1) ? 1 : 0);
        }

        for(int i = 0; i < bitMask.size(); i++)
        {
            if(bitMask[i])
            {
                std::string tempStr;
                tempStr.push_back(tempVal[i]);
                vowels.push_back(tempStr);
            }
        }

        //just take the same meaning from this word
        //I will probably change this with an API call that creates a 100% different meaning
        tempMeaning = meaning;

    } else {

    }

    return Word(tempVal, tempMeaning, vowels);
}

Word Word::ClipEnd(int start)
{
   
    std::string tempVal = value.erase(start);

    return Word(tempVal, meaning, vowels);
}

Word Word::ClipFront(int end)
{
    std::string tempVal = value.erase(0, end);

    return Word(tempVal, meaning, vowels);
}

Word Word::Compound(Word otherWord)
{
    std::uniform_int_distribution<uint_least32_t> dist = WRandGen::distribute( 0,  1);
    std::string tempVal = (dist(LangSeed::rng) > 1) ? otherWord.getValue() + value : value + otherWord.getValue();

    return Word(tempVal, meaning, vowels);
}

/*modify the meaning of the words.
  this will probably require a couple API calls */

Word Word::Broadening()
{

}

Word Word::Narrowing()
{

}

Word Word::ChangeMeaning()
{

}

Word Word::Ameliorate()
{

}

Word Word::Pejorate()
{

}

Word Word::OppositeMeaning() 
{

}

std::vector<std::string> Word::rebuildVowelList(std::vector<std::string> inVowelList)
{
    std::vector<std::string> rebuiltVowelList = inVowelList;

    auto i = std::begin(rebuiltVowelList);
    while( i != std::end(rebuiltVowelList))
    {
        //convert iterator into index to grab correct value
        std::size_t found=value.find(rebuiltVowelList[std::distance(rebuiltVowelList.begin(), i)]);
        if(found == std::string::npos)
        {
            i = rebuiltVowelList.erase(i);
        } else 
        {
            ++i;
        }
    }

    if(rebuiltVowelList.size() < 1)
    {
        //replace with a max of 4 vowels repeats will be removed anyway
        //(int)std::ceil((float)((value.size() -1) * NEW_VOWELS_PRECENT_OF_WORD))
        std::uniform_int_distribution<uint_least32_t> charDist = WRandGen::distribute( 1, STATIC_AMOUNT_OF_VOWELS);
        int vowelAmount = charDist(LangSeed::rng);
        
        for(int charIndex = 0; charIndex < vowelAmount; charIndex++)
        {
            if(value.size() > 2)
            {
                std::uniform_int_distribution<uint_least32_t> charPick = WRandGen::distribute(1, value.size() - 2 );
                int index = charPick(LangSeed::rng);
                std::string strVowel = this->value.substr(index, 1);
                rebuiltVowelList.push_back(strVowel);
            }else {
                std::string strVowel = value.substr(0, 1);
                rebuiltVowelList.push_back(strVowel);
            }
            
   
        }
    }
    return rebuiltVowelList;
}