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

class MainScene : public GE::Scene
{
private:
    GE::GameObject* player;
    GE::Light playerLight;
    GE::LightingSystem lightingSystem;
    
    GE::SpriteSheet* Player_Down_Attack_1_SpriteSheet;
    GE::SpriteSheet* Player_Down_Attack_2_SpriteSheet;
    GE::SpriteSheet* Player_Down_Idle_SpriteSheet;
    GE::SpriteSheet* Player_Down_Roll_SpriteSheet;
    GE::SpriteSheet* Player_Down_Run_SpriteSheet;
    GE::SpriteSheet* Player_Down_Walk_SpriteSheet;
    
    GE::SpriteSheet* Player_Right_Attack_1_SpriteSheet;
    GE::SpriteSheet* Player_Right_Attack_2_SpriteSheet;
    GE::SpriteSheet* Player_Right_Idle_SpriteSheet;
    GE::SpriteSheet* Player_Right_Roll_SpriteSheet;
    GE::SpriteSheet* Player_Right_Run_SpriteSheet;
    GE::SpriteSheet* Player_Right_Walk_SpriteSheet;
   
    GE::SpriteSheet* Player_Up_Attack_1_SpriteSheet;
    GE::SpriteSheet* Player_Up_Attack_2_SpriteSheet;
    GE::SpriteSheet* Player_Up_Idle_SpriteSheet;
    GE::SpriteSheet* Player_Up_Roll_SpriteSheet;
    GE::SpriteSheet* Player_Up_Run_SpriteSheet;
    GE::SpriteSheet* Player_Up_Walk_SpriteSheet;

    GE::SpriteSheet* Player_Left_Attack_1_SpriteSheet;
    GE::SpriteSheet* Player_Left_Attack_2_SpriteSheet;
    GE::SpriteSheet* Player_Left_Idle_SpriteSheet;
    GE::SpriteSheet* Player_Left_Roll_SpriteSheet;
    GE::SpriteSheet* Player_Left_Run_SpriteSheet;
    GE::SpriteSheet* Player_Left_Walk_SpriteSheet;

public:
    MainScene() : GE::Scene("MainScene"), playerLight({250.0f, 180.0f}, 180.0f, 0.6f)
    {
        lightingSystem.setAmbientLight(0.15f);
        lightingSystem.addLight(playerLight);

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
        characterController.setControlledByPlayer(true);
        characterController.setRollSpeed(85.0f); // Set the roll speed 
        characterController.addIgnoredLayer("SkeletonEnemy"); // Ignore collisions with SkeletonEnemy layer
        //get the character controller to print debug info
        player->getComponentOfType<GE::CharacterController>().setDebugPrint(false);
        //set this variable to true to enable Y movement with W and S keys
        player->getComponentOfType<GE::CharacterController>().setYMovementEnabled(true);

        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
                                /*LOAD SPRITE SHEETS*/
        Player_Down_Attack_1_SpriteSheet = new GE::SpriteSheet("Player_Down_Attack_1_SpriteSheet", "assets/Player/Down_attack1.png", 
            162, 162, //dimensione originale di una singola cella
             0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Down_Attack_2_SpriteSheet = new GE::SpriteSheet("Player_Down_Attack_2_SpriteSheet", "assets/Player/Down_attack2.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite);
        Player_Down_Idle_SpriteSheet = new GE::SpriteSheet("Player_Down_Idle_SpriteSheet", "assets/Player/Down_idle.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Down_Roll_SpriteSheet = new GE::SpriteSheet("Player_Down_Roll_SpriteSheet", "assets/Player/Down_roll.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Down_Run_SpriteSheet = new GE::SpriteSheet("Player_Down_Run_SpriteSheet", "assets/Player/Down_run.png",
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Down_Walk_SpriteSheet = new GE::SpriteSheet("Player_Down_Walk_SpriteSheet", "assets/Player/Down_walk.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Right_Attack_1_SpriteSheet = new GE::SpriteSheet("Player_Right_Attack_1_SpriteSheet", "assets/Player/Right_attack1.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Right_Attack_2_SpriteSheet = new GE::SpriteSheet("Player_Right_Attack_2_SpriteSheet", "assets/Player/Right_attack2.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Right_Idle_SpriteSheet = new GE::SpriteSheet("Player_Right_Idle_SpriteSheet", "assets/Player/Right_idle.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Right_Roll_SpriteSheet = new GE::SpriteSheet("Player_Right_Roll_SpriteSheet", "assets/Player/Right_roll.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Right_Run_SpriteSheet = new GE::SpriteSheet("Player_Right_Run_SpriteSheet", "assets/Player/Right_run.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Right_Walk_SpriteSheet = new GE::SpriteSheet("Player_Right_Walk_SpriteSheet", "assets/Player/Right_walk.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Up_Attack_1_SpriteSheet = new GE::SpriteSheet("Player_Up_Attack_1_SpriteSheet", "assets/Player/Up_attack1.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite

        Player_Up_Attack_2_SpriteSheet = new GE::SpriteSheet("Player_Up_Attack_2_SpriteSheet", "assets/Player/Up_attack2.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Up_Idle_SpriteSheet = new GE::SpriteSheet("Player_Up_Idle_SpriteSheet", "assets/Player/Up_idle.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Up_Roll_SpriteSheet = new GE::SpriteSheet("Player_Up_Roll_SpriteSheet", "assets/Player/Up_roll.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Up_Run_SpriteSheet = new GE::SpriteSheet("Player_Up_Run_SpriteSheet", "assets/Player/Up_run.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Up_Walk_SpriteSheet = new GE::SpriteSheet("Player_Up_Walk_SpriteSheet", "assets/Player/Up_walk.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Left_Attack_1_SpriteSheet = new GE::SpriteSheet("Player_Left_Attack_1_SpriteSheet", "assets/Player/Left_attack1.png", 
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Left_Attack_2_SpriteSheet = new GE::SpriteSheet("Player_Left_Attack_2_SpriteSheet", "assets/Player/Left_attack2.png",
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Left_Idle_SpriteSheet = new GE::SpriteSheet("Player_Left_Idle_SpriteSheet", "assets/Player/Left_idle.png",
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Left_Roll_SpriteSheet = new GE::SpriteSheet("Player_Left_Roll_SpriteSheet", "assets/Player/Left_roll.png",
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Left_Run_SpriteSheet = new GE::SpriteSheet("Player_Left_Run_SpriteSheet", "assets/Player/Left_run.png",
            162, 162
            , 0, 0, //punto iniziale dello sprite
             162, 162 ); //dimensione dello sprite
        Player_Left_Walk_SpriteSheet = new GE::SpriteSheet("Player_Left_Walk_SpriteSheet", "assets/Player/Left_walk.png",
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

        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
                                /*CREATE ANIMATION CLIPS*/

        float runAnimationFrameDuration = 0.05f; // Set the frame duration for run animations
        float walkAnimationFrameDuration = 0.05f; // Set the frame duration for walk animations

        float runSpeedMultiplier = 1.8f; // Set the speed multiplier for run animations

        auto Player_Down_Attack_1_SpriteSheet_clip = Player_Down_Attack_1_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, // Frame indices for attack animation
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            0.5f // Set movement multiplier to 0.5 for attack animation
        );

        auto Player_Down_Attack_2_SpriteSheet_clip = Player_Down_Attack_2_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41}, // Frame indices for second attack animation
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            0.5f // Set movement multiplier to 0.5 for second attack animation
        );

        auto Player_Down_Idle_SpriteSheet_clip = Player_Down_Idle_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // Frame indices for idle animation
            0.1f, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to 1.0 for idle animation
        );

        auto Player_Down_Roll_SpriteSheet_clip = Player_Down_Roll_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55}, // Frame indices for roll animation till 55
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for roll animation
        );

        auto Player_Down_Run_SpriteSheet_clip = Player_Down_Run_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, // Frame indices for run animation
            runAnimationFrameDuration, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            runSpeedMultiplier // Set movement multiplier to faster speed for run animation
        );

        auto Player_Down_Walk_SpriteSheet_clip = Player_Down_Walk_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}, // Frame indices for walk animation till 23
            walkAnimationFrameDuration, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for walk animation
        );

        auto Player_Right_Attack_1_SpriteSheet_clip = Player_Right_Attack_1_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, // Frame indices for attack animation
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            0.5f // Set movement multiplier to slower speed for attack animation
        );

        auto Player_Right_Attack_2_SpriteSheet_clip = Player_Right_Attack_2_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41,}, // Frame indices for second attack animation
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            0.5f // Set movement multiplier to slower speed for second attack animation
        );

        auto Player_Right_Idle_SpriteSheet_clip = Player_Right_Idle_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // Frame indices for idle animation
            0.1f, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for idle animation
        );


        auto Player_Right_Roll_SpriteSheet_clip = Player_Right_Roll_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55}, // Frame indices for roll animation till frame index of the last frame till 55
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for roll animation
        );

        auto Player_Right_Run_SpriteSheet_clip = Player_Right_Run_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, // Frame indices for run animation
            runAnimationFrameDuration, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            runSpeedMultiplier // Set movement multiplier to faster speed for run animation
        );

        auto Player_Right_Walk_SpriteSheet_clip = Player_Right_Walk_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}, // Frame indices for walk animation till frame index of the last frame till 23
            walkAnimationFrameDuration, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for walk animation
        );

        auto Player_Up_Attack_1_SpriteSheet_clip = Player_Up_Attack_1_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, // Frame indices for attack animation
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            0.5f // Set movement multiplier to slower speed for attack animation
        );

        auto Player_Up_Attack_2_SpriteSheet_clip = Player_Up_Attack_2_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41}, // Frame indices for second attack animation
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            0.5f // Set movement multiplier to slower speed for second attack animation
        );

        auto Player_Up_Idle_SpriteSheet_clip = Player_Up_Idle_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // Frame indices for idle animation
            0.1f, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for idle animation
        );

        auto Player_Up_Roll_SpriteSheet_clip = Player_Up_Roll_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
            48, 49, 50, 51, 52, 53, 54, 55}, // Frame indices for roll animation till 55
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for roll animation
        );

        auto Player_Up_Run_SpriteSheet_clip = Player_Up_Run_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, // Frame indices for run animation
            runAnimationFrameDuration, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            runSpeedMultiplier // Set movement multiplier to faster speed for run animation
        );

        auto Player_Up_Walk_SpriteSheet_clip = Player_Up_Walk_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23}, // Frame indices for walk animation till frame index of the last frame till 23
            walkAnimationFrameDuration, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for walk animation
        );

        auto Player_Left_Attack_1_SpriteSheet_clip = Player_Left_Attack_1_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, // Frame indices for attack animation
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            0.5f // Set movement multiplier to slower speed for attack animation
        );

        auto Player_Left_Attack_2_SpriteSheet_clip = Player_Left_Attack_2_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41}, // Frame indices for second attack animation
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            0.5f // Set movement multiplier to slower speed for second attack animation
        );

        auto Player_Left_Idle_SpriteSheet_clip = Player_Left_Idle_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // Frame indices for idle animation
            0.1f, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for idle animation
        );

        auto Player_Left_Roll_SpriteSheet_clip = Player_Left_Roll_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
            50, 51, 52, 53, 54, 55}, // Frame indices for roll animation
            0.05f, // Frame duration
            false, // Do not loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for roll animation
        );

        auto Player_Left_Run_SpriteSheet_clip = Player_Left_Run_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, // Frame indices for run animation
            runAnimationFrameDuration, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            runSpeedMultiplier // Set movement multiplier to faster speed for run animation
        );

        auto Player_Left_Walk_SpriteSheet_clip = Player_Left_Walk_SpriteSheet->createClip(
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23}, // Frame indices for walk animation
            walkAnimationFrameDuration, // Frame duration
            true, // Loop the animation
            GE::PlaybackMode::Forward,
            1.0f // Set movement multiplier to normal speed for walk animation
        );

        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
                                    /*ADD EVENTS TO ANIMATION CLIPS*/

        Player_Down_Attack_1_SpriteSheet_clip.events.push_back({8,8, 
            [playerPtr, controllerPtr]() 
            {
            // Execute attack logic when the last frame of the attack animation is reached
            playerPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
            }});
        
        Player_Down_Attack_2_SpriteSheet_clip.events.push_back({16,16, 
            [playerPtr, controllerPtr]() 
            {
            // Execute heavy attack logic
            playerPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
            }});
        
        Player_Down_Roll_SpriteSheet_clip.events.push_back({10,40, 
            [playerPtr]() 
            {
            // Execute roll logic
            playerPtr->getComponentOfType<GE::Hittable>().setInvincible(true); // Make the player invincible during the roll
            }});

        Player_Down_Roll_SpriteSheet_clip.events.push_back({0,0, 
            [controllerPtr]() 
            {
            // Lock the roll movement at the start of the roll
            controllerPtr->lockRollMovement();
            }});
        
        Player_Down_Roll_SpriteSheet_clip.events.push_back({41,41, 
            [playerPtr]() 
            {
            playerPtr->getComponentOfType<GE::Hittable>().setInvincible(false); // Make the player vulnerable again after the roll
            }});

        Player_Down_Roll_SpriteSheet_clip.events.push_back({55,55, 
            [controllerPtr]() 
            {
                controllerPtr->unlockRollMovement(); // Unlock the roll movement at the end of the roll
            }});

        Player_Right_Attack_1_SpriteSheet_clip.events.push_back({8,8, 
            [playerPtr, controllerPtr]() 
            {
            // Execute attack logic when the last frame of the attack animation is reached
            playerPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
            }});

        Player_Right_Attack_2_SpriteSheet_clip.events.push_back({16,16,
            [playerPtr, controllerPtr]() 
            {
            // Execute heavy attack logic
            playerPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
            }});
        
        Player_Right_Roll_SpriteSheet_clip.events.push_back({10,40, 
            [playerPtr]()
            {
            // Execute roll logic
            playerPtr->getComponentOfType<GE::Hittable>().setInvincible(true); // Make the player invincible during the roll
            }});

        Player_Right_Roll_SpriteSheet_clip.events.push_back({0,0,
            [controllerPtr]() 
            {
            // Lock the roll movement at the start of the roll
            controllerPtr->lockRollMovement();
            }});

        Player_Right_Roll_SpriteSheet_clip.events.push_back({41,41,
            [playerPtr]()
            {
            playerPtr->getComponentOfType<GE::Hittable>().setInvincible(false); // Make the player vulnerable again after the roll
            }});

        Player_Right_Roll_SpriteSheet_clip.events.push_back({55,55,
            [controllerPtr]() 
            {
                controllerPtr->unlockRollMovement(); // Unlock the roll movement at the end of the roll
            }});

        Player_Up_Attack_1_SpriteSheet_clip.events.push_back({8,8, 
            [playerPtr, controllerPtr]() 
            {
            // Execute attack logic when the last frame of the attack animation is reached
            playerPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
            }});

        Player_Up_Attack_2_SpriteSheet_clip.events.push_back({16,16,
            [playerPtr, controllerPtr]()
            {
            // Execute heavy attack logic
            playerPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
            }});

        Player_Up_Roll_SpriteSheet_clip.events.push_back({10,40,
            [playerPtr]()
            {
            // Execute roll logic
            playerPtr->getComponentOfType<GE::Hittable>().setInvincible(true); // Make the player invincible during the roll
            }});

        Player_Up_Roll_SpriteSheet_clip.events.push_back({0,0,
            [controllerPtr]()
            {
            // Lock the roll movement at the start of the roll
            controllerPtr->lockRollMovement();
            }});

        Player_Up_Roll_SpriteSheet_clip.events.push_back({41,41,
            [playerPtr]()
            {
            playerPtr->getComponentOfType<GE::Hittable>().setInvincible(false); // Make the player vulnerable again after the roll
            }});

        Player_Up_Roll_SpriteSheet_clip.events.push_back({55,55,
            [controllerPtr]()
            {
                controllerPtr->unlockRollMovement(); // Unlock the roll movement at the end of the roll
            }});

        Player_Left_Attack_1_SpriteSheet_clip.events.push_back({8,8,
            [playerPtr, controllerPtr]()
            {
            // Execute attack logic when the last frame of the attack animation is reached
            playerPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
            }});

        Player_Left_Attack_2_SpriteSheet_clip.events.push_back({16,16,
            [playerPtr, controllerPtr]()
            {
            // Execute heavy attack logic
            playerPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
            }});

        Player_Left_Roll_SpriteSheet_clip.events.push_back({10,40,
            [playerPtr]()
            {
            // Execute roll logic
            playerPtr->getComponentOfType<GE::Hittable>().setInvincible(true); // Make the player invincible during the roll
            }});

        Player_Left_Roll_SpriteSheet_clip.events.push_back({0,0,
            [controllerPtr]()
            {
            // Lock the roll movement at the start of the roll
            controllerPtr->lockRollMovement();
            }});

        Player_Left_Roll_SpriteSheet_clip.events.push_back({41,41,
            [playerPtr]()
            {
            playerPtr->getComponentOfType<GE::Hittable>().setInvincible(false); // Make the player vulnerable again after the roll
            }});

        Player_Left_Roll_SpriteSheet_clip.events.push_back({55,55,
            [controllerPtr]()
            {
                controllerPtr->unlockRollMovement(); // Unlock the roll movement at the end of the roll
            }});
        
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
                                    /*ADD ANIMATIONS*/

        playerAnimator.addAnimation("idle_down", Player_Down_Idle_SpriteSheet_clip);
        playerAnimator.addAnimation("walk_down", Player_Down_Walk_SpriteSheet_clip);
        playerAnimator.addAnimation("run_down", Player_Down_Run_SpriteSheet_clip);
        playerAnimator.addAnimation("roll_down", Player_Down_Roll_SpriteSheet_clip);
        playerAnimator.addAnimation("attack_down", Player_Down_Attack_1_SpriteSheet_clip);
        playerAnimator.addAnimation("attack2_down", Player_Down_Attack_2_SpriteSheet_clip);

        playerAnimator.addAnimation("idle_right", Player_Right_Idle_SpriteSheet_clip);
        playerAnimator.addAnimation("walk_right", Player_Right_Walk_SpriteSheet_clip);
        playerAnimator.addAnimation("run_right", Player_Right_Run_SpriteSheet_clip);
        playerAnimator.addAnimation("roll_right", Player_Right_Roll_SpriteSheet_clip);
        playerAnimator.addAnimation("attack_right", Player_Right_Attack_1_SpriteSheet_clip);
        playerAnimator.addAnimation("attack2_right", Player_Right_Attack_2_SpriteSheet_clip);
        //idle to walk, walk to idle, idle to attack, attack to idle, walk to attack, attack to walk, idle to attack2, attack2 to idle, walk to attack2, attack2 to walk, walk to roll, roll to walk, walk to run, run to walk

        playerAnimator.addAnimation("idle_up", Player_Up_Idle_SpriteSheet_clip);
        playerAnimator.addAnimation("walk_up", Player_Up_Walk_SpriteSheet_clip);
        playerAnimator.addAnimation("run_up", Player_Up_Run_SpriteSheet_clip);
        playerAnimator.addAnimation("roll_up", Player_Up_Roll_SpriteSheet_clip);
        playerAnimator.addAnimation("attack_up", Player_Up_Attack_1_SpriteSheet_clip);
        playerAnimator.addAnimation("attack2_up", Player_Up_Attack_2_SpriteSheet_clip);

        playerAnimator.addAnimation("idle_left", Player_Left_Idle_SpriteSheet_clip);
        playerAnimator.addAnimation("walk_left", Player_Left_Walk_SpriteSheet_clip);
        playerAnimator.addAnimation("run_left", Player_Left_Run_SpriteSheet_clip);
        playerAnimator.addAnimation("roll_left", Player_Left_Roll_SpriteSheet_clip);
        playerAnimator.addAnimation("attack_left", Player_Left_Attack_1_SpriteSheet_clip);
        playerAnimator.addAnimation("attack2_left", Player_Left_Attack_2_SpriteSheet_clip);

        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
                                    /*MAKE TRANSITIONS*/

        //idle down to walk down
        playerAnimator.addTransition(
            "idle_down",
            "walk_down",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2; //1 for right, -1 for left, 2 for up, -2 for down
            }
        );

        //walk down to idle down
        playerAnimator.addTransition(
            "walk_down",
            "idle_down",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2;
            }
        );
        
        //idle down to attack down
        playerAnimator.addTransition(
            "idle_down",
            "attack_down",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left) && controllerPtr->getFacingDirection() == -2;
            }
        );

        //attack down to idle down
        playerAnimator.addTransition(
            "attack_down",
            "idle_down",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack_down").frames.size()) - 1;
            }
        );

        //walk down to attack down 
        playerAnimator.addTransition(
            "walk_down",
            "attack_down",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left) && controllerPtr->getFacingDirection() == -2;
            }
        );

        //attack down to walk down
        playerAnimator.addTransition(
            "attack_down",
            "walk_down",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack_down").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        //idle down to attack2 down
        playerAnimator.addTransition(
            "idle_down",
            "attack2_down",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right) && controllerPtr->getFacingDirection() == -2;
            }
        );

        //attack2 down to idle down
        playerAnimator.addTransition(
            "attack2_down",
            "idle_down",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack2_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack2_down").frames.size()) - 1;
            }
        );

        //walk down to attack2 down
        playerAnimator.addTransition(
            "walk_down",
            "attack2_down",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right) && controllerPtr->getFacingDirection() == -2;
            }
        );

        //attack2 down to walk down
        playerAnimator.addTransition(
            "attack2_down",
            "walk_down",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack2_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack2_down").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        //walk down to roll down
        playerAnimator.addTransition(
            "walk_down",
            "roll_down",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "roll_down",
            "run_down",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_down").frames.size()) - 1 &&
                controllerPtr->isMoving() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "roll_down",
            "walk_down",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_down").frames.size()) - 1 &&
                controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "roll_down",
            "idle_down",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_down").frames.size()) - 1 &&
                !controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "roll_down",
            "run_right",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_down").frames.size()) - 1 &&
                controllerPtr->isMoving() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "roll_down",
            "walk_right",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_down").frames.size()) - 1 &&
                controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "roll_down",
            "run_up",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_down").frames.size()) - 1 &&
                controllerPtr->isMoving() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "roll_down",
            "walk_up",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_down" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_down").frames.size()) - 1 &&
                controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        //walk down to run down 
        playerAnimator.addTransition(
            "walk_down",
            "run_down",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -2;
            }
        );

        //run down to walk down
        playerAnimator.addTransition(
            "run_down",
            "walk_down",
            [controllerPtr]() {
                return !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "run_down",
            "run_right",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "run_down",
            "run_up",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        //now right
        //idle to walk, walk to idle, idle to attack, attack to idle, walk to attack, attack to walk, 
        //idle to attack2, attack2 to idle, walk to attack2, attack2 to walk, walk to roll, roll to walk, walk to run, run to walk

        playerAnimator.addTransition(
            "idle_right",
            "walk_right",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1; //1 for right, -1 for left, 2 for up, -2 for down
            }
        );

        playerAnimator.addTransition(
            "walk_right",
            "idle_right",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "walk_right",
            "walk_down",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "walk_right",
            "walk_up",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "idle_right",
            "attack_right",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "attack_right",
            "idle_right",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack_right" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack_right").frames.size()) - 1;
            }
        );

        playerAnimator.addTransition(
            "walk_right",
            "attack_right",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "attack_right",
            "walk_right",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack_right" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack_right").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "idle_right",
            "attack2_right",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "attack2_right",
            "idle_right",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack2_right" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack2_right").frames.size()) - 1;
            }
        );

        playerAnimator.addTransition(
            "walk_right",
            "attack2_right",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "attack2_right",
            "walk_right",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack2_right" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack2_right").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk_right",
            "roll_right",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "roll_right",
            "walk_right",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_right" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_right").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk_right",
            "run_right",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "run_right",
            "walk_right",
            [controllerPtr]() {
                return !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "run_right",
            "run_down",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "run_right",
            "run_up",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "run_right",
            "roll_right",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "roll_right",
            "run_right",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_right" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_right").frames.size()) - 1 &&
                controllerPtr->isMoving() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "roll_right",
            "idle_right",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_right" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_right").frames.size()) - 1 &&
                !controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "roll_right",
            "walk_right",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_right" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_right").frames.size()) - 1 &&
                controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "roll_right",
            "run_right",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_right" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_right").frames.size()) - 1 &&
                controllerPtr->isMoving() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "roll_right",
            "idle_right",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_right" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_right").frames.size()) - 1 &&
                !controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        //now do up transitions

        playerAnimator.addTransition(
            "idle_up",
            "walk_up",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2; //1 for right, -1 for left, 2 for up, -2 for down
            }
        );

        playerAnimator.addTransition(
            "walk_up",
            "idle_up",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "idle_up",
            "attack_up",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "attack_up",
            "idle_up",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack_up" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack_up").frames.size()) - 1;
            }
        );

        playerAnimator.addTransition(
            "walk_up",
            "attack_up",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "attack_up",
            "walk_up",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack_up" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack_up").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "idle_up",
            "attack2_up",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "attack2_up",
            "idle_up",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack2_up" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack2_up").frames.size()) - 1;
            }
        );

        playerAnimator.addTransition(
            "walk_up",
            "attack2_up",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "attack2_up",
            "walk_up",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack2_up" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack2_up").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk_up",
            "roll_up",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "roll_up",
            "walk_up",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_up" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_up").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk_up",
            "run_up",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "run_up",
            "walk_up",
            [controllerPtr]() {
                return !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "run_up",
            "run_right",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "run_up",
            "run_down",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "run_up",
            "roll_up",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "roll_up",
            "run_up",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_up" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_up").frames.size()) - 1 &&
                controllerPtr->isMoving() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "roll_up",
            "idle_up",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_up" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_up").frames.size()) - 1 &&
                !controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "roll_up",
            "walk_up",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_up" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_up").frames.size()) - 1 &&
                controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "roll_up",
            "run_up",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_up" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_up").frames.size()) - 1 &&
                controllerPtr->isMoving() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "roll_up",
            "idle_up",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_up" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_up").frames.size()) - 1 &&
                !controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        // Entry points into left-direction states from every other direction.
        playerAnimator.addTransition(
            "idle_down",
            "idle_left",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "idle_right",
            "idle_left",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "idle_up",
            "idle_left",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "walk_down",
            "walk_left",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "walk_right",
            "walk_left",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "walk_up",
            "walk_left",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "run_down",
            "run_left",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "run_right",
            "run_left",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "run_up",
            "run_left",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        // now left
        playerAnimator.addTransition(
            "idle_left",
            "walk_left",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "idle_left",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "walk_down",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "walk_up",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "idle_left",
            "attack_left",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "attack_left",
            "idle_left",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack_left" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack_left").frames.size()) - 1;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "attack_left",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "attack_left",
            "walk_left",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack_left" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack_left").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "idle_left",
            "attack2_left",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "attack2_left",
            "idle_left",
            [animatorPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack2_left" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack2_left").frames.size()) - 1;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "attack2_left",
            [controllerPtr]() {
                return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "attack2_left",
            "walk_left",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "attack2_left" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("attack2_left").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "roll_left",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "roll_left",
            "walk_left",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_left" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_left").frames.size()) - 1 &&
                controllerPtr->isMoving();
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "run_left",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "run_left",
            "walk_left",
            [controllerPtr]() {
                return !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "run_left",
            "run_down",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "run_left",
            "run_up",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "run_left",
            "roll_left",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "roll_left",
            "run_left",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_left" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_left").frames.size()) - 1 &&
                controllerPtr->isMoving() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "roll_left",
            "idle_left",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_left" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_left").frames.size()) - 1 &&
                !controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "roll_left",
            "walk_left",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_left" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_left").frames.size()) - 1 &&
                controllerPtr->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -1;
            }
        );

        playerAnimator.addTransition(
            "roll_left",
            "run_left",
            [animatorPtr, controllerPtr]() {
                return animatorPtr->getCurrentAnimation() == "roll_left" && 
                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation("roll_left").frames.size()) - 1 &&
                controllerPtr->isMoving() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->getFacingDirection() == -1;
            }
        );

        // left to all other directions: when the facing direction changes, the state must enter the matching left/right/up/down animation
        playerAnimator.addTransition(
            "idle_left",
            "walk_right",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "idle_left",
            "walk_up",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "idle_left",
            "walk_down",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "idle_right",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "idle_up",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "idle_down",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "walk_right",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "walk_up",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "walk_left",
            "walk_down",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "run_left",
            "run_right",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "run_left",
            "run_up",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "run_left",
            "run_down",
            [controllerPtr]() {
                return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2;
            }
        );

        //walk right to walk left
        playerAnimator.addTransition(
            "walk_right",
            "walk_left",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        //idle right to walk left
        playerAnimator.addTransition(
            "idle_right",
            "walk_left",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        //idle down to walk left
        playerAnimator.addTransition(
            "idle_down",
            "walk_left",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );

        //idle up to walk left
        playerAnimator.addTransition(
            "idle_up",
            "walk_left",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -1;
            }
        );
        //idle down to walk right
        playerAnimator.addTransition(
            "idle_down",
            "walk_right",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1; //1 for right, -1 for left, 2 for up, -2 for down
            }
        );

        playerAnimator.addTransition(
            "walk_right",
            "idle_down",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1;
            }
        );

        //idle down to walk up
        playerAnimator.addTransition(
            "idle_down",
            "walk_up",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2; //1 for right, -1 for left, 2 for up, -2 for down
            }
        );

        playerAnimator.addTransition(
            "walk_up",
            "idle_down",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2;
            }
        );

        //idle up to walk right
        playerAnimator.addTransition(
            "idle_up",
            "walk_right",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1; //1 for right, -1 for left, 2 for up, -2 for down
            }
        );

        playerAnimator.addTransition(
            "walk_right",
            "idle_up",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1;
            }
        );

        //idle up to walk down
        playerAnimator.addTransition(
            "idle_up",
            "walk_down",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2; //1 for right, -1 for left, 2 for up, -2 for down
            }
        );

        playerAnimator.addTransition(
            "walk_down",
            "idle_up",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2;
            }
        );

        //idle right to walk down
        playerAnimator.addTransition(
            "idle_right",
            "walk_down",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2; //1 for right, -1 for left, 2 for up, -2 for down
            }
        );

        playerAnimator.addTransition(
            "walk_down",
            "idle_right",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2;
            }
        );

        playerAnimator.addTransition(
            "walk_down",
            "walk_right",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "walk_down",
            "walk_up",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2;
            }
        );

        //idle right to walk up
        playerAnimator.addTransition(
            "idle_right",
            "walk_up",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2; //1 for right, -1 for left, 2 for up, -2 for down
            }
        );

        playerAnimator.addTransition(
            "walk_up",
            "idle_right",
            [controllerPtr]() {
                return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 2;
            }
        );

        playerAnimator.addTransition(
            "walk_up",
            "walk_right",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == 1;
            }
        );

        playerAnimator.addTransition(
            "walk_up",
            "walk_down",
            [controllerPtr]() {
                return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == -2;
            }
        );

        


        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////

        playerAnimator.play("idle_down"); // Start with the idle_down animation
        
        
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

        //spawn some enemies
        // GE::EnemyManager::getInstance()->spawn(400, 400, 50);
        // GE::EnemyManager::getInstance()->spawn(1150, 400, 50);

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

        playerLight.setPosition({player->getX(), player->getY()});
        lightingSystem.update(getGameObjects());
    }

    virtual ~MainScene()
    {
        removeGameObject(player);
        delete player;

        delete Player_Down_Attack_1_SpriteSheet;
        delete Player_Down_Attack_2_SpriteSheet;
        delete Player_Down_Idle_SpriteSheet;
        delete Player_Down_Roll_SpriteSheet;
        delete Player_Down_Run_SpriteSheet;
        delete Player_Down_Walk_SpriteSheet;
        delete Player_Right_Attack_1_SpriteSheet;
        delete Player_Right_Attack_2_SpriteSheet;
        delete Player_Right_Idle_SpriteSheet;
        delete Player_Right_Roll_SpriteSheet;
        delete Player_Right_Run_SpriteSheet;
        delete Player_Right_Walk_SpriteSheet;
        delete Player_Up_Attack_1_SpriteSheet;
        delete Player_Up_Attack_2_SpriteSheet;
        delete Player_Up_Idle_SpriteSheet;
        delete Player_Up_Roll_SpriteSheet;
        delete Player_Up_Run_SpriteSheet;
        delete Player_Up_Walk_SpriteSheet;
        delete Player_Left_Attack_1_SpriteSheet;
        delete Player_Left_Attack_2_SpriteSheet;
        delete Player_Left_Idle_SpriteSheet;
        delete Player_Left_Roll_SpriteSheet;
        delete Player_Left_Run_SpriteSheet;
        delete Player_Left_Walk_SpriteSheet;
    }

    // Used by the engine/camera to follow the player rectangle.
    GE::GameObject* getPlayer() const { return player; }
};