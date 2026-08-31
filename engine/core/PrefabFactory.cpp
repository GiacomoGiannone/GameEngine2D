#include "PrefabFactory.hpp"

#include "GameObject.hpp"
#include "Graphics/SpriteRenderer.hpp"
#include "Core/CollisionBox.hpp"
#include "Graphics/SpriteSheet.hpp"
#include "Graphics/Animator.hpp"
#include "Core/CharacterController.hpp"
#include "Core/EnemyAI.hpp"
#include "Core/LightAttack.hpp"
#include "Core/HeavyAttack.hpp"
#include "Core/Hittable.hpp"
#include "Core/Engine.hpp"
#include "Core/Scene.hpp"
#include "Core/Input.hpp"
#include "GameSpecificLibs/Chest.hpp"
#include "core/UpAttack.hpp"

#include <cmath>
#include <utility>

namespace GE
{
    GameObject* PrefabFactory::create(const std::string& prefabName, float x_position, float y_position, int renderOrder)
    {
        if(prefabName == "Wall_1")
        {
            GameObject* wall = new GameObject();
            wall->getTransform().setPosition(x_position, y_position);
            wall->setRenderOrder(renderOrder);
            //add a texture renderer component to the wall
            auto& wallRenderer = wall->addComponent<GE::SpriteRenderer>("Textures/Wall_1.png", "assets/Textures/Wall_1.png", wall->getTransform());
            //add a collision box to the wall using the texture dimensions
            auto wallTextureSize = wallRenderer.getTexture().getSize();
            wall->addComponent<GE::CollisionBox>(static_cast<float>(wallTextureSize.x), static_cast<float>(wallTextureSize.y));
            return wall;
        }

        if(prefabName == "Wall_2")
        {
            GameObject* wall = new GameObject();
            wall->getTransform().setPosition(x_position, y_position);
            wall->setRenderOrder(renderOrder);
            //add a texture renderer component to the wall
            auto& wallRenderer = wall->addComponent<GE::SpriteRenderer>("Textures/Wall_2.png", "assets/Textures/Wall_2.png", wall->getTransform());
            //add a collision box to the wall using the texture dimensions
            auto wallTextureSize = wallRenderer.getTexture().getSize();
            wall->addComponent<GE::CollisionBox>(static_cast<float>(wallTextureSize.x), static_cast<float>(wallTextureSize.y));
            return wall;
        }

        if(prefabName == "Floor_1")
        {
            GameObject* floor = new GameObject();
            floor->getTransform().setPosition(x_position, y_position);
            floor->setRenderOrder(renderOrder);
            //add a texture renderer component to the floor
            floor->addComponent<GE::SpriteRenderer>("Textures/Floor_1.png", "assets/Textures/Floor_1.png", floor->getTransform());
            return floor;
        }

        if(prefabName == "Floor_2")
        {
            GameObject* floor = new GameObject();
            floor->getTransform().setPosition(x_position, y_position);
            floor->setRenderOrder(renderOrder);
            //add a texture renderer component to the floor
            floor->addComponent<GE::SpriteRenderer>("Textures/Floor_2.png", "assets/Textures/Floor_2.png", floor->getTransform());
            return floor;
        }

        if(prefabName == "SkeletonEnemy")
        {
            GameObject* skeletonEnemy = new GameObject();
            skeletonEnemy->getTransform().setPosition(x_position, y_position);
            skeletonEnemy->setRenderOrder(renderOrder);
            skeletonEnemy->setLayerName("SkeletonEnemy"); // Set the layer name for the skeleton enemy
            //add a texture renderer component to the skeleton enemy
            skeletonEnemy->addComponent<GE::SpriteRenderer>(skeletonEnemy->getTransform());
            //add a collision box to the skeleton enemy using the texture dimensions
            auto& skeletonRenderer = skeletonEnemy->getComponentOfType<GE::SpriteRenderer>();
            auto skeletonTextureSize = skeletonRenderer.getTexture().getSize();
            //add animator
            skeletonEnemy->addComponent<GE::Animator>(skeletonRenderer);
            auto& animator = skeletonEnemy->getComponentOfType<GE::Animator>();
            //add characterController
            auto& characterController = skeletonEnemy->addComponent<GE::CharacterController>(
                50.0f, //speed (slower than the player)
                35.0f, //width of the collision box
                70.0f  //height of the collision box
            );

            //add a collision so that the player cant run over the skeleton enemy
            skeletonEnemy->addComponent<GE::CollisionBox>(35.0f, 70.0f); //width, height
            skeletonEnemy->getComponentOfType<GE::CollisionBox>().setDebugPrint(false);
            //link the controller to the animator so movement multipliers apply (same as the player)
            characterController.setAnimator(&animator);
            //set the character controller to not be controlled by the player
            characterController.setControlledByPlayer(false);
            //let the AI drive the skeleton on both axes (EnemyAI issues cardinal move intents)
            characterController.setYMovementEnabled(true);

            //pointers used by the animation event/transition lambdas (same pattern as the player in mainScene)
            GE::GameObject* enemyPtr = skeletonEnemy;
            GE::CharacterController* controllerPtr = &characterController;
            GE::Animator* animatorPtr = &animator;

            //add attack components so the animation events can execute attacks (mirrors the player's setup)
            sf::Vector2f lightAttackCollisionBox(70.0f, 25.0f); //width, height
            skeletonEnemy->addComponent<GE::LightAttack>(7.0f, 5.0f, lightAttackCollisionBox); //damage, cooldown, collision box
            sf::Vector2f heavyAttackCollisionBox(60.0f, 90.0f); //width, height
            skeletonEnemy->addComponent<GE::HeavyAttack>(15.0f, 7.0f, heavyAttackCollisionBox); //damage, cooldown, collision box
            //make the skeleton damageable by the player's attacks
            skeletonEnemy->addComponent<GE::Hittable>(35.0f); //health

            //print debug the attack collision
            skeletonEnemy->getComponentOfType<GE::LightAttack>().setDebugPrint(false);
            skeletonEnemy->getComponentOfType<GE::HeavyAttack>().setDebugPrint(false);

            //add a simple chase AI that drives CharacterController via move intent
            skeletonEnemy->addComponent<GE::EnemyAI>();

            //load spriteSheets
            GE::SpriteSheet* Down_Attack1_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Down_Attack_1_SpriteSheet", "assets/Enemy1/Down_attack1.png", 
                162, 162, //dimensione originale di una singola cella
                 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            GE::SpriteSheet* Down_Attack2_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Down_Attack_2_SpriteSheet", "assets/Enemy1/Down_attack2.png", 
                162, 162
                , 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite);
            GE::SpriteSheet* Down_Idle_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Down_Idle_SpriteSheet", "assets/Enemy1/Down_idle.png", 
                162, 162
                , 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            GE::SpriteSheet* Down_walk_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Down_Walk_SpriteSheet", "assets/Enemy1/Down_walk.png", 
                162, 162
                , 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            //now left
            GE::SpriteSheet* Left_Attack1_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Left_Attack_1_SpriteSheet", "assets/Enemy1/Left_attack1.png", 
                162, 162, //dimensione originale di una singola cella
                 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            GE::SpriteSheet* Left_Attack2_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Left_Attack_2_SpriteSheet", "assets/Enemy1/Left_attack2.png", 
                162, 162
                , 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite);
            GE::SpriteSheet* Left_Idle_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Left_Idle_SpriteSheet", "assets/Enemy1/Left_idle.png",
                162, 162
                , 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            GE::SpriteSheet* Left_walk_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Left_Walk_SpriteSheet", "assets/Enemy1/Left_walk.png",
                162, 162
                , 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            //now right
            GE::SpriteSheet* Right_Attack1_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Right_Attack_1_SpriteSheet", "assets/Enemy1/Right_attack1.png", 
                162, 162, //dimensione originale di una singola cella
                 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            GE::SpriteSheet* Right_Attack2_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Right_Attack_2_SpriteSheet", "assets/Enemy1/Right_attack2.png",
                162, 162
                , 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite);
            GE::SpriteSheet* Right_Idle_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Right_Idle_SpriteSheet", "assets/Enemy1/Right_idle.png",
                162, 162
                , 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            GE::SpriteSheet* Right_walk_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Right_Walk_SpriteSheet", "assets/Enemy1/Right_walk.png",
                162, 162
                , 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            //now up
            GE::SpriteSheet* Up_Attack1_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Up_Attack_1_SpriteSheet", "assets/Enemy1/Up_attack1.png", 
                162, 162, //dimensione originale di una singola cella
                 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            GE::SpriteSheet* Up_Attack2_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Up_Attack_2_SpriteSheet", "assets/Enemy1/Up_attack2.png",
                162, 162
                , 0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite);
            GE::SpriteSheet* Up_Idle_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Up_Idle_SpriteSheet", "assets/Enemy1/Up_idle.png",
                162, 162,
                0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite
            GE::SpriteSheet* Up_walk_SpriteSheet = new GE::SpriteSheet("SkeletonEnemy_Up_Walk_SpriteSheet", "assets/Enemy1/Up_walk.png",
                162, 162,
                0, 0, //punto iniziale dello sprite
                 162, 162 ); //dimensione dello sprite

            //now make the clips
            auto Down_Attack1_SpriteSheet_clip = Down_Attack1_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17}, 
                                                                                        0.1f, false);
            auto Down_Attack2_SpriteSheet_clip = Down_Attack2_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, //till 41
                                                                                        18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 
                                                                                        33, 34, 35, 36, 37, 38, 39, 40, 41}, 
                                                                                        0.04f, false);
            auto Down_Idle_SpriteSheet_clip = Down_Idle_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 
                                                                                    0.1f, true);
            auto Down_walk_SpriteSheet_clip = Down_walk_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}, 
                                                                                    0.1f, true);
            
            auto Left_Attack1_SpriteSheet_clip = Left_Attack1_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17}, 
                                                                                        0.1f, false);
            auto Left_Attack2_SpriteSheet_clip = Left_Attack2_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, //till 41
                                                                                        18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
                                                                                        33, 34, 35, 36, 37, 38, 39, 40, 41},
                                                                                        0.04f, false);
            auto Left_Idle_SpriteSheet_clip = Left_Idle_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 
                                                                                    0.1f, true);
            auto Left_walk_SpriteSheet_clip = Left_walk_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 
                                                                                    15, 16, 17, 18, 19, 20, 21, 22, 23},
                                                                                    0.04f, true);
            auto Right_Attack1_SpriteSheet_clip = Right_Attack1_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17}, 
                                                                                        0.1f, false);
            auto Right_Attack2_SpriteSheet_clip = Right_Attack2_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, //till 41
                                                                                        18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
                                                                                        33, 34, 35, 36, 37, 38, 39, 40, 41},
                                                                                        0.04f, false);
            auto Right_Idle_SpriteSheet_clip = Right_Idle_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 
                                                                                    0.1f, true);    
            auto Right_walk_SpriteSheet_clip = Right_walk_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 
                                                                                    15, 16, 17, 18, 19, 20, 21, 22, 23},
                                                                                    0.04f, true);
            auto Up_Attack1_SpriteSheet_clip = Up_Attack1_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17}, 
                                                                                        0.04f, false);
            auto Up_Attack2_SpriteSheet_clip = Up_Attack2_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, //till 41
                                                                                        18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
                                                                                        33, 34, 35, 36, 37, 38, 39, 40, 41},
                                                                                        0.04f, false);
            auto Up_Idle_SpriteSheet_clip = Up_Idle_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                                                                                    0.04f, true);
            auto Up_walk_SpriteSheet_clip = Up_walk_SpriteSheet->createClip({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                                                                                    15, 16, 17, 18, 19, 20, 21, 22, 23},
                                                                                    0.04f, true);

            ///////////////////////////////////////////////////////////////////////////////////////////
                                /*ADD EVENTS TO ANIMATION CLIPS*/

            //Attack events fire on a key frame of each attack clip, exactly like the player's setup.
            //The skeleton has no roll clips, so there are no roll events.

            //down
            //lock facing on frame 0 of each attack clip, unlock facing on the last frame of each attack clip
            Down_Attack1_SpriteSheet_clip.events.push_back({0, 0, 
            [&]() 
            {
                characterController.lockFacing();
            }});
            Down_Attack1_SpriteSheet_clip.events.push_back({8,8,
                [enemyPtr, controllerPtr]()
                {
                // Execute light attack logic when the attack frame is reached
                enemyPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Down_Attack1_SpriteSheet_clip.events.push_back({17, 17,
            [&]() 
            {
                characterController.unlockFacing();
            }});

            Down_Attack2_SpriteSheet_clip.events.push_back({16,16,
                [enemyPtr, controllerPtr]()
                {
                // Execute heavy attack logic
                enemyPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
                }});

            //now left
            Left_Attack1_SpriteSheet_clip.events.push_back({8,8,
                [enemyPtr, controllerPtr]()
                {
                // Execute light attack logic when the attack frame is reached
                enemyPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Left_Attack2_SpriteSheet_clip.events.push_back({16,16,
                [enemyPtr, controllerPtr]()
                {
                // Execute heavy attack logic
                enemyPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
                }});

            //now right
            Right_Attack1_SpriteSheet_clip.events.push_back({8,8,
                [enemyPtr, controllerPtr]()
                {
                // Execute light attack logic when the attack frame is reached
                enemyPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Right_Attack2_SpriteSheet_clip.events.push_back({16,16,
                [enemyPtr, controllerPtr]()
                {
                // Execute heavy attack logic
                enemyPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
                }});

            //now up
            Up_Attack1_SpriteSheet_clip.events.push_back({8,8,
                [enemyPtr, controllerPtr]()
                {
                // Execute light attack logic when the attack frame is reached
                enemyPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Up_Attack2_SpriteSheet_clip.events.push_back({16,16,
                [enemyPtr, controllerPtr]()
                {
                // Execute heavy attack logic
                enemyPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
                }});

            animator.addAnimation("Down_Attack1", Down_Attack1_SpriteSheet_clip);
            animator.addAnimation("Down_Attack2", Down_Attack2_SpriteSheet_clip);
            animator.addAnimation("Down_Idle", Down_Idle_SpriteSheet_clip);
            animator.addAnimation("Down_Walk", Down_walk_SpriteSheet_clip);
            animator.addAnimation("Left_Attack1", Left_Attack1_SpriteSheet_clip);
            animator.addAnimation("Left_Attack2", Left_Attack2_SpriteSheet_clip);
            animator.addAnimation("Left_Idle", Left_Idle_SpriteSheet_clip);
            animator.addAnimation("Left_Walk", Left_walk_SpriteSheet_clip);
            animator.addAnimation("Right_Attack1", Right_Attack1_SpriteSheet_clip);
            animator.addAnimation("Right_Attack2", Right_Attack2_SpriteSheet_clip);
            animator.addAnimation("Right_Idle", Right_Idle_SpriteSheet_clip);
            animator.addAnimation("Right_Walk", Right_walk_SpriteSheet_clip);
            animator.addAnimation("Up_Attack1", Up_Attack1_SpriteSheet_clip);
            animator.addAnimation("Up_Attack2", Up_Attack2_SpriteSheet_clip);
            animator.addAnimation("Up_Idle", Up_Idle_SpriteSheet_clip);
            animator.addAnimation("Up_Walk", Up_walk_SpriteSheet_clip);

            ///////////////////////////////////////////////////////////////////////////////////////////
                                /*MAKE TRANSITIONS*/

            //The skeleton is AI-driven (EnemyAI drives CharacterController via moveIntent), so instead of
            //mouse/keyboard input the attack states are entered based on the distance to the player-controlled
            //target, while idle/walk states follow movement and facing exactly like the player's setup.
            const float lightAttackRange = 90.0f; //enter Attack1 when the target is within this distance
            const float heavyAttackRange = 60.0f; //enter Attack2 when the target is within this distance

            //returns true when the player-controlled target is within 'range' units of this skeleton
            //(same scene-scan pattern used by EnemyAI::findPlayerTarget and Attack::execute)
            auto isTargetWithinRange = [enemyPtr](float range) {
                Engine* engine = Engine::getInstance();
                Scene* scene = (engine != nullptr) ? engine->getCurrentScene() : nullptr;
                if (scene == nullptr)
                {
                    return false;
                }

                for (GameObject* object : scene->getGameObjects())
                {
                    if (!object->hasComponentOfType<CharacterController>())
                    {
                        continue;
                    }

                    auto& objectController = object->getComponentOfType<CharacterController>();
                    if (!objectController.isControlledByPlayer())
                    {
                        continue;
                    }

                    const float dx = object->getX() - enemyPtr->getX();
                    const float dy = object->getY() - enemyPtr->getY();
                    return std::sqrt(dx * dx + dy * dy) <= range;
                }

                return false;
            };

            //facing codes: 1 for right, -1 for left, 2 for up, -2 for down
            const std::pair<const char*, int> directions[] = {
                {"Down", -2}, {"Right", 1}, {"Up", 2}, {"Left", -1}
            };

            for (const auto& direction : directions)
            {
                const std::string idleName = std::string(direction.first) + "_Idle";
                const std::string walkName = std::string(direction.first) + "_Walk";
                const std::string attack1Name = std::string(direction.first) + "_Attack1";
                const std::string attack2Name = std::string(direction.first) + "_Attack2";

                //idle to walk / walk to idle (same behaviour as the player)
                animator.addTransition(
                    idleName, walkName,
                    [controllerPtr, facing = direction.second]() {
                        return controllerPtr->isMoving() && controllerPtr->getFacingDirection() == facing;
                    }
                );
                animator.addTransition(
                    walkName, idleName,
                    [controllerPtr, facing = direction.second]() {
                        return !controllerPtr->isMoving() && controllerPtr->getFacingDirection() == facing;
                    }
                );

                //walk/idle to attack2 (heavy, closest range). Registered before the light attack so the
                //heavy attack wins when the target is inside both ranges (mirrors right vs left click).
                animator.addTransition(
                    walkName, attack2Name,
                    [enemyPtr, controllerPtr, isTargetWithinRange, heavyAttackRange, facing = direction.second]() {
                        return isTargetWithinRange(heavyAttackRange) 
                            && controllerPtr->getFacingDirection() == facing
                            && !enemyPtr->getComponentOfType<GE::HeavyAttack>().isOnCooldown();   // <-- aggiungi
                    }
                );
                animator.addTransition(
                    idleName, attack2Name,
                    [enemyPtr, controllerPtr, isTargetWithinRange, heavyAttackRange, facing = direction.second]() {
                        return isTargetWithinRange(heavyAttackRange) 
                            && controllerPtr->getFacingDirection() == facing
                            && !enemyPtr->getComponentOfType<GE::HeavyAttack>().isOnCooldown();   // <-- aggiungi
                    }
                );

                animator.addTransition(
                    walkName, attack1Name,
                    [enemyPtr, controllerPtr, isTargetWithinRange, lightAttackRange, facing = direction.second]() {
                        return isTargetWithinRange(lightAttackRange) 
                            && controllerPtr->getFacingDirection() == facing
                            && !enemyPtr->getComponentOfType<GE::LightAttack>().isOnCooldown();   // <-- aggiungi
                    }
                );
                animator.addTransition(
                    idleName, attack1Name,
                    [enemyPtr, controllerPtr, isTargetWithinRange, lightAttackRange, facing = direction.second]() {
                        return isTargetWithinRange(lightAttackRange) 
                            && controllerPtr->getFacingDirection() == facing
                            && !enemyPtr->getComponentOfType<GE::LightAttack>().isOnCooldown();   // <-- aggiungi
                    }
                );

                //attack1 back to idle/walk when the last frame of the attack is reached
                animator.addTransition(
                    attack1Name, idleName,
                    [animatorPtr, controllerPtr, animName = attack1Name]() {
                        return animatorPtr->getCurrentAnimation() == animName &&
                        animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation(animName).frames.size()) - 1 &&
                        !controllerPtr->isMoving();
                    }
                );
                animator.addTransition(
                    attack1Name, walkName,
                    [animatorPtr, controllerPtr, animName = attack1Name]() {
                        return animatorPtr->getCurrentAnimation() == animName &&
                        animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation(animName).frames.size()) - 1 &&
                        controllerPtr->isMoving();
                    }
                );

                //attack2 back to idle/walk when the last frame of the attack is reached
                animator.addTransition(
                    attack2Name, idleName,
                    [animatorPtr, controllerPtr, animName = attack2Name]() {
                        return animatorPtr->getCurrentAnimation() == animName &&
                        animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation(animName).frames.size()) - 1 &&
                        !controllerPtr->isMoving();
                    }
                );
                animator.addTransition(
                    attack2Name, walkName,
                    [animatorPtr, controllerPtr, animName = attack2Name]() {
                        return animatorPtr->getCurrentAnimation() == animName &&
                        animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation(animName).frames.size()) - 1 &&
                        controllerPtr->isMoving();
                    }
                );
            }

            //cross-direction entries: when the facing direction changes while moving (or standing still),
            //enter the matching walk/idle state of the new direction (same idea as the player's setup)
            for (const auto& fromDirection : directions)
            {
                for (const auto& toDirection : directions)
                {
                    if (fromDirection.second == toDirection.second)
                        continue;

                    const std::string fromWalk =
                        std::string(fromDirection.first) + "_Walk";

                    const std::string fromIdle =
                        std::string(fromDirection.first) + "_Idle";

                    const std::string toWalk =
                        std::string(toDirection.first) + "_Walk";

                    const std::string toIdle =
                        std::string(toDirection.first) + "_Idle";

                    // Walk → Walk
                    animator.addTransition(
                        fromWalk,
                        toWalk,
                        [controllerPtr, facing = toDirection.second]() {
                            return controllerPtr->isMoving() &&
                                controllerPtr->getFacingDirection() == facing;
                        }
                    );

                    // Idle → Idle
                    animator.addTransition(
                        fromIdle,
                        toIdle,
                        [controllerPtr, facing = toDirection.second]() {
                            return !controllerPtr->isMoving() &&
                                controllerPtr->getFacingDirection() == facing;
                        }
                    );

                    // ⭐ Idle → Walk
                    animator.addTransition(
                        fromIdle,
                        toWalk,
                        [controllerPtr, facing = toDirection.second]() {
                            return controllerPtr->isMoving() &&
                                controllerPtr->getFacingDirection() == facing;
                        }
                    );
                }
            }

            animator.play("Down_Idle"); // Start with the Down_Idle animation
            
            return skeletonEnemy;
        }

        if(prefabName == "Player")
        {
            GameObject* player;
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

            //add hittable component to the player so the skeleton can damage it
            player->addComponent<GE::Hittable>(600.0f); //health
            
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
            player->addComponent<GE::LightAttack>(20.0f, 1.0f, lightAttackCollisionBox); //damage, cooldown, collision box, id
            player->getComponentOfType<GE::LightAttack>().setDebugPrint(false); //set to true to see the attack collision box

            sf::Vector2f heavyAttackCollisionBox(70.0f, 100.0f); //width, height
            player->addComponent<GE::HeavyAttack>(35.0f, 2.0f, heavyAttackCollisionBox); //damage, cooldown, collision box, id
            player->getComponentOfType<GE::HeavyAttack>().setDebugPrint(false);
            player->addComponent<GE::Hittable>(100.0f); //add hittable component to player with 100 health

            sf::Vector2f UpwardAttackCollisionBox(20.0f, 80.0f); //width, height
            player->addComponent<GE::UpAttack>(20.0f, 1.0f, UpwardAttackCollisionBox); //damage, cooldown, collision box, id
            player->getComponentOfType<GE::UpAttack>().setDebugPrint(false);

            ///////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////////////////////
                                    /*CREATE ANIMATION CLIPS*/

            float runAnimationFrameDuration = 0.05f; // Set the frame duration for run animations
            float walkAnimationFrameDuration = 0.05f; // Set the frame duration for walk animations
            float rollAnimationFrameDuration = 0.04f; // Set the frame duration for roll animations
            float heavyAttackAnimationFrameDuration = 0.04f; // Set the frame duration for heavy attack animations

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
                heavyAttackAnimationFrameDuration, // Frame duration
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
                rollAnimationFrameDuration, // Frame duration
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
                heavyAttackAnimationFrameDuration, // Frame duration
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
                rollAnimationFrameDuration, // Frame duration
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
                heavyAttackAnimationFrameDuration, // Frame duration
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
                rollAnimationFrameDuration, // Frame duration
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
                heavyAttackAnimationFrameDuration, // Frame duration
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
                rollAnimationFrameDuration, // Frame duration
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

            Player_Down_Attack_1_SpriteSheet_clip.events.push_back({0,0, 
                [&]() 
                {
                    characterController.lockFacing();
                }});
            Player_Down_Attack_1_SpriteSheet_clip.events.push_back({8,8, 
                [playerPtr, controllerPtr]() 
                {
                // Execute attack logic when the last frame of the attack animation is reached
                playerPtr->getComponentOfType<GE::UpAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Player_Down_Attack_1_SpriteSheet_clip.events.push_back({17,17, 
                [&]() 
                {
                    characterController.unlockFacing();
                }});
            
            Player_Down_Attack_2_SpriteSheet_clip.events.push_back({0,0, 
                [&]() 
                {
                    characterController.lockFacing();
                }});
            Player_Down_Attack_2_SpriteSheet_clip.events.push_back({16,16, 
                [playerPtr, controllerPtr]() 
                {
                // Execute heavy attack logic
                playerPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Player_Down_Attack_2_SpriteSheet_clip.events.push_back({41,41, 
                [&]() 
                {
                    characterController.unlockFacing();
                }});
            
            Player_Down_Roll_SpriteSheet_clip.events.push_back({0,45, 
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
            
            Player_Down_Roll_SpriteSheet_clip.events.push_back({46,46, 
                [playerPtr]() 
                {
                playerPtr->getComponentOfType<GE::Hittable>().setInvincible(false); // Make the player vulnerable again after the roll
                }});

            Player_Down_Roll_SpriteSheet_clip.events.push_back({55,55, 
                [controllerPtr]() 
                {
                    controllerPtr->unlockRollMovement(); // Unlock the roll movement at the end of the roll
                }});

            Player_Right_Attack_1_SpriteSheet_clip.events.push_back({0,0, 
                [&]() 
                {
                    characterController.lockFacing();
                }});
            Player_Right_Attack_1_SpriteSheet_clip.events.push_back({8,8, 
                [playerPtr, controllerPtr]() 
                {
                // Execute attack logic when the last frame of the attack animation is reached
                playerPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Player_Right_Attack_1_SpriteSheet_clip.events.push_back({17,17, 
                [&]() 
                {
                    characterController.unlockFacing();
                }});

            Player_Right_Attack_2_SpriteSheet_clip.events.push_back({0,0, 
                [&]() 
                {
                    characterController.lockFacing();
                }});
            Player_Right_Attack_2_SpriteSheet_clip.events.push_back({16,16,
                [playerPtr, controllerPtr]() 
                {
                // Execute heavy attack logic
                playerPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Player_Right_Attack_2_SpriteSheet_clip.events.push_back({41,41, 
                [&]() 
                {
                    characterController.unlockFacing();
                }});
            
            Player_Right_Roll_SpriteSheet_clip.events.push_back({0,45, 
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

            Player_Right_Roll_SpriteSheet_clip.events.push_back({46,46,
                [playerPtr]()
                {
                playerPtr->getComponentOfType<GE::Hittable>().setInvincible(false); // Make the player vulnerable again after the roll
                }});

            Player_Right_Roll_SpriteSheet_clip.events.push_back({55,55,
                [controllerPtr]() 
                {
                    controllerPtr->unlockRollMovement(); // Unlock the roll movement at the end of the roll
                }});

            Player_Up_Attack_1_SpriteSheet_clip.events.push_back({0,0, 
                [&]() 
                {
                    characterController.lockFacing();
                }});
            Player_Up_Attack_1_SpriteSheet_clip.events.push_back({8,8, 
                [playerPtr, controllerPtr]() 
                {
                // Execute attack logic when the last frame of the attack animation is reached
                playerPtr->getComponentOfType<GE::UpAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Player_Up_Attack_1_SpriteSheet_clip.events.push_back({17,17, 
                [&]() 
                {
                    characterController.unlockFacing();
                }});

            Player_Up_Attack_2_SpriteSheet_clip.events.push_back({0,0, 
                [&]() 
                {
                    characterController.lockFacing();
                }});
            Player_Up_Attack_2_SpriteSheet_clip.events.push_back({16,16,
                [playerPtr, controllerPtr]()
                {
                // Execute heavy attack logic
                playerPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Player_Up_Attack_2_SpriteSheet_clip.events.push_back({41,41, 
                [&]() 
                {
                    characterController.unlockFacing();
                }});

            Player_Up_Roll_SpriteSheet_clip.events.push_back({0,45,
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

            Player_Up_Roll_SpriteSheet_clip.events.push_back({46,46,
                [playerPtr]()
                {
                playerPtr->getComponentOfType<GE::Hittable>().setInvincible(false); // Make the player vulnerable again after the roll
                }});

            Player_Up_Roll_SpriteSheet_clip.events.push_back({55,55,
                [controllerPtr]()
                {
                    controllerPtr->unlockRollMovement(); // Unlock the roll movement at the end of the roll
                }});

            Player_Left_Attack_1_SpriteSheet_clip.events.push_back({0,0, 
                [&]() 
                {
                    characterController.lockFacing();
                }});
            Player_Left_Attack_1_SpriteSheet_clip.events.push_back({8,8,
                [playerPtr, controllerPtr]()
                {
                // Execute attack logic when the last frame of the attack animation is reached
                playerPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Player_Left_Attack_1_SpriteSheet_clip.events.push_back({17,17, 
                [&]() 
                {
                    characterController.unlockFacing();
                }});

            Player_Left_Attack_2_SpriteSheet_clip.events.push_back({0,0, 
                [&]() 
                {
                    characterController.lockFacing();
                }});
            Player_Left_Attack_2_SpriteSheet_clip.events.push_back({16,16,
                [playerPtr, controllerPtr]()
                {
                // Execute heavy attack logic
                playerPtr->getComponentOfType<GE::HeavyAttack>().execute(controllerPtr->getFacingDirection());
                }});
            Player_Left_Attack_2_SpriteSheet_clip.events.push_back({41,41, 
                [&]() 
                {
                    characterController.unlockFacing();
                }});

            Player_Left_Roll_SpriteSheet_clip.events.push_back({0,45,
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

            Player_Left_Roll_SpriteSheet_clip.events.push_back({46,46,
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

            struct DirectionInfo
            {
                const char* name;
                int facingCode; // 1=right, -1=left, 2=up, -2=down
            };

            const DirectionInfo directions[] = {
                {"down", -2}, {"right", 1}, {"up", 2}, {"left", -1}
            };

            struct LocomotionMode
            {
                const char* name;
                std::function<bool(GE::CharacterController*)> matches;
            };

            const LocomotionMode locomotionModes[] = {
                {"idle", [](GE::CharacterController* c) { return !c->isMoving(); }},
                {"walk", [](GE::CharacterController* c) { return c->isMoving() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift); }},
                {"run",  [](GE::CharacterController* c) { return c->isMoving() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift); }}
            };

            // Ogni stato di locomozione può saltare direttamente a ogni altro stato di
            // locomozione la cui condizione (direzione + modalità) è vera in questo momento.
            for (const auto& srcDir : directions)
            {
                for (const auto& srcMode : locomotionModes)
                {
                    const std::string sourceAnim = std::string(srcMode.name) + "_" + srcDir.name;

                    for (const auto& dstDir : directions)
                    {
                        for (const auto& dstMode : locomotionModes)
                        {
                            const std::string targetAnim = std::string(dstMode.name) + "_" + dstDir.name;
                            if (targetAnim == sourceAnim) continue;

                            playerAnimator.addTransition(
                                sourceAnim, targetAnim,
                                [controllerPtr, matches = dstMode.matches, facing = dstDir.facingCode]() {
                                    return matches(controllerPtr) && controllerPtr->getFacingDirection() == facing;
                                }
                            );
                        }
                    }
                }
            }


            for (const auto& dir : directions)
            {
                const std::string idleAnim    = std::string("idle_") + dir.name;
                const std::string walkAnim    = std::string("walk_") + dir.name;
                const std::string attackAnim  = std::string("attack_") + dir.name;
                const std::string attack2Anim = std::string("attack2_") + dir.name;
                const int facing = dir.facingCode;

                // ingresso in attacco, solo da idle/walk (non da run), gated dal cooldown
                for (const std::string& fromAnim : { idleAnim, walkAnim })
                {
                    playerAnimator.addTransition(
                        fromAnim, attackAnim,
                        [playerPtr, controllerPtr, facing]() {
                            return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Left)
                                && controllerPtr->getFacingDirection() == facing
                                && !playerPtr->getComponentOfType<GE::LightAttack>().isOnCooldown();
                        }
                    );

                    playerAnimator.addTransition(
                        fromAnim, attack2Anim,
                        [playerPtr, controllerPtr, facing]() {
                            return GE::Input::isMouseButtonJustPressed(GE::MouseButton::Right)
                                && controllerPtr->getFacingDirection() == facing
                                && !playerPtr->getComponentOfType<GE::HeavyAttack>().isOnCooldown();
                        }
                    );
                }

                // uscita dall'attacco all'ultimo frame, verso idle o walk (stessa direzione)
                for (const std::string& attackName : { attackAnim, attack2Anim })
                {
                    playerAnimator.addTransition(
                        attackName, idleAnim,
                        [animatorPtr, controllerPtr, attackName]() {
                            return animatorPtr->getCurrentAnimation() == attackName &&
                                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation(attackName).frames.size()) - 1 &&
                                !controllerPtr->isMoving();
                        }
                    );
                    playerAnimator.addTransition(
                        attackName, walkAnim,
                        [animatorPtr, controllerPtr, attackName]() {
                            return animatorPtr->getCurrentAnimation() == attackName &&
                                animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation(attackName).frames.size()) - 1 &&
                                controllerPtr->isMoving();
                        }
                    );
                }
            }

            for (const auto& dir : directions)
            {
                const std::string walkAnim = std::string("walk_") + dir.name;
                const std::string runAnim  = std::string("run_") + dir.name;
                const std::string rollAnim = std::string("roll_") + dir.name;
                const int facing = dir.facingCode;

                // ingresso in roll da walk o run, stessa direzione
                for (const std::string& fromAnim : { walkAnim, runAnim })
                {
                    playerAnimator.addTransition(
                        fromAnim, rollAnim,
                        [controllerPtr, facing]() {
                            return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)
                                && controllerPtr->getFacingDirection() == facing;
                        }
                    );
                }

                // uscita dal roll all'ultimo frame: va verso QUALUNQUE stato di locomozione
                // che corrisponde all'input corrente (riusa la stessa logica del blocco di locomozione)
                for (const auto& dstDir : directions)
                {
                    for (const auto& dstMode : locomotionModes)
                    {
                        const std::string targetAnim = std::string(dstMode.name) + "_" + dstDir.name;

                        playerAnimator.addTransition(
                            rollAnim, targetAnim,
                            [animatorPtr, controllerPtr, matches = dstMode.matches, facing = dstDir.facingCode, rollAnim]() {
                                return animatorPtr->getCurrentAnimation() == rollAnim &&
                                    animatorPtr->getCurrentFrame() == static_cast<int>(animatorPtr->getAnimation(rollAnim).frames.size()) - 1 &&
                                    matches(controllerPtr) &&
                                    controllerPtr->getFacingDirection() == facing;
                            }
                        );
                    }
                }
            }

            
            ///////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////////////////////

            playerAnimator.play("idle_down"); // Start with the idle_down animation
            
            
            player->setRenderOrder(99); // Set a lower render order for this object

            return player;
        }

        if(prefabName == "Chest")
        {
            GE::GameObject* chest = new GE::GameObject(x_position, y_position, renderOrder);

            chest->addComponent<GE::SpriteRenderer>(
                "chest_closed",
                "assets/Textures/ClosedChest.png",
                chest->getTransform()
            );

            chest->addComponent<GE::ChestLogic>(
                "chest_open",
                "assets/Textures/OpenChest.png"
            );

            const auto& chestRenderer = chest->getComponentOfType<GE::SpriteRenderer>();
            auto chestTextureSize = chestRenderer.getTexture().getSize(); // Store the texture for later use

            //add collision the size of the chest sprite
            // chest->addComponent<GE::CollisionBox>(
            //     static_cast<float>(chestTextureSize.x),
            //     static_cast<float>(chestTextureSize.y)
            // );

            return chest;
        }
        return nullptr;
    }
}