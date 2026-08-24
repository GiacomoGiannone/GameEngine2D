#pragma once

#include <SFML/System/Vector2.hpp>

namespace GE
{
    class Light
    {
    private:
        sf::Vector2f position;
        float radius;
        float intensity;

    public:
        Light(sf::Vector2f position, float radius, float intensity = 1.0f)
            : position(position), radius(radius), intensity(intensity)
        {
        }

        const sf::Vector2f& getPosition() const
        {
            return position;
        }

        float getRadius() const
        {
            return radius;
        }

        float getIntensity() const
        {
            return intensity;
        }

        void setPosition(const sf::Vector2f& newPosition)
        {
            position = newPosition;
        }

        void setRadius(float newRadius)
        {
            radius = newRadius;
        }

        void setIntensity(float newIntensity)
        {
            intensity = newIntensity;
        }
    };
}
