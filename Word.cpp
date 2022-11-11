#include "Word.hpp"

Word::Word(std::string value, std::string meaning, std::vector<std::string> vowels)
{
    this->value = value;
    this->meaning = meaning;
    this->vowels = vowels;
    rebuildVowelList(vowels);
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
    std::string IgnoreList[2] = {"KILL", "DEFAULT"};
    for(int i = 0; i < 2; i++)
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
            if(poolVowel.size() > vowel.size())
            {
                LongVowelPool.push_back(poolVowel);
            } else if( poolVowel.size() == vowel.size() && vowel.size() == 1)
            {
                //figure out how to handle if all the pool and the vowels are one char
            }
        }
    }

    //find the index of the vowel in the word and replace it with a random one from the vowel pool.
    std::uniform_int_distribution<uint_least32_t> distVowel = WRandGen::distribute( 0,  vowels.size());
    size_t indexOfVowel = value.find(vowels[distVowel(gen)]);

    std::uniform_int_distribution<uint_least32_t> distVowelPool = WRandGen::distribute( 0,  LongVowelPool.size());

    std::string tempVal = value.erase(indexOfVowel, 1).insert(indexOfVowel + 1, LongVowelPool[distVowelPool(gen)]);
    return Word(tempVal, meaning, vowels);
}

Word Word::ShortenVowel(std::vector<std::string> vowelPool)
{
    std::vector<std::string> ShortVowelPool;
    for( std::string vowel : vowels)
    {
        for(std::string poolVowel : vowelPool)
        {
            if(poolVowel.size() < vowel.size())
            {
                ShortVowelPool.push_back(poolVowel);
            } else if( poolVowel.size() == vowel.size() && vowel.size() == 1)
            {
                //figure out how to handle if all the pool and the vowels are one char
            }
        }
    }

    //find the index of the vowel in the word and replace it with a random one from the vowel pool.
    std::uniform_int_distribution<uint_least32_t> distVowel = WRandGen::distribute( 0,  vowels.size());
    std::string selectedVowel = vowels[distVowel(gen)];
    size_t indexOfVowel = value.find(selectedVowel);

    std::uniform_int_distribution<uint_least32_t> distVowelPool = WRandGen::distribute( 0,  ShortVowelPool.size());

    std::string tempVal = value.erase(indexOfVowel, selectedVowel.size()).insert(indexOfVowel + 1, ShortVowelPool[distVowelPool(gen)]);
    return Word(tempVal, meaning, vowels);
}

Word Word::DeleteVowel()
{
    std::uniform_int_distribution<uint_least32_t> distVowel = WRandGen::distribute( 0,  vowels.size());
    std::string selectedVowel = vowels[distVowel(gen)];
    size_t indexOfVowel = value.find(selectedVowel);

    std::string tempVal = value.erase(indexOfVowel, selectedVowel.size());
    return Word(tempVal, meaning, vowels);
}

Word Word::AddSuffix(std::vector<std::string> suffixPool)
{
    std::uniform_int_distribution<uint_least32_t> distSuffix = WRandGen::distribute( 0,  suffixPool.size());

    std::string selectedSuffix = suffixPool[distSuffix(gen)];
    std::string tempVal = value + selectedSuffix;

    return Word(tempVal, meaning, vowels);

}

Word Word::AddPreffix(std::vector<std::string> preffixPool)
{
    std::uniform_int_distribution<uint_least32_t> distPreffix = WRandGen::distribute( 0,  preffixPool.size());

    std::string selectedPreffix = preffixPool[distPreffix(gen)];
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
    std::string tempVal = value.substr(start, start - end);

    return Word(tempVal, meaning, vowels);
}

//idk how to set up MIX yet, I don't know what I want the mixed word to look like yet
Word Word::Mix(int start, int end, int otherWordStart, int otherWordEnd, Word otherWord)
{
    std::string tempVal = value.erase(start, start - end).insert(start + 1, otherWord.getValue().substr(otherWordStart, otherWordStart - otherWordEnd));

    return Word(tempVal, meaning, vowels);

}

Word Word::Negate(std::vector<std::string> negatePool)
{
    //I want this to be a bit different, but currently it will be very similar to the prefix
    //adding a negating prefix and changing the meaning to the opposite of the meaning
    //after a bit of research I will come back and fix this

    std::uniform_int_distribution<uint_least32_t> distNegate = WRandGen::distribute( 0,  negatePool.size());

    std::string selectedNegation =negatePool[distNegate(gen)];
    std::string tempVal = selectedNegation + value;

    std::string tempMeaning = "the opposite of " + meaning;
    
    return Word(tempVal, tempMeaning, vowels);

}

Word Word::Subsitute(int start, int end, Word otherWord)
{
    std::string tempVal = value.erase(start, start - end).insert(start + 1, otherWord.getValue().substr(start, start - end));

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
            tempVal += DEFAULT_CHAR_SET[dist(gen)];
        }

        //create a random bit mask of the word to create where the vowels are
        std::vector<int> bitMask;

        std::uniform_int_distribution<uint_least32_t> maskDist = WRandGen::distribute( 0,  PERCENT_CHANCE_VOWEL - 1);
        for(auto character : tempVal)
        {
            bitMask.push_back((maskDist(gen) < 1) ? 1 : 0);
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
    std::string tempVal = (dist(gen) > 1) ? otherWord.getValue() + value : value + otherWord.getValue();

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

void Word::rebuildVowelList(std::vector<std::string> vowelList)
{
    for(int i = 0; i < vowelList.size(); i++)
    {
        std::size_t found=value.find(vowels[i]);
        if (found == std::string::npos){
            vowelList.erase(vowelList.begin() + i);
        }
    }
    vowels = vowelList;
}