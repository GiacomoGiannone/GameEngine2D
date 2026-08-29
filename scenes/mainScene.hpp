#pragma once

// Inherits from Scene and defines a main scene with
// some GameObjects that have a rectangle renderer component.

#include "core/Scene.hpp"
#include "core/GameObject.hpp"
#include "graphics/Color.hpp"
#include "graphics/RectangleRenderer.hpp"
#include "core/CharacterController.hpp"
#include "core/CollisionBox.hpp"
#include "graphics/SpriteRenderer.hpp"
#include "graphics/SpriteSheet.hpp"
#include "Core/Input.hpp"
#include "Core/Attack.hpp"
#include "Core/LightAttack.hpp"
#include "Core/HeavyAttack.hpp"
#include "Core/Hittable.hpp"
#include "Core/LevelLoader.hpp"
#include "Core/LevelBuilder.hpp"
#include "Core/Level.hpp"
#include "core/Light.hpp"
#include "core/LightingSystem.hpp"
#include "GameSpecificLibs/EnemyManager.hpp"
#include "Core/PrefabFactory.hpp"
#include "GameSpecificLibs/Chest.hpp"
#include "GameSpecificLibs/PlayerManager.hpp"
#include "GameSpecificLibs/Healthbar.hpp"

class MainScene : public GE::Scene
{
private:
    GE::Light playerLight;
    GE::LightingSystem lightingSystem;
    GE::GameObject* player = nullptr;

public:
    MainScene() : GE::Scene("MainScene"), playerLight({250.0f, 180.0f}, 180.0f, 0.6f)
    {
        //get a player prefab from the factory and add it to the scene
        player = GE::PrefabFactory::create("Player", 250.0f, 180.0f, 1);
        addGameObject(player);
        player->getComponentOfType<GE::CharacterController>().setWorldObjects(&getGameObjects());

        setCameraTarget(player);

        //instatiate a levelLoader
        GE::Level level = GE::LevelLoader::load("assets/Levels/CastleLevel/Layout.txt");

        //build the level in the scene
        GE::LevelBuilder levelBuilder(this);
        levelBuilder.build(level);

        //get the camera and set a larger viewport size to see more of the scene
        getCamera().setViewportSize(800, 600);

        GE::PlayerManager::getInstance()->setPlayer(player);

        GE::GameObject* chest = GE::PrefabFactory::create("Chest", 400.0f, 200.0f, 1);
        chest->getComponentOfType<GE::ChestLogic>().setPlayer(player);
        chest->getComponentOfType<GE::ChestLogic>().setLoot("Gold Coin");
        addGameObject(chest);

        //spawn some enemies
        // GE::EnemyManager::getInstance()->spawn(400, 400, 50);
        // GE::EnemyManager::getInstance()->spawn(1150, 400, 50);

        //add healthbar UI element
        GE::Healthbar* healthbar = new GE::Healthbar(200.0f, 20.0f);
        healthbar->setTarget(&player->getComponentOfType<GE::Hittable>());
        addUIElement(healthbar);

        int numEnemies = 20;
        for (int i = 0; i < numEnemies; ++i)
        {
            float x = static_cast<float>(rand() % 1600); // Random x position within the scene width
            float y = static_cast<float>(rand() % 1200); // Random y position within the scene height
            GE::EnemyManager::getInstance()->spawn(x, y, 50);
        }

        //register the spawned enemies with the scene so they are updated and rendered.
        //(They cannot be added by EnemyManager::spawn itself because the scene is not yet
        //assigned to the engine while this constructor runs.)
        for (GE::GameObject* enemy : GE::EnemyManager::getInstance()->getEnemies())
        {
            addGameObject(enemy);
            if (enemy->hasComponentOfType<GE::CharacterController>())
            {
                enemy->getComponentOfType<GE::CharacterController>().setWorldObjects(&getGameObjects());
            }
        }
    }

    void update(float deltaTime) override
    {
        GE::Scene::update(deltaTime);

        //remove dead enemies from the scene/manager (Hittable marks them via destroy())
        GE::EnemyManager::getInstance()->cleanupDestroyed();
        GE::PlayerManager::getInstance()->Update();

        //if the player is dead, unwire the camera from it so it doesn't follow a destroyed object
        if (player->isDestroyed())
        {
            setCameraTarget(nullptr);
        }

        playerLight.setPosition({player->getX(), player->getY()});
        lightingSystem.update(getGameObjects());
    }

    virtual ~MainScene()
    {
        removeGameObject(player);
        delete player;
    }

    // Used by the engine/camera to follow the player rectangle.
    GE::GameObject* getPlayer() const { return player; }
};