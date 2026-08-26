#pragma once

#include "core/Component.hpp"

namespace GE
{
    class CharacterController;
    class GameObject;
}

namespace GE
{
    class EnemyAI : public Component
    {
    private:
        CharacterController* controller = nullptr;
        float aggroRange = 300.0f;
        float stopDistance = 40.0f;

        float decisionTimer = 0.0f;
        float decisionInterval = 0.5f; // Time interval between decisions

        GameObject* findPlayerTarget() const;

    public:
        EnemyAI(float aggroRange = 300.0f, float stopDistance = 40.0f)
            : aggroRange(aggroRange), stopDistance(stopDistance)
        {
        }

        void update(float deltaTime) override;
        void render(Renderer& renderer) override;
    };
}
