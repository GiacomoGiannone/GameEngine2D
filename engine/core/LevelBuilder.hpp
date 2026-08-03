#pragma once

#include <string>

namespace GE
{
    class Level;
    class Scene;
}

namespace GE
{
    class LevelBuilder
    {
        private:
            Scene* scene; // Reference to the scene where the level will be built 
        public:
            LevelBuilder(Scene* s) : scene(s) {}
            void changeScene(Scene* s) { scene = s; } // Method to change the scene reference
            void build(Level level);
    };
}