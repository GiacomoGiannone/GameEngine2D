#pragma once

#include "Attack.hpp"

namespace GE
{
    class UpAttack : public Attack
    {
    public:
        UpAttack(float damage, float cooldown, sf::Vector2f collisionBox)
            : Attack(damage, cooldown, collisionBox) {}
    };
}