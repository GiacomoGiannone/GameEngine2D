#pragma once

//character controller class to handle movement and collision detection

#include <SFML/Graphics.hpp>
#include <vector>

#include "core/Component.hpp"

namespace GE
{
    class GameObject;
    class CollisionBox;
    class Animator;
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
        float originalHeight; // Store the original height for resetting after roll
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
        Animator* animator = nullptr;
        int facingDirection = 1; // 1 for right, -1 for left
        bool isRollLocked = false; // Flag to lock rolling for a short duration
        sf::Vector2f rollVelocity{0.0f, 0.0f}; // Store the velocity during roll
        float rollSpeed = 300.0f; // Speed during roll

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
        bool isMoving() const { return std::abs(velocity.x) > 0.1f || std::abs(velocity.y) > 0.1f; }
        void setAnimator(Animator* animator) { this->animator = animator; }
        int getFacingDirection() const { return facingDirection; } // 1 for right, -1 for left, 2 for up, -2 for down
        void roll(); //this function reduces the hitbox of the character controller to half of its height 
        void resetHitbox() { setRectangleShape(width, originalHeight); } //this function resets the hitbox of the character controller to its original height

        void lockRollMovement();
        void unlockRollMovement();
        bool getIsRollLocked() const { return isRollLocked; }
        void setRollSpeed(float speed) { rollSpeed = speed; }
        
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