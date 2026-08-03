#include "LevelLoader.hpp"
#include "Level.hpp"
#include <fstream>

namespace GE
{
    Level LevelLoader::load(const std::string& file)
    {
        Level level;

        std::ifstream in(file);

        std::string line;

        while(std::getline(in, line))
        {
            level.addRow(line);
        }

        return level;
    }
}