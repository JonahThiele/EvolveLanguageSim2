#include <string>
#include <vector>
#include "Speaker.hpp"
#include "Word.hpp"

class Barbarian{
    public:
        //presserve the language and don't modify, can't speak to other barbarians.
        Barbarian(std::vector<Word> dictionary, int x, int y);

        void Murder(std::vector<Speaker> &speakerList);

        std::vector<Word> Speak();
    
    private:

        int x;

        int y;

        std::vector<Word> dictionary;

};