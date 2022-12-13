#include "SimulationHandler.hpp"
#include <iostream>

void SimulationHandler::RunSimulation(int speakers, std::string dictionary1, std::string dictionary2, std::string dictionary3, int generations)
{
    bool run = true;

    std::vector<Speaker> SpeakerPopulation;

    //set up random generation for position of speakers

    std::uniform_int_distribution<uint_least32_t> disty = WRandGen::distribute( 0,  MAX_Y);

    std::uniform_int_distribution<uint_least32_t> distx = WRandGen::distribute( 0,  MAX_X);

    //set the random picking of a dictioanry for the speakers, based on cmd line args
    //set this to a constant for now
    std::uniform_int_distribution<uint_least32_t> distDict = WRandGen::distribute( 0,  DEFAULT_RANDOM_DICTS - 1);

    //vector that holds the file paths
    std::vector<std::string> filePaths = {dictionary1, dictionary2, dictionary3};

    //decalre Dictionary loader
    DictionaryLoader dictLoader;

    //call new random seed

    //WRandGen::setUpgenerator();

    for(int speakersGenerated = 0; speakersGenerated < speakers; speakersGenerated++)
    {
        //create a new dictionary loader to load the class
        //draw the file names from constants now until we get the arguemnt parsing set up
        
        dictLoader.InputDictionary(filePaths[distDict(LangSeed::rng)]);

        SpeakerPopulation.push_back(Speaker(distx(LangSeed::rng), disty(LangSeed::rng), dictLoader.getDictionary()));
    }
    
    while(generations > 0)
    {
        for(int speaker1 = 0; speaker1 < SpeakerPopulation.size(); speaker1++)
        {
            for(int speaker2 = 0; speaker2 < SpeakerPopulation.size(); speaker2++)
            {
                SpeakerPopulation[speaker2].learnWords(SpeakerPopulation[speaker1].speakToOtherPerson(SpeakerPopulation[speaker2]));
                
            }
        }

        generations--;
            
    }
    //output a few xml

    std::uniform_int_distribution<uint_least32_t> Poppick = WRandGen::distribute( 0,  SpeakerPopulation.size());

    dictLoader.OutputDictionary(SpeakerPopulation[Poppick(LangSeed::rng)].getDict());
    

}