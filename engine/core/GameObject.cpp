#include "GameObject.hpp"
#include "Component.hpp"

namespace GE
{
    GameObject::GameObject() {}

    void GameObject::update(float deltaTime)
    {
        for (auto& component : components)
        {
            component->update(deltaTime);
        }
    }

    void GameObject::render(Renderer& renderer)
    {
        for (auto& component : components)
        {
            component->render(renderer);
        }
    }
}


