#pragma once

#include <vector>
#include <string>

#include "Camera.hpp"

namespace GE
{
    class GameObject;
    class Renderer;
}

namespace GE
{
    class Scene
    {
    private:
        std::vector<GameObject*> gameObjects;
        std::string name;
        Camera camera;
    public:
        Scene(const std::string& name);
        virtual ~Scene() = default;

        void addGameObject(GameObject* gameObject);
        void removeGameObject(GameObject* gameObject);
        const std::vector<GameObject*>& getGameObjects() const;
        const std::string& getName() const;
        Camera& getCamera();
        const Camera& getCamera() const;
        void setCameraTarget(GameObject* gameObject);
        virtual void update(float deltaTime);
        virtual void render(Renderer& renderer);
    };
}