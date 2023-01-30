#ifndef SIMULATION_HANDLER_HPP
#define SIMULATION_HANDLER_HPP

#include "Speaker.hpp"
#include "Constants.h"
#include "DictionaryLoader.hpp"
#include "WRandGen.hpp"
#include "LangSeed.hpp"
#include "MeaningLoader.hpp"
#include <vector>
#include <random>
#include <cctype>
#include <iostream>
#include <cstdio>
#include <unistd.h>



class SimulationHandler {
    public:

        void RunSimulation(int speakers, std::string dictionary1, std::string dictionary2, std::string dictionary3, int generations);

        //random generator wrappers
};

#endif