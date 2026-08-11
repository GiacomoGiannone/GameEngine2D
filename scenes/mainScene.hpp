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

class MainScene : public GE::Scene
{
private:
    GE::GameObject* player;
    
    GE::SpriteSheet* playerSpriteSheetIdle;
    GE::SpriteSheet* playerSpriteSheetWalk;
    GE::SpriteSheet* playerSpriteSheetAttack;
    GE::SpriteSheet* playerRunSpriteSheet;
    GE::SpriteSheet* playerAttack2SpriteSheet;
    GE::SpriteSheet* playerRollSpriteSheet;

    public:
    MainScene() : GE::Scene("MainScene")
    {
        // Player-controlled rectangle (camera will follow this).
        player = new GE::GameObject();
        player->getTransform().setPosition(250.0f, 180.0f);

        // Add sprite renderer to player FIRST (needed by Animator)
        // No initial texture is provided - the Animator will set the texture when an animation is played
        auto& spriteRenderer = player->addComponent<GE::SpriteRenderer>(player->getTransform());

        // Add character controller
        auto& characterController = player->addComponent<GE::CharacterController>(
            65.0f, // speed
            35.0f, // width of the collision box
            70.0f  // height of the collision box
        );
        characterController.setRollSpeed(85.0f); // Set the roll speed 
        //get the character controller to print debug info
        player->getComponentOfType<GE::CharacterController>().setDebugPrint(false);
        //set this variable to true to enable Y movement with W and S keys
        player->getComponentOfType<GE::CharacterController>().setYMovementEnabled(true);

        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
                                /*LOAD SPRITE SHEETS*/
        playerSpriteSheetWalk = new GE::SpriteSheet("player_walk", "assets/Walk_3.png", 
            162, 162, //dimensione originale di una singola cella
             0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        playerSpriteSheetAttack = new GE::SpriteSheet("player_attack", "assets/Attack.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite);
        playerSpriteSheetIdle = new GE::SpriteSheet("player_idle", "assets/Idle.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        playerRunSpriteSheet = new GE::SpriteSheet("player_run", "assets/Run.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        playerAttack2SpriteSheet = new GE::SpriteSheet("player_attack2", "assets/Attack2.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        playerRollSpriteSheet = new GE::SpriteSheet("player_roll", "assets/Roll.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////

        auto& playerAnimator = player->addComponent<GE::Animator>(spriteRenderer);
        characterController.setAnimator(&playerAnimator);

        GE::GameObject* playerPtr = player;
        GE::CharacterController* controllerPtr = &characterController;
        GE::Animator* animatorPtr = &playerAnimator;

        //add attack components to player
        sf::Vector2f lightAttackCollisionBox(80.0f, 20.0f); //width, height
        player->addComponent<GE::LightAttack>(10.0f, 1.0f, lightAttackCollisionBox); //damage, cooldown, collision box, id
        player->getComponentOfType<GE::LightAttack>().setDebugPrint(true); //set to true to see the attack collision box

        sf::Vector2f heavyAttackCollisionBox(70.0f, 100.0f); //width, height
        player->addComponent<GE::HeavyAttack>(25.0f, 2.0f, heavyAttackCollisionBox); //damage, cooldown, collision box, id
        player->getComponentOfType<GE::HeavyAttack>().setDebugPrint(true);
        player->addComponent<GE::Hittable>(100.0f); //add hittable component to player with 100 health

        playerAnimator.addAnimation(
            "walk",
            playerSpriteSheetWalk->createClip(
                {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}, // Frame indices for walking animation, till 25
                0.05f, // Frame duration
                true, // Do not loop the animation
                GE::PlaybackMode::Forward
            )
        );

        playerAnimator.addAnimation(
            "idle",
            playerSpriteSheetIdle->createClip(
                {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // Frame indices for idle animation, till 11
                0.05f, // Frame duration
                true, // Do not loop the animation
                GE::PlaybackMode::Forward
            )
        );

        auto attackClip = playerSpriteSheetAttack->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18}, // Frame indices for attack animation
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            0.5f // Set movement multiplier to 0.5 for attack animation
        );

        auto rollClip = playerRollSpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 
                 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71}, // Frame indices for roll animation till 71
            0.03f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            1.3f // Set movement multiplier to 1.3 for roll animation
        );

        auto secondAttackClip = playerAttack2SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 
            33, 34, 35, 36, 37, 38, 39, 40, 41}, // Frame indices for second attack animation till 41
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            0.5f // Set movement multiplier to 0.5 for second attack animation
        );

        auto runClip = playerRunSpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}, // Frame indices for run animation till 23
            0.04f, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            2.2f // Set movement multiplier to 2.5 for run animation
        );

        attackClip.events.push_back({8,8, 
            [playerPtr, controllerPtr]() 
            {
            // Execute attack logic when the last frame of the attack animation is reached
            playerPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
            }});

        rollClip.events.push_back({10,40, 
            [playerPtr]() 
            {
            // Execute roll logic
            playerPtr->getComponentOfType<GE::Hittable>().setInvincible(true); // Make the player invincible during the roll
            }});

        rollClip.events.push_back({41,41, 
            [playerPtr]() 
            {
            playerPtr->getComponentOfType<GE::Hittable>().setInvincible(false); // Make the player vulnerable again after the roll
            }});

        rollClip.events.push_back({0,0, 
            [controllerPtr]() 
            {
            // Lock the roll movement at the start of the roll
            controllerPtr->lockRollMovement();
            }});

        rollClip.events.push_back({71,71, 
            [controllerPtr]() 
            {
                controllerPtr->unlockRollMovement(); // Unlock the roll movement at the end of the roll
            }});
        secondAttackClip.events.push_back({16,16, 
            [playerPtr, controllerPtr]() 
            {
            // Execute heavy attack logic
            playerPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
            }});

        playerAnimator.addAnimation(
            "attack",
            attackClip
        );

        playerAnimator.addAnimation(
            "roll",
            rollClip
        );

        playerAnimator.addAnimation(
            "attack2",
            secondAttackClip
        );

        playerAnimator.addAnimation(
            "run",
            runClip
        );

        playerAnimator.addTransition(
            "idle",
            "walk",
            [controllerPtr]() {
                return controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk",
            "idle",
            [controllerPtr]() {
                return !controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk",
            "attack",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left);
            }
        );

        playerAnimator.addTransition(
            "idle",
            "attack",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left);
            }
        );

        playerAnimator.addTransition(
            "attack",
            "idle",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack").frames.size()) - 1;
            }
        );

        playerAnimator.addTransition(
            "attack",
            "walk",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk",
            "roll",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
            }
        );

        // playerAnimator.addTransition(
        //     "idle",
        //     "roll",
        //     [&characterController]() {
        //         return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
        //     }
        // );

        playerAnimator.addTransition(
            "roll",
            "idle",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll").frames.size()) - 1;
            }
        );

        playerAnimator.addTransition(
            "roll",
            "walk",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        // if press right mouse button, play second attack animation
        playerAnimator.addTransition(
            "idle",
            "attack2",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right);
            }
        );

        playerAnimator.addTransition(
            "walk",
            "attack2",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right);
            }
        );

        playerAnimator.addTransition(
            "attack2",
            "idle",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack2" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack2").frames.size()) - 1;
            }
        );

        playerAnimator.addTransition(
            "attack2",
            "walk",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack2" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack2").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk",
            "run",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);
            }
        );

        playerAnimator.addTransition(
            "run",
            "walk",
            [controllerPtr]() {
                return !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);
            }
        );

        playerAnimator.addTransition(
            "run",
            "roll",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
            }
        );

        //run to idle transition
        playerAnimator.addTransition(
            "run",
            "idle",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "run" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("run").frames.size()) - 1 &&
                !controllerPtr->isMoving();
            }
        );

        playerAnimator.play("idle");
        
        
        player->setRenderOrder(99); // Set a lower render order for this object


        addGameObject(player);
        characterController.setWorldObjects(&getGameObjects());

        setCameraTarget(player);

        //instatiate a levelLoader
        GE::Level level = GE::LevelLoader::load("assets/Levels/CastleLevel/Layout.txt");

        //build the level in the scene
        GE::LevelBuilder levelBuilder(this);
        levelBuilder.build(level);

        //get the camera and set a larger viewport size to see more of the scene
        getCamera().setViewportSize(800, 600);
    }

    virtual ~MainScene()
    {
        removeGameObject(player);
        delete player;

        delete playerSpriteSheetWalk;
        delete playerSpriteSheetAttack;
        delete playerSpriteSheetIdle;
    }

    // Used by the engine/camera to follow the player rectangle.
    GE::GameObject* getPlayer() const { return player; }
};