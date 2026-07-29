#pragma once

#include "Component.hpp"
#include <algorithm>

namespace GE
{
    class Hittable : public Component
    {
        protected:
            float health;
            float maxHealth;
        public:
            Hittable(float maxHealth) : health(maxHealth), maxHealth(maxHealth) {}
            float getHealth() const { return health; }
            float getMaxHealth() const { return maxHealth; }
            void setHealth(float value) { health = std::clamp(value, 0.0f, maxHealth); }
            void setMaxHealth(float value) { maxHealth = value; if (health > maxHealth) health = maxHealth; }
            virtual void takeDamage(float damage) { setHealth(health - damage); }
            //in the update function check if health is less than or equal to 0, if so, destroy the game object
            //also check if you were hit by an attack, if so, take damage and destroy the attack game object
            void update(float deltaTime) override;
    };
}