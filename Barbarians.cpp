#include "Barbarians.hpp"

Barbarian::Barbarian(std::vector<std::shared_ptr<Word>> dictionary, int x, int y)
{
    this->x = x;
    this->y = y;
    this->dictionary = dictionary;
}

void Barbarian::Murder(std::vector<std::shared_ptr<Speaker>>& speakerList)
{
    for(unsigned int i = 0; i < speakerList.size(); i++)
    {
        if(speakerList[i]->getX() - x <= KILL_ZONE && speakerList[i]->getY() - y  <= KILL_ZONE)
        {
            speakerList.erase(speakerList.begin() + i);
            i--;
        }
    }
}

std::vector<std::shared_ptr<Word>> Barbarian::Speak()
{
    std::shuffle(std::begin(dictionary), std::end(dictionary), std::default_random_engine());

    std::uniform_int_distribution<uint_least32_t> distAmountShare(0, dictionary.size() - 1);

    std::vector<std::shared_ptr<Word>> sharedWords; 
    //loop for easier reading
    int maxShared = distAmountShare(LangSeed::rng);
    for(int i = 0; i < maxShared; i++){

        sharedWords.push_back(std::move(dictionary[i]));
    }


    return sharedWords;

}
