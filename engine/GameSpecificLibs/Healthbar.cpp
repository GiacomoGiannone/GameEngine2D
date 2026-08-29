#include "Healthbar.hpp"

#include "graphics/Renderer.hpp"
#include "Core/Hittable.hpp"

namespace GE
{
    Healthbar::Healthbar(float width, float height)
    {
        background.setPosition({20.0f, 20.0f}); // Position of the health bar
        background.setSize(sf::Vector2f(width, height));
        background.setFillColor(sf::Color(100, 100, 100)); // Dark gray background
        fill.setPosition({20.0f, 20.0f});   // <-- aggiungi
        fill.setSize(sf::Vector2f(width, height));
        fill.setFillColor(sf::Color(0, 255, 0)); // Green fill
    }

    void Healthbar::Update(float deltaTime)
    {
        if (target != nullptr)
        {
            setHealth(target->getHealth() / target->getMaxHealth());
        }
    }

    void Healthbar::Render(GE::Renderer& renderer)
    {
        renderer.draw(background);
        renderer.draw(fill);
    }

    void Healthbar::setHealth(float healthPercentage)
    {
        if (healthPercentage < 0.0f) healthPercentage = 0.0f;
        if (healthPercentage > 1.0f) healthPercentage = 1.0f;
        fill.setSize(sf::Vector2f(background.getSize().x * healthPercentage, background.getSize().y));
    }

    void Healthbar::setTarget(const Hittable* hittable)
    {
        target = hittable;
    }
}