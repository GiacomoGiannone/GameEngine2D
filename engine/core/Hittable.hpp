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
            bool isInvincible = false; // Optional: To make the object temporarily invincible after taking damage
        public:
            Hittable(float maxHealth) : health(maxHealth), maxHealth(maxHealth) {}
            float getHealth() const { return health; }
            float getMaxHealth() const { return maxHealth; }
            void setHealth(float value) { health = std::clamp(value, 0.0f, maxHealth); }
            void setMaxHealth(float value) { maxHealth = value; if (health > maxHealth) health = maxHealth; }
            virtual void takeDamage(float damage) { if (!isInvincible) setHealth(health - damage); }
            void update(float deltaTime) override;
            void render(Renderer& renderer) override { (void)renderer; }
            void setInvincible(bool value) { isInvincible = value; }
            bool getInvincible() const { return isInvincible; }
    };
}