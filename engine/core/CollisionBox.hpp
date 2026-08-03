#pragma once

#include <SFML/Graphics.hpp>
#include "Component.hpp"

namespace GE
{
    //use this class for collision checking, it will represent a collision box for game objects
    class CollisionBox : public Component
    {
    private:
        //use a sf::RectangleShape to represent the collision box
        sf::RectangleShape box;
    public:
        CollisionBox(float width, float height) : box(sf::Vector2f(width, height)) {}
        float getWidth() const { return box.getSize().x; }
        float getHeight() const { return box.getSize().y; }

        void setSize(float width, float height) { box.setSize(sf::Vector2f(width, height)); }

        void update(float deltaTime) override { (void)deltaTime; }
        void render(Renderer& renderer) override;
    };
}