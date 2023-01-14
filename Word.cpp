#include "Word.hpp"

Word::Word(const std::string &value, const std::string &meaning, const std::vector<std::string> &InVowels)
{
    this->value = value;
    this->meaning = meaning;
    this->vowels = rebuildVowelList(InVowels);
    std::cout << "Value:" << this->value << "\n";
    for(auto const& i : this->vowels)
        std::cout << i << ',';
    std::cout << "\n";
}

bool Word::Equal(const Word &otherWord) const
{
    if(otherWord.getValue() == this->value && otherWord.getMeaning() == this->meaning && otherWord.getVowels() == this->vowels)
    {
        return true;
    }
    
    return false;
}

bool Word::isDummyWord() const
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

Word Word::LengthenVowel(const std::vector<std::string> &vowelPool) const
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
    //create a copy of the vowel dictionary to call find on ?
    std::vector<std::string> tempVowels = vowels;
    std::uniform_int_distribution<uint_least32_t> distVowel = WRandGen::distribute( 0,  vowels.size()-1);
    std::vector<std::string>::iterator it = std::find(tempVowels.begin(), tempVowels.end(), tempVowels[distVowel(LangSeed::rng)]);
    int indexOfVowel = it - vowels.begin();

    std::uniform_int_distribution<uint_least32_t> distVowelPool = WRandGen::distribute( 0,  LongVowelPool.size() -1);

    std::string copyVal = value;

    std::string tempVal = copyVal.erase(indexOfVowel, 1).insert(indexOfVowel, LongVowelPool[distVowelPool(LangSeed::rng)]);
    return Word(tempVal, meaning, vowels);
}

Word Word::ShortenVowel(const std::vector<std::string> &vowelPool) const
{
    //check if the word is only one char
    if(value.size() < 2)
    {
        return Word(value, meaning, vowels); 
    } else {
    //this loop could be rewritten
       std::vector<std::string> ShortVowelPool;
        for( std::string replacedvowel : vowels)
        {
            for(std::string poolVowel : vowelPool)
            {
                //if the vowel is only 1 char just replace the char
                if(poolVowel.size() <= replacedvowel.size())
                {
                    ShortVowelPool.push_back(poolVowel);
                }
            }
        }

        //find the index of the vowel in the word and replace it with a random one from the vowel pool.
        std::uniform_int_distribution<uint_least32_t> distVowel = WRandGen::distribute( 0,  vowels.size() - 1);
        std::string selectedVowel = vowels[distVowel(LangSeed::rng)];
        size_t indexOfVowel = value.find(selectedVowel);

        std::uniform_int_distribution<uint_least32_t> distVowelPool = WRandGen::distribute( 0,  ShortVowelPool.size() -1);
        
        std::string copyVal = value;
        std::string tempVal = copyVal.replace(indexOfVowel, selectedVowel.size(), ShortVowelPool[distVowelPool(LangSeed::rng)]);
        
        return Word(tempVal, meaning, vowels); 
    }
    
}

Word Word::DeleteVowel() const
{
    std::uniform_int_distribution<uint_least32_t> distVowel = WRandGen::distribute( 0,  (vowels.size()-1));
    std::string selectedVowel = vowels[distVowel(LangSeed::rng)];
    size_t indexOfVowel = value.find(selectedVowel);

    std::string copyVal = value;
    std::string tempVal = copyVal.erase(indexOfVowel, selectedVowel.size());
    return Word(tempVal, meaning, vowels);
}

Word Word::AddSuffix(const std::vector<std::string> &suffixPool) const
{
    std::uniform_int_distribution<uint_least32_t> distSuffix = WRandGen::distribute( 0,  (suffixPool.size() -1));

    std::string selectedSuffix = suffixPool[distSuffix(LangSeed::rng)];
    std::string tempVal = value + selectedSuffix;

    return Word(tempVal, meaning, vowels);

}

Word Word::AddPreffix(const std::vector<std::string> &preffixPool) const
{
    std::uniform_int_distribution<uint_least32_t> distPreffix = WRandGen::distribute( 0,  preffixPool.size());

    std::string selectedPreffix = preffixPool[distPreffix(LangSeed::rng)];
    std::string tempVal = selectedPreffix + value;
    
    return Word(tempVal, meaning, vowels);

}

Word Word::Shrink(int start, int end) const
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
Word Word::Mix(int start, int end, int otherWordStart, int otherWordEnd, const Word &otherWord) const
{
    if(start - end == 0)
    {
        Word word = *this;
        return word;
    }

    std::string copyVal = value;
    std::string tempVal = copyVal.replace(start, end - start, otherWord.getValue().substr(otherWordStart, otherWordEnd - otherWordStart));
    return Word(tempVal, meaning, vowels);

}

Word Word::Negate(const std::vector<std::string> &negatePool) const
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

Word Word::Subsitute(int start, int end, const Word &otherWord, bool replace) const
{   
    std::string tempVal;
    std::string copyVal = value;
    if(replace)
    {
        tempVal = copyVal.replace(start, end - start, otherWord.getValue());
    }else{
      tempVal = copyVal.replace(start, end - start, otherWord.getValue().substr(start, end - start));  
    }
    return Word(tempVal, meaning, vowels);
}

Word Word::CreateNew(int size, bool structuredCreation) const
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

Word Word::ClipEnd(int start) const
{
    std::string copyVal = value;
    std::string tempVal = copyVal.erase(start);

    return Word(tempVal, meaning, vowels);
}

Word Word::ClipFront(int end) const
{
    std::string copyVal = value;
    std::string tempVal = copyVal.erase(0, end);

    return Word(tempVal, meaning, vowels);
}

Word Word::Compound(const Word &otherWord) const
{
    std::uniform_int_distribution<uint_least32_t> dist = WRandGen::distribute( 0,  1);
    std::string tempVal = (dist(LangSeed::rng) > 1) ? otherWord.getValue() + value : value + otherWord.getValue();

    return Word(tempVal, meaning, vowels);
}

/*modify the meaning of the words.
  this will probably require a couple API calls */

Word Word::Broadening() const
{

}

Word Word::Narrowing() const
{

}

Word Word::ChangeMeaning() const
{

}

Word Word::Ameliorate() const
{

}

Word Word::Pejorate() const
{

}

Word Word::OppositeMeaning() const
{

}

std::vector<std::string> Word::rebuildVowelList(const std::vector<std::string> &inVowelList) const
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