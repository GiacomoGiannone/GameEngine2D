#include "LevelBuilder.hpp"
#include "Level.hpp"
#include "Scene.hpp"
#include "PrefabFactory.hpp"

namespace GE
{
    void LevelBuilder::build(Level level)
    {
        if (scene == nullptr)
        {
            return;
        }

        for (int y = 0; y < level.getNumRows(); ++y)
        {
            const std::string& row = level.getRow(y);

            for (int x = 0; x < static_cast<int>(row.size()); ++x)
            {
                char c = level.getCell(y, x);

                switch (c)
                {
                    case '#':
                        //get a wall from PrefabFactory and add it to the scene
                        scene->addGameObject(PrefabFactory::create("Wall_1", x * 164.0f, y * 164.0f, 0));
                        break;

                    case '.':
                        //get a floor from PrefabFactory and add it to the scene
                        scene->addGameObject(PrefabFactory::create("Floor_1", x * 164.0f, y * 164.0f, 0));
                        break;
                }
            }
        }
    }
}