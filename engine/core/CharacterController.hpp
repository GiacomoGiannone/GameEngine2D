#pragma once

//character controller class to handle movement and collision detection

#include <SFML/Graphics.hpp>
#include <vector>

#include "core/Component.hpp"

namespace GE
{
    class GameObject;
    class CollisionBox;
}

namespace GE
{
    class CharacterController : public Component
    {
    public:
        enum class ColliderShape
        {
            Rectangle,
            Circle,
            Convex
        };

    private:
        float speed;
        float width, height;
        ColliderShape colliderShape;
        sf::Vector2f velocity;
        float gravity;
        float jumpVelocity;
        bool onGround;
        const std::vector<GE::GameObject*>* worldObjects;
        bool debugPrint = false;
        //if Y_movement_enabled is true, you can move the character up and down with the W and S keys, 
        //otherwise the character can only move left and right with the A and D keys
        bool Y_movement_enabled = false;

        static bool getCollisionBox(const GE::GameObject* gameObject, const GE::CollisionBox*& outBox);

        //collision detection helper functions
        bool checkCollision(float x_position, float y_position, const std::vector<GE::GameObject*>& gameObjects) const;

        void setRectangleShape(float width, float height);
        void setCircleShape(float radius);
        void setConvexShape(const std::vector<sf::Vector2f>& points);

        //we give the character controller a predefined rectangle shape to check for collisions with other game objects
        sf::RectangleShape collisionBox;
        //alternatively, we can declare other shapes like sf::CircleShape or sf::ConvexShape for more complex collision detection
        sf::CircleShape collisionCircle;
        sf::ConvexShape collisionConvex;
    public:
        //init with rectangle shape dimensions and speed
        CharacterController(float speed, float width, float height);
        //init with circle shape radius and speed
        CharacterController(float speed, float radius);
        //init with convex shape points and speed
        CharacterController(float speed, const std::vector<sf::Vector2f>& points);

        void setBoxCollider(float width, float height) { setRectangleShape(width, height); }
        void setCircleCollider(float radius) { setCircleShape(radius); }
        void setConvexCollider(const std::vector<sf::Vector2f>& points) { setConvexShape(points); }
        void setYMovementEnabled(bool enabled) { Y_movement_enabled = enabled; }
        bool isYMovementEnabled() const { return Y_movement_enabled; }

        virtual ~CharacterController() = default;

        void setWorldObjects(const std::vector<GE::GameObject*>* gameObjects);
        void setGravity(float value);
        void setJumpVelocity(float value);
        void setDebugPrint(bool value);

        void update(float deltaTime) override;
        void render(Renderer& renderer) override;

        void move(float deltaTime, float& x_position, float& y_position, const std::vector<GE::GameObject*>& gameObjects);
    };
}