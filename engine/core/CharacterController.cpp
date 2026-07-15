#include "CharacterController.hpp"

#include "GameObject.hpp"
#include "CollisionBox.hpp"

#include <algorithm>
#include <limits>
#include <SFML/Window/Keyboard.hpp>
#include "graphics/Renderer.hpp"

namespace GE
{
    CharacterController::CharacterController(float speed, float width, float height)
        : speed(speed), width(width), height(height), colliderShape(ColliderShape::Rectangle), velocity(0.0f, 0.0f), gravity(900.0f), jumpVelocity(-350.0f), onGround(false), worldObjects(nullptr)
    {
        setRectangleShape(width, height);
    }

    CharacterController::CharacterController(float speed, float radius)
        : speed(speed), width(radius * 2.0f), height(radius * 2.0f), colliderShape(ColliderShape::Circle), velocity(0.0f, 0.0f), gravity(900.0f), jumpVelocity(-350.0f), onGround(false), worldObjects(nullptr)
    {
        setCircleShape(radius);
    }

    CharacterController::CharacterController(float speed, const std::vector<sf::Vector2f>& points)
        : speed(speed), width(0.0f), height(0.0f), colliderShape(ColliderShape::Convex), velocity(0.0f, 0.0f), gravity(900.0f), jumpVelocity(-350.0f), onGround(false), worldObjects(nullptr)
    {
        setConvexShape(points);
    }

    void CharacterController::setWorldObjects(const std::vector<GE::GameObject*>* gameObjects)
    {
        worldObjects = gameObjects;
    }

    void CharacterController::setGravity(float value)
    {
        gravity = value;
    }

    void CharacterController::setJumpVelocity(float value)
    {
        jumpVelocity = value;
    }

    void CharacterController::setRectangleShape(float width, float height)
    {
        collisionBox.setSize({width, height});
        this->width = width;
        this->height = height;
    }

    void CharacterController::setCircleShape(float radius)
    {
        collisionCircle.setRadius(radius);
        width = radius * 2.0f;
        height = radius * 2.0f;
    }

    void CharacterController::setConvexShape(const std::vector<sf::Vector2f>& points)
    {
        collisionConvex.setPointCount(points.size());

        float minX = std::numeric_limits<float>::max();
        float minY = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float maxY = std::numeric_limits<float>::lowest();

        for (std::size_t i = 0; i < points.size(); ++i)
        {
            const auto& point = points[i];
            collisionConvex.setPoint(i, point);

            minX = std::min(minX, point.x);
            minY = std::min(minY, point.y);
            maxX = std::max(maxX, point.x);
            maxY = std::max(maxY, point.y);
        }

        width = maxX - minX;
        height = maxY - minY;
    }

    bool CharacterController::getCollisionBox(const GE::GameObject* gameObject, const GE::CollisionBox*& outBox)
    {
        if (!gameObject->hasComponentOfType<CollisionBox>())
        {
            return false;
        }

        outBox = &gameObject->getComponentOfType<CollisionBox>();
        return true;
    }

    bool CharacterController::checkCollision(float x_position, float y_position, const std::vector<GE::GameObject*>& gameObjects) const
    {
        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == owner)
            {
                continue;
            }

            const CollisionBox* objBox = nullptr;
            if (!getCollisionBox(gameObject, objBox))
            {
                continue;
            }

            const float objX = gameObject->getX();
            const float objY = gameObject->getY();
            const float objWidth = objBox->getWidth();
            const float objHeight = objBox->getHeight();

            if (x_position < objX + objWidth &&
                x_position + width > objX &&
                y_position < objY + objHeight &&
                y_position + height > objY)
            {
                return true;
            }
        }

        return false;
    }

    void CharacterController::update(float deltaTime)
    {
        if (owner == nullptr || worldObjects == nullptr)
        {
            return;
        }

        owner->getTransform().setWidth(width);
        owner->getTransform().setHeight(height);

        float x_position = owner->getX();
        float y_position = owner->getY();

        move(deltaTime, x_position, y_position, *worldObjects);

        owner->setPosition(x_position, y_position);
    }

    void CharacterController::render(Renderer& renderer)
    {
        if(debugPrint)
        {
            //draw the shape used for collision detection
            switch (colliderShape)
            {
                case ColliderShape::Rectangle:
                    collisionBox.setPosition(sf::Vector2f(owner->getX(), owner->getY()));
                    renderer.draw(collisionBox);
                    break;
                case ColliderShape::Circle:
                    collisionCircle.setPosition(sf::Vector2f(owner->getX(), owner->getY()));
                    renderer.draw(collisionCircle);
                    break;
                case ColliderShape::Convex:
                    collisionConvex.setPosition(sf::Vector2f(owner->getX(), owner->getY()));
                    renderer.draw(collisionConvex);
                    break;
            }
        }
    }

    void CharacterController::move(float deltaTime, float& x_position, float& y_position, const std::vector<GE::GameObject*>& gameObjects)
    {
        velocity.x = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            velocity.x -= speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            velocity.x += speed;
        }

        if (onGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            velocity.y = jumpVelocity;
            onGround = false;
        }

        float newX = x_position + velocity.x * deltaTime;
        float pX = newX;
        float pY = y_position;

        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == owner)
            {
                continue;
            }

            const CollisionBox* objBox = nullptr;
            if (!getCollisionBox(gameObject, objBox))
            {
                continue;
            }

            const float objX = gameObject->getX();
            const float objY = gameObject->getY();
            const float objWidth = objBox->getWidth();
            const float objHeight = objBox->getHeight();

            if (pX < objX + objWidth &&
                pX + width > objX &&
                y_position < objY + objHeight &&
                y_position + height > objY)
            {
                if (velocity.x > 0.0f)
                {
                    newX = objX - width;
                }
                else if (velocity.x < 0.0f)
                {
                    newX = objX + objWidth;
                }

                velocity.x = 0.0f;
                pX = newX;
            }
        }

        x_position = newX;

        velocity.y += gravity * deltaTime;
        float newY = y_position + velocity.y * deltaTime;
        pY = newY;
        onGround = false;

        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == owner)
            {
                continue;
            }

            const CollisionBox* objBox = nullptr;
            if (!getCollisionBox(gameObject, objBox))
            {
                continue;
            }

            const float objX = gameObject->getX();
            const float objY = gameObject->getY();
            const float objWidth = objBox->getWidth();
            const float objHeight = objBox->getHeight();

            if (x_position < objX + objWidth &&
                x_position + width > objX &&
                pY < objY + objHeight &&
                pY + height > objY)
            {
                if (velocity.y > 0.0f)
                {
                    newY = objY - height;
                    velocity.y = 0.0f;
                    onGround = true;
                }
                else if (velocity.y < 0.0f)
                {
                    newY = objY + objHeight;
                    velocity.y = 0.0f;
                }

                pY = newY;
            }
        }

        y_position = newY;
    }

    void CharacterController::setDebugPrint(bool value)
    {
        debugPrint = value;
    }
}