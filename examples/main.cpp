#include "core/Engine.hpp"
#include "../scenes/mainScene.hpp"

int main()
{
    GE::Engine engine(1920, 1080, "Game Engine");
    MainScene mainScene;
    engine.setScene(&mainScene);

    engine.run();

    return 0;
}
