#include "Barbarians.hpp"

Barbarian::Barbarian(std::vector<Word> dictionary, int x, int y)
{
    this->x = x;
    this->y = y;
    this->dictionary = dictionary;
}

void Barbarian::Murder(std::vector<Speaker> &speakerList)
{
    for(int i = 0; i < speakerList.size(); i++)
    {
        if(speakerList[i].getX() - x <= KILL_ZONE && speakerList[i].getY() - y  <= KILL_ZONE)
        {
            speakerList.erase(speakerList.begin() + i);
            i--;
        }
    }
}

std::vector<Word> Barbarian::Speak()
{
    std::shuffle(std::begin(dictionary), std::end(dictionary), std::default_random_engine());

    std::uniform_int_distribution<uint_least32_t> distAmountShare(0, dictionary.size() - 1);

    std::vector<Word> sharedWords = {dictionary.begin(), dictionary.begin() + distAmountShare(LangSeed::rng)}; 

    return sharedWords;

}