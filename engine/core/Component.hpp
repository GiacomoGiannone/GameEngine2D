#pragma once

namespace GE
{
    class GameObject;
}

namespace GE
{
    class Renderer;
}

namespace GE
{
    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        void setOwner(GameObject* owner) { this->owner = owner; }
        GameObject* getOwner() const { return owner; }

        virtual void update(float deltaTime) = 0;
        virtual void render(Renderer& renderer) = 0;

    protected:
        GameObject* owner{nullptr};
    };

}