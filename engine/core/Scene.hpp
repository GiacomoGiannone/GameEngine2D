#pragma once

#include <vector>
#include <string>

#include "Camera.hpp"

namespace GE
{
    class GameObject;
    class Renderer;
    class UI_Element;
}

namespace GE
{
    class Scene
    {
    private:
        std::vector<GameObject*> gameObjects;
        std::vector<UI_Element*> uiElements;
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
        void renderUI(Renderer& renderer);
        void addUIElement(UI_Element* uiElement) { uiElements.push_back(uiElement); }
    };
}