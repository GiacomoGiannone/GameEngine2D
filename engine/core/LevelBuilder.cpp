#include "LevelBuilder.hpp"
#include "Level.hpp"
#include "Scene.hpp"
#include "PrefabFactory.hpp"

#include <iostream>

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
                    {
                        //get a horizontal wall from PrefabFactory and add it to the scene
                        GameObject* wall = PrefabFactory::create("Wall_2", x * 81.0f, y * 81.0f, 0);
                        if (wall == nullptr)
                        {
                            std::cout << "[LevelBuilder] Failed to create prefab Wall_2 at (" << x << ", " << y << ")" << std::endl;
                        }
                        scene->addGameObject(wall);
                        break;
                    }

                    case '.':
                    {
                        //get a floor from PrefabFactory and add it to the scene
                        GameObject* floor = PrefabFactory::create("Floor_2", x * 81.0f, y * 81.0f, 0);
                        if (floor == nullptr)
                        {
                            std::cout << "[LevelBuilder] Failed to create prefab Floor_2 at (" << x << ", " << y << ")" << std::endl;
                        }
                        scene->addGameObject(floor);
                        break;
                    }
                }
            }
        }
    }
}