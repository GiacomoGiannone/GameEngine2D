#pragma once

#include "Attack.hpp"

namespace GE
{
    class LightAttack : public Attack
    {
    public:
        LightAttack(float damage, float cooldown, sf::Vector2f collisionBox)
            : Attack(damage, cooldown, collisionBox) {}
    };
}