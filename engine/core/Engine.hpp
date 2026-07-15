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
        void update();
        void render();
    public:
        Engine(int width, int height, const std::string& title);
        virtual ~Engine() = default;

        void setScene(Scene* scene);
        void run();
    };
}
