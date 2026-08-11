#pragma once

#include <string>

namespace GE
{
    class Renderer;
    class Scene;
}

namespace GE
{
    class Engine
    {
    private:
        Renderer* renderer;
        Scene* activeScene;
        static Engine* instance; // Singleton instance
        void update();
        void render();
    public:
        Engine(int width, int height, const std::string& title);
        virtual ~Engine() = default;

        static Engine* getInstance();

        void setScene(Scene* scene);
        void run();
        Scene* getCurrentScene() const { return activeScene; }
        Renderer* getRenderer() const { return renderer; }
    };
}