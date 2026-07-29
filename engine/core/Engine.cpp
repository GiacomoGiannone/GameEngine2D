#include "Engine.hpp"
#include "graphics/Renderer.hpp"
#include "Scene.hpp"
#include "Input.hpp"

namespace GE
{
    Engine* Engine::instance = nullptr;

    Engine::Engine(int width, int height, const std::string& title)
    {
        renderer = new Renderer(width, height, title);
        activeScene = nullptr;
        instance = this;
    }

    Engine* Engine::getInstance()
    {
        return instance;
    }

    void Engine::setScene(Scene* scene)
    {
        activeScene = scene;
    }

    void Engine::update()
    {
        while (const auto event = renderer->getWindow()->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                renderer->getWindow()->close();
            }
        }

        const float deltaTime = 1.0f / 60.0f;

        if (activeScene != nullptr)
        {
            activeScene->update(deltaTime);
            Input::update(); // Update input state after processing events
        }
    }

    void Engine::render()
    {
        renderer->clear();

        if (activeScene != nullptr)
        {
            activeScene->render(*renderer);
        }

        renderer->display();
    }

    void Engine::run()
    {
        while (renderer->isOpen())
        {
            update();
            render();
        }
    }
}
