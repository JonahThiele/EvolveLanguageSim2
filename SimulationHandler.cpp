#include "SimulationHandler.hpp"

void SimulationHandler::RunSimulation(int speakers, std::string dictionary1, std::string dictionary2, std::string dictionary3, int generations)
{
    bool run = true;

    std::vector<Speaker> SpeakerPopulation;
    std::vector<Barbarian> BarbarianPopulation;

    //set up random generation for position of speakers

    std::uniform_int_distribution<uint_least32_t> disty = WRandGen::distribute( 0,  MAX_Y);

    std::uniform_int_distribution<uint_least32_t> distx = WRandGen::distribute( 0,  MAX_X);

    //set the random picking of a dictioanry for the speakers, based on cmd line args
    //set this to a constant for now
    std::uniform_int_distribution<uint_least32_t> distDict = WRandGen::distribute( 0,  DEFAULT_RANDOM_DICTS - 1);

    //generate random sizes for the 
    std::uniform_int_distribution<uint_least32_t> distDictSizes = WRandGen::distribute( MIN_MAX_DICT,  MAX_MAX_DICT);
    //vector that holds the file paths
    std::vector<std::string> filePaths = {dictionary1, dictionary2, dictionary3};

    //decalre Dictionary loader
    DictionaryLoader dictLoader;

    MeaningLoader meaningLoader;

    //call new random seed

    //WRandGen::setUpgenerator();
    for(int barbGenerated = 0;  barbGenerated < 50; barbGenerated++)
    {
        dictLoader.InputDictionary(BARB_DICT_FILE);
        BarbarianPopulation.push_back(Barbarian(dictLoader.getDictionary(), distx(LangSeed::rng), disty(LangSeed::rng)));
    }

    for(int speakersGenerated = 0; speakersGenerated < speakers; speakersGenerated++)
    {
        //create a new dictionary loader to load the class
        //draw the file names from constants now until we get the arguemnt parsing set up

        //create 10% isolated community
        std::uniform_int_distribution<uint_least32_t> distIsolate = WRandGen::distribute( 0,  99);
        
        if(distIsolate(LangSeed::rng) <= 24)
        {
            dictLoader.InputDictionary(filePaths[distDict(LangSeed::rng)]);
            SpeakerPopulation.push_back(Speaker(distx(LangSeed::rng), disty(LangSeed::rng), dictLoader.getDictionary(), distDictSizes(LangSeed::rng), std::make_shared<MeaningLoader>(meaningLoader), ISOLATION_TAG));
        } else 
        {
            dictLoader.InputDictionary(filePaths[distDict(LangSeed::rng)]);
            SpeakerPopulation.push_back(Speaker(distx(LangSeed::rng), disty(LangSeed::rng), dictLoader.getDictionary(), distDictSizes(LangSeed::rng), std::make_shared<MeaningLoader>(meaningLoader), 0));
        }
        
    }
    
    bool barbs = BARBS_ON;
    
    while(generations > 0)
    {
        if(barbs && generations > 3)
        {
            std::cout << "BARBARIANS!!!\n";
            for(Barbarian barb : BarbarianPopulation)
            {
                //purges words by killing speakers and adds a different influence
                barb.Murder(SpeakerPopulation);
                std::uniform_int_distribution<uint_least32_t> distSpeak = WRandGen::distribute( 0,  SpeakerPopulation.size() - 1);
                SpeakerPopulation[distSpeak(LangSeed::rng)].learnWords(barb.Speak());
            }
            barbs = !barbs;
        }
        std::cout << "Gen:" << generations << "\n";
        for(int speaker1 = 0; speaker1 < SpeakerPopulation.size(); speaker1++)
        {
            std::cout << "Person:" << speaker1 + 1 << "/" << SpeakerPopulation.size() << "\n";
            fflush(stdout);

            if(SpeakerPopulation[speaker1].increaseAge())
            {
                for(int speaker2 = 0; speaker2 < SpeakerPopulation.size(); speaker2++)
                {
                    if(std::abs(SpeakerPopulation[speaker1].getX()  -  SpeakerPopulation[speaker2].getX()) <= CLOSEX && std::abs(SpeakerPopulation[speaker1].getY()  -  SpeakerPopulation[speaker2].getY()) <= CLOSEY && SpeakerPopulation[speaker2].getTag() == SpeakerPopulation[speaker1].getTag())
                    {
                        SpeakerPopulation[speaker2].learnWords(SpeakerPopulation[speaker1].speakToOtherPerson(SpeakerPopulation[speaker2]));
                    }
                
                }
                std::cout << "\r"; 
            } else 
            {
                SpeakerPopulation.erase(SpeakerPopulation.begin() + speaker1);
                speaker1--;
            }

            //25% of the time new births occur
            std::uniform_int_distribution<uint_least32_t> distBirth = WRandGen::distribute( 0,  99);
            if(distBirth(LangSeed::rng) <= 24)
            {
                dictLoader.InputDictionary(filePaths[distDict(LangSeed::rng)]);
                SpeakerPopulation.push_back(Speaker(distx(LangSeed::rng), disty(LangSeed::rng), dictLoader.getDictionary(), distDictSizes(LangSeed::rng), std::make_shared<MeaningLoader>(meaningLoader), 0));
            }
         
        }

        generations--;
            
    }
    //output a few xml

    std::uniform_int_distribution<uint_least32_t> Poppick = WRandGen::distribute( 0,  SpeakerPopulation.size());

    dictLoader.OutputDictionary(SpeakerPopulation[Poppick(LangSeed::rng)].getDict());
    

}