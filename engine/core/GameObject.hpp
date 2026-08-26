#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <typeinfo>
#include <stdexcept>
#include <iostream>

#include "Component.hpp"


namespace GE
{
    class Renderer;
}

#include "Transform.hpp"

namespace GE
{
    class GameObject
    {
    private:
        std::vector<std::unique_ptr<Component>> components;
        Transform transform;
        int renderOrder;
        //not all game object are clickable
        bool isClickable;

        //set to true by destroy(); the object is removed from the world at a safe time
        bool destroyed = false;
        std::string layerName; // Name of the layer this GameObject belongs to
    public:
        GameObject();
        GameObject(float x_position, float y_position, int renderOrder) : transform(), renderOrder(renderOrder), isClickable(false) { transform.setPosition(x_position, y_position); }
        virtual ~GameObject() = default;

        void update(float deltaTime);
        void render(Renderer& renderer);

        void setPosition(float x, float y) { transform.setPosition(x, y); }
        float getX() const { return transform.getX(); }
        float getY() const { return transform.getY(); }
        Transform& getTransform() { return transform; }
        const Transform& getTransform() const { return transform; }
        int getRenderOrder() const { return renderOrder; }
        void setRenderOrder(int order) { renderOrder = order; }
        bool isDestroyed() const { return destroyed; }
        bool destroy();
        void setLayerName(const std::string& name) { layerName = name; }
        const std::string& getLayerName() const { return layerName; }

        template<typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            auto component = std::make_unique<T>(
                std::forward<Args>(args)...
            );

            T& ref = *component;
            ref.setOwner(this);

            for (const auto& existingComponent : components)
            {
                if (typeid(*existingComponent) == typeid(T))
                {
                    std::cerr << "Error: Component of type " << typeid(T).name() << " already exists in GameObject." << std::endl;
                    return ref;
                }
            }

            components.push_back(std::move(component));
            return ref;
        }

        template<typename T>
        T& getComponentOfType() const
        {
            for (const auto& component : components)
            {
                if (typeid(*component) == typeid(T))
                {
                    return *static_cast<T*>(component.get());
                }
            }
            throw std::runtime_error("Component not found");
        }

        template<typename T>
        bool hasComponentOfType() const
        {
            for (const auto& component : components)
            {
                if (typeid(*component) == typeid(T))
                {
                    return true;
                }
            }
            return false;
        }
    };
}


