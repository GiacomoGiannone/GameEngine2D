#pragma once

#include <SFML/Graphics.hpp>
#include "core/Component.hpp"
#include "core/Transform.hpp"

namespace GE
{
    class Renderer;
}

namespace GE
{
    class RectangleRenderer : public Component
    {
    private:
        sf::RectangleShape rectangle;
        Transform& transform;
    public:
        RectangleRenderer(float width, float height, const sf::Color& color, Transform& transform);
        void update(float deltaTime) override;
        void render(Renderer& renderer) override;
    };
}