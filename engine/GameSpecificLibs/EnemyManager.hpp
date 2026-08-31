#pragma once
#include <vector>

namespace GE
{
    class GameObject;
}

namespace GE
{
    class EnemyManager
    {
    public:
        static void updateEnemies(float deltaTime);
        static void addEnemy(GameObject* enemy);
        static void removeEnemy(GameObject* enemy);
        static std::vector<GameObject*> getEnemies();
        //remove from the scene and delete every enemy flagged with destroy() (dead enemies)
        static void cleanupDestroyed();
        static EnemyManager* getInstance()
        {
            if (!instance)
            {
                instance = new EnemyManager();
            }
            return instance;
        }
        bool spawn(float x, float y, int renderOrder);
        bool isPositionValid(float x, float y, float enemyWidth, float enemyHeight, 
                                    const std::vector<GameObject*>& gameObjects) const;
    private:
        static std::vector<GameObject*> enemies;
        //singleton pattern
        EnemyManager() = default;
        EnemyManager(const EnemyManager&) = delete;
        EnemyManager& operator=(const EnemyManager&) = delete;
        static EnemyManager* instance;
    };
}