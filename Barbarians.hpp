#ifndef BARBARIANS_HPP
#define BARBARIANS_HPP
#include <string>
#include <vector>
#include "Speaker.hpp"
#include "Word.hpp"
#include <memory>

class Barbarian{
    public:
        //presserve the language and don't modify, can't speak to other barbarians.
        Barbarian(std::vector<std::shared_ptr<Word>> dictionary, int x, int y);

        void Murder(std::vector<std::shared_ptr<Speaker>> &speakerList);

        std::vector<std::shared_ptr<Word>> Speak();
    
    private:

        int x;

        int y;

        std::vector<std::shared_ptr<Word>> dictionary;

};

#endif
