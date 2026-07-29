#include "Attack.hpp"
#include "GameObject.hpp"
#include "Graphics/Renderer.hpp"
#include <iostream>

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