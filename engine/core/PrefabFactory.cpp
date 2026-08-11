#include "PrefabFactory.hpp"

#include "GameObject.hpp"
#include "Graphics/SpriteRenderer.hpp"
#include "Core/CollisionBox.hpp"

namespace GE
{
    GameObject* PrefabFactory::create(const std::string& prefabName, float x_position, float y_position, int renderOrder)
    {
        if(prefabName == "Wall_1")
        {
            GameObject* wall = new GameObject();
            wall->getTransform().setPosition(x_position, y_position);
            wall->setRenderOrder(renderOrder);
            //add a texture renderer component to the wall
            auto& wallRenderer = wall->addComponent<GE::SpriteRenderer>("Textures/Wall_2.png", "assets/Textures/Wall_2.png", wall->getTransform());
            //add a collision box to the wall using the texture dimensions
            auto wallTextureSize = wallRenderer.getTexture().getSize();
            wall->addComponent<GE::CollisionBox>(static_cast<float>(wallTextureSize.x), static_cast<float>(wallTextureSize.y));
            return wall;
        }

        if(prefabName == "Floor_1")
        {
            GameObject* floor = new GameObject();
            floor->getTransform().setPosition(x_position, y_position);
            floor->setRenderOrder(renderOrder);
            //add a texture renderer component to the floor
            floor->addComponent<GE::SpriteRenderer>("Textures/Floor_1.png", "assets/Textures/Floor_1.png", floor->getTransform());
            return floor;
        }

        return nullptr;
    }
}