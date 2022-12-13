#include "SimulationHandler.hpp"
#include <algorithm>
#include <iostream>

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char* argv[])
{

    SimulationHandler sim = SimulationHandler();

    //user inserts at least 3 different dictionaries
    char * dictionary1 = getCmdOption(argv, argv + argc, "-d1");

    char * dictionary2 = getCmdOption(argv, argv + argc, "-d2");

    char * dictionary3 = getCmdOption(argv, argv + argc, "-d3");

    //These are optional dictionaries
    char * dictionary4 = getCmdOption(argv, argv + argc, "-d4");

    char * dictionary5 = getCmdOption(argv, argv + argc, "-d5");

    if(cmdOptionExists(argv, argv+argc, "-h"))
    {
        //print out how to use program
        std::cout << "welcome to Word Evolve 2:" << 
        "\n -g = generations" << 
        "\n -s = amount of speakers" << 
        "\n -d[1-5] = xml initial dictionaries" <<
        "\n NOTE: d1-d3 are required for a basic model to run\n";
    }

    char * generations = getCmdOption(argv, argv + argc, "-g");

    char * speakers = getCmdOption(argv, argv + argc, "-s");

    if (dictionary1 != 0 && dictionary2 != 0 && dictionary3 != 0)
    {
       sim.RunSimulation(std::stoi(speakers), dictionary1, dictionary2, dictionary3, std::stoi(generations));
    }


    return 0;
}