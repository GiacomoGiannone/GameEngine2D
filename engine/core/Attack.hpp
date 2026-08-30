#pragma once

#include "Component.hpp"
#include "SFML/Graphics.hpp"

namespace GE
{
    class Attack: public Component
    {
        protected:
            float damage;
            float range;
            float cooldown;
            float cooldownTimer{0.0f};
            //collision box for the attack, will be spawned in front of the owner game object
            sf::RectangleShape collisionBox;
            bool debugPrint{false};
            bool isActive{false};
            float activeDuration{0.50f};
            float activeTimer{0.0f};
            //each attack should have an ID so that when we check for collisions we can identify which attack is which
            int attackID;
        public:
            Attack(float damage, float cooldown, sf::Vector2f collisionBox) : damage(damage), cooldown(cooldown), collisionBox(collisionBox) {}
            float getDamage() const { return damage; }
            float getRange() const { return range; }
            float getCooldown() const { return cooldown; }
            int getAttackID() const { return attackID; }
            bool isOnCooldown() const { return cooldownTimer > 0.0f; }

            void update(float deltaTime) override;
            void render(Renderer& renderer) override;
            void setDebugPrint(bool value) { debugPrint = value; }
            void execute(int direction = 1); //direction: 1 for right, -1 for left
        };
}