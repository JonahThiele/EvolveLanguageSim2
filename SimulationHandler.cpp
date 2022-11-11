#include "SimulationHandler.hpp"

void SimulationHandler::RunSimulation(int speakers, std::string dictionary1, std::string dictionary2, std::string dictionary3, int generations)
{
    bool run = true;

    std::vector<Speaker> SpeakerPopulation;

    //set up random generation for position of speakers
    std::mt19937 gen = WRandGen::generator();

    std::uniform_int_distribution<uint_least32_t> disty = WRandGen::distribute( 0,  MAX_Y);

    std::uniform_int_distribution<uint_least32_t> distx = WRandGen::distribute( 0,  MAX_X);

    //set the random picking of a dictioanry for the speakers, based on cmd line args
    //set this to a constant for now
    std::uniform_int_distribution<uint_least32_t> distDict = WRandGen::distribute( 0,  DEFAULT_RANDOM_DICTS - 1);

    //vector that holds the file paths
    std::vector<std::string> filePaths = {dictionary1, dictionary2, dictionary3};

    for(int speakersGenerated = 0; speakersGenerated < speakers; speakersGenerated++)
    {
        //create a new dictionary loader to load the class
        //draw the file names from constants now until we get the arguemnt parsing set up
        
        DictionaryLoader dictLoader(filePaths[distDict(gen)]);

        SpeakerPopulation.push_back(Speaker(distx(gen), disty(gen), dictLoader.getDictionary()));
    }
    
    while(generations > 0)
    {
        for(Speaker speaker1 : SpeakerPopulation)
        {
            for(Speaker speaker2 : SpeakerPopulation)
            {
                speaker2.learnWords(speaker1.speakToOtherPerson(speaker2));
            }
        }

        generations--;
            
    }
}