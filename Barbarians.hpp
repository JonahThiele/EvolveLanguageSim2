#include <string>
#include <vector>
#include "Speaker.hpp"

class Barbarian{
    public:
        //presserve the language and don't modify, can't speak to other barbarians.
        Barbarian();

        bool Murder(std::vector<Speaker> &speakerList);

        std::vector<std::string> Speak();

};