#include "Attack.hpp"
#include "GameObject.hpp"
#include "Graphics/Renderer.hpp"
#include <iostream>
#include "Scene.hpp"
#include "Engine.hpp"
#include "Hittable.hpp"
#include "CollisionBox.hpp"

namespace GE
{
    void Attack::execute(int direction)
    {
        //spawn a collision box in front of the owner game object, with the specified range and damage
        //if debugPrint is true, draw the collision box

        //get a random number to simulate the ID to prevent multiple attacks from the same attack component from colliding with the same target
        attackID = rand() % 100000;

        //first get the owner game object
        GameObject* owner = getOwner();
        if (owner == nullptr)
        {
            std::cerr << "Error: Attack component has no owner." << std::endl;
            return;
        }

        //get the owner's transform
        Transform& ownerTransform = owner->getTransform();

        //calculate the position of the collision box based on the owner's position and rotation
        //flip the collision box based on the direction parameter (1 for right, -1 for left)
        sf::Vector2f ownerPos(ownerTransform.getX(), ownerTransform.getY());
        float rotationDeg = ownerTransform.getRotation();

        // Set the origin based on direction: left-center for right, right-center for left
        if (direction == -1)
        {
            collisionBox.setOrigin({collisionBox.getSize().x, collisionBox.getSize().y / 2.0f});
        }
        else
        {
            collisionBox.setOrigin({0.0f, collisionBox.getSize().y / 2.0f});
        }

        // Position the collision box at the owner's position
        collisionBox.setPosition(ownerPos);
        collisionBox.setRotation(sf::degrees(rotationDeg));

        isActive = true;
        activeTimer = activeDuration;

        std::cout << "Attack executed! Damage: " << damage << ", ID: " << attackID << std::endl;

        //get all objects in the scene and check for collisions with the attack collision box
        //if a collision is detected, apply damage to the target and destroy the attack collision box

        //1. Get the current scene
        Scene* currentScene = Engine::getInstance()->getCurrentScene();
        if (currentScene == nullptr)
        {
            std::cerr << "Error: No current scene found." << std::endl;
            return;
        }

        //2. Get all game objects in the scene
        const std::vector<GameObject*>& gameObjects = currentScene->getGameObjects();

        //3. Check for collisions with objects that have the component Hittable
        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == owner) continue; // Skip the owner

            if (gameObject->hasComponentOfType<Hittable>())
            {
                Hittable& hittable = gameObject->getComponentOfType<Hittable>();

                // Check if the hittable object has a CollisionBox component
                if (gameObject->hasComponentOfType<CollisionBox>())
                {
                    CollisionBox& hittableCollisionBox = gameObject->getComponentOfType<CollisionBox>();

                    // Check for collision between the two rectangles
                    // Use sf::FloatRect for the attack box (derived from sf::RectangleShape)
                    sf::FloatRect attackBounds = collisionBox.getGlobalBounds();

                    // Reconstruct a FloatRect from the CollisionBox's position and size
                    // The CollisionBox uses sf::RectangleShape internally but doesn't expose getGlobalBounds directly
                    // We need to get the GameObject's position and use the CollisionBox's width/height
                    float targetX = gameObject->getX();
                    float targetY = gameObject->getY();
                    float targetW = hittableCollisionBox.getWidth();
                    float targetH = hittableCollisionBox.getHeight();

                    // Build the target's bounding rect centered on its position
                    sf::FloatRect targetBounds(
                        sf::Vector2f(targetX - targetW / 2.0f, targetY - targetH / 2.0f),
                        sf::Vector2f(targetW, targetH)
                    );

                    // Use findIntersection for SFML 3 compatibility
                    if (attackBounds.findIntersection(targetBounds))
                    {
                        // Apply damage to the target
                        hittable.takeDamage(damage);
                        std::cout << "Hit object! Damage applied: " << damage << std::endl;
                    }
                }
            }
        }
    }

    void Attack::render(Renderer& renderer)
    {
        if (debugPrint && isActive)
        {
            //draw the collision box for debug visualization
            collisionBox.setFillColor(sf::Color(255, 0, 0, 100)); // semi-transparent red
            renderer.draw(collisionBox);
        }
    }

    void Attack::update(float deltaTime)
    {
        if (!isActive)
            return;

        activeTimer -= deltaTime;
        if (activeTimer <= 0.0f)
        {
            isActive = false;
        }
    }
}