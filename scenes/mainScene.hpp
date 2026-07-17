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

class MainScene : public GE::Scene
{
private:
    GE::GameObject* player;
    GE::GameObject* rectangleObject2;
    GE::GameObject* rectangleObject3;
    GE::GameObject* rectangleObject4;

public:
    MainScene() : GE::Scene("MainScene")
    {
        // Player-controlled rectangle (camera will follow this).
        player = new GE::GameObject();
        player->getTransform().setPosition(250.0f, 180.0f);

        // Add sprite renderer to player FIRST (needed by Animator)
        auto& spriteRenderer = player->addComponent<GE::SpriteRenderer>(
            "player_texture",
            "assets/player.png",
            player->getTransform()
        );

        // Add character controller
        auto& characterController = player->addComponent<GE::CharacterController>(
            200.0f, // speed
            20.0f, // width of the collision box
            40.0f  // height of the collision box
        );
        //get the character controller to print debug info
        player->getComponentOfType<GE::CharacterController>().setDebugPrint(true);
        //set this variable to true to enable Y movement with W and S keys
        player->getComponentOfType<GE::CharacterController>().setYMovementEnabled(true);

        // Create sprite sheet and animator
        GE::SpriteSheet spriteSheet("player_texture", "assets/player.png", 64, 64);
        auto& playerAnimator = player->addComponent<GE::Animator>(spriteRenderer);
        // playerAnimator.addAnimation(
        //     "walk",
        //     spriteSheet.createClip(
        //         {0, 1, 2, 3, 4, 5, 6, 7}, // Frame indices for walking animation
        //         0.1f, // Frame duration
        //         true   // Loop the animation
        //     )
        // );
        
        rectangleObject2 = new GE::GameObject();
        rectangleObject2->getTransform().setPosition(100.0f, 400.0f);
        rectangleObject2->addComponent<GE::RectangleRenderer>(
            200.0f,
            100.0f,
            GE::Color(255, 100, 100),
            rectangleObject2->getTransform()
        );
        //add collision box to rectangleObject2
        rectangleObject2->addComponent<GE::CollisionBox>(200.0f, 100.0f);
        addGameObject(rectangleObject2);

        rectangleObject3 = new GE::GameObject();
        rectangleObject3->getTransform().setPosition(550.0f, 100.0f);
        rectangleObject3->addComponent<GE::RectangleRenderer>(
            150.0f,
            250.0f,
            GE::Color(100, 255, 100),
            rectangleObject3->getTransform()
        );
        //add collision box to rectangleObject3
        rectangleObject3->addComponent<GE::CollisionBox>(150.0f, 250.0f);
        addGameObject(rectangleObject3);

        rectangleObject4 = new GE::GameObject();
        rectangleObject4->getTransform().setPosition(400.0f, 350.0f);
        rectangleObject4->addComponent<GE::RectangleRenderer>(
            120.0f,
            120.0f,
            GE::Color(255, 255, 100),
            rectangleObject4->getTransform()
        );
        //add collision box to rectangleObject4
        rectangleObject4->addComponent<GE::CollisionBox>(120.0f, 120.0f);
        rectangleObject4->setRenderOrder(100); // Set a higher render order for this object
        rectangleObject3->setRenderOrder(100); // Set a lower render order for this object
        rectangleObject2->setRenderOrder(100); // Set a lower render order for this object
        player->setRenderOrder(100); // Set a lower render order for this object

        addGameObject(rectangleObject4);

        addGameObject(player);
        characterController.setWorldObjects(&getGameObjects());

        setCameraTarget(player);

        //get the camera and set a larger viewport size to see more of the scene
        getCamera().setViewportSize(800, 600);
    }

    virtual ~MainScene()
    {
        removeGameObject(player);
        delete player;

        removeGameObject(rectangleObject2);
        delete rectangleObject2;

        removeGameObject(rectangleObject3);
        delete rectangleObject3;

        removeGameObject(rectangleObject4);
        delete rectangleObject4;
    }

    // Used by the engine/camera to follow the player rectangle.
    GE::GameObject* getPlayer() const { return player; }
};