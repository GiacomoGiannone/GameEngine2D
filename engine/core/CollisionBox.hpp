#pragma once

#include <SFML/Graphics.hpp>
#include "Component.hpp"

namespace GE
{
    //use this class for collision checking, it will represent a collision box for game objects
    class CollisionBox : public Component, public sf::Drawable
    {
    private:
        //use a sf::RectangleShape to represent the collision box
        sf::RectangleShape box;
        bool debugPrint = false; // Set to true to enable debug rendering of the collision box
    public:
        CollisionBox() = default;
        CollisionBox(float width, float height) : box(sf::Vector2f(width, height)) {}
        float getWidth() const { return box.getSize().x; }
        float getHeight() const { return box.getSize().y; }

        void setDebugPrint(bool value) { debugPrint = value; }

        void setSize(float width, float height) { box.setSize(sf::Vector2f(width, height)); }
        void setSize(const sf::Vector2f& size) { box.setSize(size); }
        const sf::Vector2f& getSize() const { return box.getSize(); }

        void setPosition(const sf::Vector2f& position) { box.setPosition(position); }
        void setFillColor(const sf::Color& color) { box.setFillColor(color); }

        void update(float deltaTime) override { (void)deltaTime; }
        void render(Renderer& renderer) override;

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            target.draw(box, states);
        }
    };
}