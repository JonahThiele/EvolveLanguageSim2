#include "SimulationHandler.hpp"

void SimulationHandler::RunSimulation(int speakers, std::string dictionary1, std::string dictionary2, std::string dictionary3, int generations)
{
    
    //save generation max so we can create a proper loading screen
    int generationMax = generations;

    std::vector<std::shared_ptr<Speaker>> SpeakerPopulation;
    std::vector<std::shared_ptr<Barbarian>> BarbarianPopulation;

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

    
    for(int barbGenerated = 0;  barbGenerated < 50; barbGenerated++)
    {
        dictLoader.InputDictionary(BARB_DICT_FILE);
        BarbarianPopulation.push_back(std::move(std::make_shared<Barbarian>(Barbarian(dictLoader.getDictionary(), distx(LangSeed::rng), disty(LangSeed::rng)))));
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
            SpeakerPopulation.emplace_back(std::move(std::make_shared<Speaker>(Speaker(distx(LangSeed::rng), disty(LangSeed::rng), dictLoader.getDictionary(), distDictSizes(LangSeed::rng), std::make_shared<MeaningLoader>(meaningLoader), ISOLATION_TAG))));
        } else 
        {
            dictLoader.InputDictionary(filePaths[distDict(LangSeed::rng)]);
            SpeakerPopulation.emplace_back(std::move(std::make_shared<Speaker>(Speaker(distx(LangSeed::rng), disty(LangSeed::rng), dictLoader.getDictionary(), distDictSizes(LangSeed::rng), std::make_shared<MeaningLoader>(meaningLoader), 0))));
        }
        
    }
    
    bool barbs = BARBS_ON;
   
    int digitOffset = (generations < 10 ? 1 :   
        (generations < 100 ? 2 :   
        (generations < 1000 ? 3 :   
        (generations < 10000 ? 4 :   
        (generations < 100000 ? 5 :   
        (generations < 1000000 ? 6 :   
        (generations < 10000000 ? 7 :  
        (generations < 100000000 ? 8 :  
        (generations < 1000000000 ? 9 :  
        10)))))))));  

    //print out the start time
   std::time_t now = std::time(nullptr);
   std::tm localtm = *std::localtime(&now);

   std::cout.imbue(std::locale("en_US.UTF-8"));
    
   std::cout << "starting at " << std::put_time(&localtm, "%c") << '\n';
  
    float  progressBarInc = 60 / generations;  
   //move the cursor down one line to start the info line
   std::string offsetStr= std::to_string(digitOffset * 2 + 2);

     std::cout << generations << '/' << generationMax << ":\n";
     std::cout << "Starting the script..\n";

    float currProgress = 0;
    int lastProgress = 0;
    while(generations > 0)
    {
        currProgress += progressBarInc;
       // std::cout << currProgress << std::endl;
      // std::cout << lastProgress << std::endl;   
        if(std::round(currProgress) > lastProgress){
            lastProgress = std::round(currProgress);
            //move the cursor up one and over two and delete the newline char which is replaced with the
            std::cout << offsetStr << std::endl;
            std::cout << "\x1b[2A" << "\x1b[" + offsetStr << "C" << '\n';
    }

        if(barbs && generations > 3)
        {
            std::cout << "BARBARIANS!!!\n" << std::endl;
            for(auto &barb : BarbarianPopulation)
            {
                //purges words by killing speakers and adds a different influence
                barb->Murder(SpeakerPopulation);
                std::uniform_int_distribution<uint_least32_t> distSpeak = WRandGen::distribute( 0,  SpeakerPopulation.size() - 1);
                SpeakerPopulation[distSpeak(LangSeed::rng)]->learnWords(barb->Speak());
            }
            barbs = !barbs;
        }

       
        for(unsigned int speaker1 = 0; speaker1 < SpeakerPopulation.size(); speaker1++)
        {

            if(SpeakerPopulation[speaker1]->increaseAge())
            {
                for( unsigned int speaker2 = 0; speaker2 < SpeakerPopulation.size(); speaker2++)
                {
                    if(std::abs(SpeakerPopulation[speaker1]->getX()  -  SpeakerPopulation[speaker2]->getX()) <= CLOSEX && std::abs(SpeakerPopulation[speaker1]->getY()  -  SpeakerPopulation[speaker2]->getY()) <= CLOSEY && SpeakerPopulation[speaker2]->getTag() == SpeakerPopulation[speaker1]->getTag())
                    {
                        SpeakerPopulation[speaker2]->learnWords(SpeakerPopulation[speaker1]->speakToOtherPerson(*SpeakerPopulation[speaker2]));
                    }
                
                }
 
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
                SpeakerPopulation.emplace_back(std::move(std::make_shared<Speaker>(Speaker(distx(LangSeed::rng), disty(LangSeed::rng), dictLoader.getDictionary(), distDictSizes(LangSeed::rng), std::make_shared<MeaningLoader>(meaningLoader), 0))));
            }
        

        }
        
        generations--;
            
    }
    //output a few xml

    std::uniform_int_distribution<uint_least32_t> Poppick = WRandGen::distribute( 0,  SpeakerPopulation.size());

    dictLoader.OutputDictionary(SpeakerPopulation[Poppick(LangSeed::rng)]->getDict());
    

}
