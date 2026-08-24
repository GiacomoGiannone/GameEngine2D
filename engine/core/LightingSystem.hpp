#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "Light.hpp"
#include "GameObject.hpp"
#include "graphics/SpriteRenderer.hpp"

namespace GE
{
    class LightingSystem
    {
    private:
        std::vector<Light*> lights;
        float ambientLight = 0.2f;

    public:
        void addLight(Light& light)
        {
            lights.push_back(&light);
        }

        void removeLight(Light* light)
        {
            auto it = std::find(lights.begin(), lights.end(), light);
            if (it != lights.end())
            {
                lights.erase(it);
            }
        }

        void setAmbientLight(float value)
        {
            ambientLight = std::clamp(value, 0.0f, 1.0f);
        }

        void update(const std::vector<GameObject*>& objects)
        {
            for (const auto* object : objects)
            {
                if (!object->hasComponentOfType<SpriteRenderer>())
                {
                    continue;
                }

                auto& spriteRenderer = object->getComponentOfType<SpriteRenderer>();
                const sf::Vector2f spriteCenter(
                    object->getX(),
                    object->getY()
                );

                float brightness = ambientLight;

                for (const auto* light : lights)
                {
                    const sf::Vector2f delta = spriteCenter - light->getPosition();
                    const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

                    float factor = 1.0f - (distance / std::max(light->getRadius(), 1.0f));
                    factor = std::clamp(factor, 0.0f, 1.0f);
                    factor *= light->getIntensity();

                    brightness = std::max(brightness, factor);
                }

                brightness = std::clamp(brightness, 0.0f, 1.0f);

                const unsigned char value = static_cast<unsigned char>(255.0f * brightness);
                spriteRenderer.setColor(sf::Color(value, value, value));
            }
        }
    };
}
