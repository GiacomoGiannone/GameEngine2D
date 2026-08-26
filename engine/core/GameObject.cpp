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

    bool GameObject::destroy()
    {
        //Mark the object as destroyed. The actual cleanup (removal from the scene and
        //deletion) is done later, outside the component/update loop. Clearing components
        //here would destroy components while they are still being updated (use-after-free).
        destroyed = true;
        return true;
    }
}


