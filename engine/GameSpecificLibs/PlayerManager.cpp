#include "PlayerManager.hpp"
#include "core/Engine.hpp"
#include "core/Scene.hpp"
#include "core/GameObject.hpp"

namespace GE
{
    PlayerManager* PlayerManager::instance = nullptr;

    bool PlayerManager::DestroyPlayer()
    {
        Engine* engine = Engine::getInstance();
        Scene* scene = (engine != nullptr) ? engine->getCurrentScene() : nullptr;

        if (player != nullptr && scene != nullptr)
        {
            scene->removeGameObject(player);
            delete player;
            player = nullptr;
            return true;
        }
        return false;
    }

    void PlayerManager::Update()
    {
         if (player != nullptr && player->isDestroyed())
        {
            DestroyPlayer();
            // qui puoi anche agganciare la tua logica di game over, respawn, ecc.
        }
    }
}