#pragma once

#include <string>

namespace GE
{
    class Level;
}

namespace GE
{
    //this class reads the layout file and returns a level so that you can create a Level object from it
    class LevelLoader
    {
        public:
            static Level load(const std::string& file);
    };
}