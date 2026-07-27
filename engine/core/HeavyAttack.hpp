#pragma once

#include "Attack.hpp"

namespace GE
{
    class HeavyAttack : public Attack
    {
    public:
        HeavyAttack(float damage, float cooldown, sf::Vector2f collisionBox, int id)
            : Attack(damage, cooldown, collisionBox, id) {}
    };
}