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
            skeletonEnemy->addComponent<GE::CollisionBox>(static_cast<float>(skeletonTextureSize.x), static_cast<float>(skeletonTextureSize.y));
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
            skeletonEnemy->addComponent<GE::LightAttack>(7.0f, 1.5f, lightAttackCollisionBox); //damage, cooldown, collision box
            sf::Vector2f heavyAttackCollisionBox(60.0f, 90.0f); //width, height
            skeletonEnemy->addComponent<GE::HeavyAttack>(15.0f, 2.5f, heavyAttackCollisionBox); //damage, cooldown, collision box
            //make the skeleton damageable by the player's attacks
            skeletonEnemy->addComponent<GE::Hittable>(50.0f); //health

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
            Down_Attack1_SpriteSheet_clip.events.push_back({8,8,
                [enemyPtr, controllerPtr]()
                {
                // Execute light attack logic when the attack frame is reached
                enemyPtr->getComponentOfType<GE::LightAttack>().execute(controllerPtr->getFacingDirection());
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
                    [controllerPtr, isTargetWithinRange, heavyAttackRange, facing = direction.second]() {
                        return isTargetWithinRange(heavyAttackRange) && controllerPtr->getFacingDirection() == facing;
                    }
                );
                animator.addTransition(
                    idleName, attack2Name,
                    [controllerPtr, isTargetWithinRange, heavyAttackRange, facing = direction.second]() {
                        return isTargetWithinRange(heavyAttackRange) && controllerPtr->getFacingDirection() == facing;
                    }
                );

                //walk/idle to attack1 (light, longer range than the heavy attack)
                animator.addTransition(
                    walkName, attack1Name,
                    [controllerPtr, isTargetWithinRange, lightAttackRange, facing = direction.second]() {
                        return isTargetWithinRange(lightAttackRange) && controllerPtr->getFacingDirection() == facing;
                    }
                );
                animator.addTransition(
                    idleName, attack1Name,
                    [controllerPtr, isTargetWithinRange, lightAttackRange, facing = direction.second]() {
                        return isTargetWithinRange(lightAttackRange) && controllerPtr->getFacingDirection() == facing;
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

        return nullptr;
    }
}