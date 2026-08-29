#pragma once
#include "graphics/Renderer.hpp"
#include "Core/UI_Element.hpp"
#include "SFML/Graphics.hpp"


namespace GE
{
    class Hittable; // Forward declaration of Hittable class
}

namespace GE
{
    class Healthbar : public UI_Element
    {
        private:
            sf::RectangleShape background;
            sf::RectangleShape fill;
            const Hittable* target{nullptr}; // Pointer to the target Hittable component
        public:
            Healthbar(float width, float height);
            void Update(float deltaTime) override;
            void Render(GE::Renderer& renderer) override;
            void setHealth(float healthPercentage);
            void setTarget(const Hittable* hittable); // Set the target Hittable component
    };
}