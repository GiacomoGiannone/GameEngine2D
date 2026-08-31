#include "EnemyManager.hpp"
#include "core/GameObject.hpp"
#include "core/PrefabFactory.hpp"
#include "core/Engine.hpp"
#include "core/Scene.hpp"
#include "core/CollisionBox.hpp"

#include <algorithm>

namespace GE
{
    std::vector<GameObject*> EnemyManager::enemies;
    EnemyManager* EnemyManager::instance = nullptr;

    void EnemyManager::updateEnemies(float deltaTime)
    {
        for (auto& enemy : enemies)
        {
            enemy->update(deltaTime);
        }
    }

    void EnemyManager::addEnemy(GameObject* enemy)
    {
        enemies.push_back(enemy);
    }

    void EnemyManager::removeEnemy(GameObject* enemy)
    {
        auto it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);
        }
    }

    std::vector<GameObject*> EnemyManager::getEnemies()
    {
        return enemies;
    }

    void EnemyManager::cleanupDestroyed()
    {
        //called after the scene update loop, so no component is being updated while we free memory
        Engine* engine = Engine::getInstance();
        Scene* scene = (engine != nullptr) ? engine->getCurrentScene() : nullptr;

        for (auto it = enemies.begin(); it != enemies.end();)
        {
            GameObject* enemy = *it;
            if (enemy != nullptr && enemy->isDestroyed())
            {
                if (scene != nullptr)
                {
                    scene->removeGameObject(enemy);
                }
                delete enemy;
                it = enemies.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    bool EnemyManager::isPositionValid(float x, float y, float enemyWidth, float enemyHeight, 
                                    const std::vector<GameObject*>& gameObjects) const
    {
        const float enemyHalfWidth = enemyWidth / 2.0f;
        const float enemyHalfHeight = enemyHeight / 2.0f;

        for (const auto& obj : gameObjects)
        {
            if (obj->hasComponentOfType<CollisionBox>())
            {
                auto& collisionBox = obj->getComponentOfType<CollisionBox>();
                float objX = obj->getX();
                float objY = obj->getY();
                float halfWidth = collisionBox.getWidth() / 2.0f;
                float halfHeight = collisionBox.getHeight() / 2.0f;

                // Test rettangolo-contro-rettangolo (AABB), non punto-contro-rettangolo
                bool overlapX = (x - enemyHalfWidth) < (objX + halfWidth) && 
                                (x + enemyHalfWidth) > (objX - halfWidth);
                bool overlapY = (y - enemyHalfHeight) < (objY + halfHeight) && 
                                (y + enemyHalfHeight) > (objY - halfHeight);

                if (overlapX && overlapY)
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool EnemyManager::spawn(float x, float y, int renderOrder)
    {
        GameObject* enemy = PrefabFactory::create("SkeletonEnemy", x, y, renderOrder);
        if (enemy == nullptr)
        {
            return false;
        }

        //track the enemy in the manager (the scene is responsible for adding it to the world)
        addEnemy(enemy);
        return true;
    }
}