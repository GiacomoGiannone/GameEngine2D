#include "EnemyAI.hpp"

#include <cmath>

#include "Engine.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"
#include "CharacterController.hpp"

namespace GE
{
    GameObject* EnemyAI::findPlayerTarget() const
    {
        Engine* engine = Engine::getInstance();
        if (engine == nullptr)
        {
            return nullptr;
        }

        Scene* scene = engine->getCurrentScene();
        if (scene == nullptr)
        {
            return nullptr;
        }

        for (auto* object : scene->getGameObjects())
        {
            if (!object->hasComponentOfType<CharacterController>())
            {
                continue;
            }

            auto& objectController = object->getComponentOfType<CharacterController>();
            if (objectController.isControlledByPlayer())
            {
                return object;
            }
        }

        return nullptr;
    }

    void EnemyAI::update(float deltaTime)
    {
        if (owner == nullptr)
            return;

        if (controller == nullptr)
        {
            if (!owner->hasComponentOfType<CharacterController>())
                return;

            controller = &owner->getComponentOfType<CharacterController>();
        }

        // Aggiorna il timer
        decisionTimer -= deltaTime;

        // L'AI non prende una nuova decisione ogni frame
        if (decisionTimer > 0.0f)
            return;

        // Reset del timer
        decisionTimer = decisionInterval;

        GameObject* playerTarget = findPlayerTarget();

        if (playerTarget == nullptr)
        {
            controller->clearMoveIntent();
            return;
        }

        const float dx = playerTarget->getX() - owner->getX();
        const float dy = playerTarget->getY() - owner->getY();

        const float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > aggroRange)
        {
            controller->clearMoveIntent();
            return;
        }

        if (distance <= stopDistance)
        {
            controller->clearMoveIntent();
            return;
        }

        // Movimento cardinale
        if (std::abs(dy) > std::abs(dx))
        {
            controller->setMoveIntent({
                0.0f,
                dy > 0.0f ? 1.0f : -1.0f
            });
        }
        else
        {
            controller->setMoveIntent({
                dx > 0.0f ? 1.0f : -1.0f,
                0.0f
            });
        }
    }

    void EnemyAI::render(Renderer& renderer)
    {
        (void)renderer;
    }
}
