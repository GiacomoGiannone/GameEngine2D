#include "Scene.hpp"
#include "GameObject.hpp"
#include "graphics/Renderer.hpp"
#include "core/UI_Element.hpp"

#include <algorithm>
#include <iostream>

namespace GE
{
    Scene::Scene(const std::string& name) : name(name) {}

    void Scene::addGameObject(GameObject* gameObject)
    {
        if (gameObject == nullptr)
        {
            std::cout << "[Scene] Warning: attempted to add null GameObject to scene '" << name << "'" << std::endl;
            return;
        }

        gameObjects.push_back(gameObject);
    }

    void Scene::removeGameObject(GameObject* gameObject)
    {
        auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
        if (it != gameObjects.end())
        {
            gameObjects.erase(it);
        }
    }

    const std::vector<GameObject*>& Scene::getGameObjects() const
    {
        return gameObjects;
    }

    const std::string& Scene::getName() const
    {
        return name;
    }

    Camera& Scene::getCamera()
    {
        return camera;
    }

    const Camera& Scene::getCamera() const
    {
        return camera;
    }

    void Scene::setCameraTarget(GameObject* gameObject)
    {
        camera.setTarget(gameObject);
    }

    void Scene::update(float deltaTime)
    {
        for (auto& gameObject : gameObjects)
        {
            gameObject->update(deltaTime);
        }

        for (auto& uiElement : uiElements)
        {
            uiElement->Update(deltaTime);   // <-- aggiungi
        }

        camera.update(deltaTime);
    }

    void Scene::render(Renderer& renderer)
    {
        camera.applyTo(*renderer.getWindow());
        //first sort the game objects by render order, then render them
        std::sort(gameObjects.begin(), gameObjects.end(), [](const GameObject* a, const GameObject* b) {
            return a->getRenderOrder() < b->getRenderOrder();
        });
        
        for (auto& gameObject : gameObjects)
        {
            gameObject->render(renderer);
        }

        renderUI(renderer);
    }

    void Scene::renderUI(Renderer& renderer)
    {
        // Render UI elements using the UI view
        renderer.setView(camera.getUiView());
        for (auto& uiElement : uiElements)
        {
            uiElement->Render(renderer);
        }
    }
}