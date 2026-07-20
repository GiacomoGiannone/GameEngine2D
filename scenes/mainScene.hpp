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
    GE::SpriteSheet* playerSpriteSheet;
    GE::SpriteSheet* playerSpriteSheetIdle;
    GE::SpriteSheet* playerSpriteSheetWalk;
    GE::SpriteSheet* playerSpriteSheetAttack;

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
            75.0f, // speed
            35.0f, // width of the collision box
            100.0f  // height of the collision box
        );
        //get the character controller to print debug info
        player->getComponentOfType<GE::CharacterController>().setDebugPrint(true);
        //set this variable to true to enable Y movement with W and S keys
        player->getComponentOfType<GE::CharacterController>().setYMovementEnabled(true);

        // Create sprite sheet and animator
        playerSpriteSheetWalk = new GE::SpriteSheet("player_walk", "assets/Walk_3.png", 
            162, 162, //dimensione originale di una singola cella
             21, 11, //punto iniziale dello sprite
             120, 140 ); //dimensione dello sprite
        playerSpriteSheetAttack = new GE::SpriteSheet("player_attack", "assets/Attack.png", 
            162, 162
            , 21, 11, //punto iniziale dello sprite
             120, 140 ); //dimensione dello sprite);
        playerSpriteSheetIdle = new GE::SpriteSheet("player_idle", "assets/Idle.png", 
            162, 162
            , 21, 11, //punto iniziale dello sprite
             120, 140 ); //dimensione dello sprite
        
            
        auto& playerAnimator = player->addComponent<GE::Animator>(spriteRenderer);
        characterController.setAnimator(&playerAnimator);

        playerAnimator.addAnimation(
            "walk",
            playerSpriteSheetWalk->createClip(
                {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}, // Frame indices for walking animation, till 25
                0.10f, // Frame duration
                true
            )
        );

        playerAnimator.addAnimation(
            "idle",
            playerSpriteSheetIdle->createClip(
                {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // Frame indices for idle animation, till 11
                0.10f, // Frame duration
                true
                   // Loop the animation
            )
        );

        playerAnimator.addAnimation(
            "attack",
            playerSpriteSheetAttack->createClip(
                {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, // Frame indices for attack animation, till 14
                0.10f, // Frame duration
                true
                  // Do not loop the animation
            )
        );

        playerAnimator.addTransition(
            "idle",
            "walk",
            [&characterController]() {
                return characterController.isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk",
            "idle",
            [&characterController]() {
                return !characterController.isMoving();
            }
        );

        playerAnimator.play("idle");
        
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

        delete playerSpriteSheetWalk;
        delete playerSpriteSheetAttack;
        delete playerSpriteSheetIdle;
    }

    // Used by the engine/camera to follow the player rectangle.
    GE::GameObject* getPlayer() const { return player; }
};