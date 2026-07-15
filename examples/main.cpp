#include "core/Engine.hpp"
#include "../scenes/mainScene.hpp"

int main()
{
    GE::Engine engine(800, 600, "Game Engine");
    MainScene mainScene;
    engine.setScene(&mainScene);

    engine.run();

    return 0;
}
