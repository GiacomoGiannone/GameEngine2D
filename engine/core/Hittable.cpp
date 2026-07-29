#include "Hittable.hpp"
#include "GameObject.hpp"

namespace GE
{
    void Hittable::update(float deltaTime)
    {
        (void)deltaTime; // Unused parameter

        if (health <= 0.0f)
        {
            GameObject* owner = getOwner();
            if (owner != nullptr)
            {
                owner->destroy();
            }
        }
    }
}