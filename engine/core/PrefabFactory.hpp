#pragma once
#include <string>

namespace GE
{
    class GameObject;
}

namespace GE
{
    class PrefabFactory
    {
        public:
            static GameObject* create(const std::string& prefabName, float x_position, float y_position, int renderOrder);
    };
}