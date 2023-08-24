#include "SimulationHandler.hpp"
#include "indicators.hpp"

void SimulationHandler::RunSimulation(int speakers, std::string dictionary1, std::string dictionary2, std::string dictionary3, int generationMax)
{
    
    //save generation max so we can create a proper loading screen
    int generations = 0;

    std::vector<std::shared_ptr<Speaker>> SpeakerPopulation;
    std::vector<std::shared_ptr<Barbarian>> BarbarianPopulation;

    //set up random generation for position of speakers

    std::uniform_int_distribution<uint_least32_t> disty = WRandGen::distribute( 0,  MAX_Y);

    std::uniform_int_distribution<uint_least32_t> distx = WRandGen::distribute( 0,  MAX_X);

    //set the random picking of a dictioanry for the speakers, based on cmd line args
    //set this tes  a constant for now
    std::uniform_int_distribution<uint_least32_t> distDict = WRandGen::distribute( 0,  DEFAULT_RANDOM_DICTS - 1);

    //generate random sizes for the 
    std::uniform_int_distribution<uint_least32_t> distDictSizes = WRandGen::distribute( MIN_MAX_DICT,  MAX_MAX_DICT);
    //vector that holds the file paths
    std::vector<std::string> filePaths = {dictionary1, dictionary2, dictionary3};

    //decalre Dictionary loader
    DictionaryLoader dictLoader;
    //MeaningLoader mLoader;
    std::shared_ptr<MeaningLoader> meaningLoader = std::make_shared<MeaningLoader>();

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
        
        std::shared_ptr<MeaningLoader> personalMeaning = meaningLoader;
        if(distIsolate(LangSeed::rng) <= 24)
        {
            dictLoader.InputDictionary(filePaths[distDict(LangSeed::rng)]);
            SpeakerPopulation.push_back(std::move(std::make_shared<Speaker>(Speaker(distx(LangSeed::rng), disty(LangSeed::rng), dictLoader.getDictionary(), distDictSizes(LangSeed::rng), personalMeaning, ISOLATION_TAG,10, 10, 10, "Standard") )));
        } else 
        {
            dictLoader.InputDictionary(filePaths[distDict(LangSeed::rng)]);
            SpeakerPopulation.push_back(std::move(std::make_shared<Speaker>(Speaker(distx(LangSeed::rng), disty(LangSeed::rng), dictLoader.getDictionary(), distDictSizes(LangSeed::rng), personalMeaning, 0, 10, 10, 10, "Standard"))));
        }
        
    }
    
    bool barbs = BARBS_ON; 

    std::string str = "0\\" + generationMax;

    indicators::show_console_cursor(false);
  

   indicators::ProgressBar genBar{
              indicators::option::BarWidth{80},
              indicators::option::Start{"["},
              indicators::option::End{"]"},
              indicators::option::PrefixText{"Generations"},
              indicators::option::PostfixText{str},
              indicators::option::ShowElapsedTime{false},
              indicators::option::ForegroundColor{indicators::Color::white}  ,
              indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
              };

   indicators::ProgressBar popBar{
                indicators::option::BarWidth{80},
                indicators::option::Start{"["},
                indicators::option::End{"]"},
                indicators::option::PrefixText{"Speakers"},
                indicators::option::ShowElapsedTime{false},
                indicators::option::ForegroundColor{indicators::Color::white}  ,
                indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
                };

   indicators::DynamicProgress<indicators::ProgressBar> bars(genBar, popBar);

    while(generations < generationMax)
    {

         bars[0].set_progress((float)((float)generations / (float)generationMax) * 100);
         bars[0].set_option(indicators::option::PostfixText{std::to_string(generations) + "/" + std::to_string(generationMax)});          

         bars[1].set_progress(0);

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
            
              bars[1].set_progress((float)((float)speaker1 / (float)SpeakerPopulation.size()) * 100);
              bars[1].set_option(indicators::option::PostfixText{std::to_string(speaker1) + "/" + std::to_string(SpeakerPopulation.size())});
              
            if(SpeakerPopulation[speaker1]->increaseAge())
            {
                for( unsigned int speaker2 = 0; speaker2 < SpeakerPopulation.size(); speaker2++)
                {

                     bars[1].set_progress((float)((float)speaker2 / (float)SpeakerPopulation.size()) * 100);
                     bars[1].set_option(indicators::option::PostfixText{std::to_string(speaker2) + "/" + std::to_string(SpeakerPopulation.size())});
                    
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
                std::shared_ptr<MeaningLoader> personalMeaning = meaningLoader;
                dictLoader.InputDictionary(filePaths[distDict(LangSeed::rng)]);
                SpeakerPopulation.push_back(std::move(std::make_shared<Speaker>(Speaker(distx(LangSeed::rng), disty(LangSeed::rng), dictLoader.getDictionary(), distDictSizes(LangSeed::rng), personalMeaning, 0, 10, 10, 10, "Standard"))));
            }
        

        }

        bars[1].set_progress(100);
        bars[1].set_option(indicators::option::PostfixText{std::to_string(SpeakerPopulation.size()) + "/" + std::to_string(SpeakerPopulation.size())});
        bars[1].is_completed();
        
        generations++;
            
    }
        bars[0].set_progress(100);
        bars[0].set_option(indicators::option::PostfixText{std::to_string(generations) + "/" + std::to_string(generationMax)});
        bars[0].is_completed();
        //output a few xml

        std::uniform_int_distribution<uint_least32_t> Poppick = WRandGen::distribute( 0,  SpeakerPopulation.size());

        dictLoader.OutputDictionary(SpeakerPopulation[Poppick(LangSeed::rng)]->getDict());
        
        indicators::show_console_cursor(true);

}
