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
        static EnemyManager* getInstance()
        {
            if (!instance)
            {
                instance = new EnemyManager();
            }
            return instance;
        }
    private:
        static std::vector<GameObject*> enemies;
        //singleton pattern
        EnemyManager() = default;
        EnemyManager(const EnemyManager&) = delete;
        EnemyManager& operator=(const EnemyManager&) = delete;
        EnemyManager* instance = nullptr;
    };
}