#include "CharacterController.hpp"

#include "GameObject.hpp"
#include "CollisionBox.hpp"

#include <algorithm>
#include <limits>
#include <SFML/Window/Keyboard.hpp>
#include "graphics/Renderer.hpp"
#include "graphics/Animator.hpp"

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
        const float halfWidth = width * 0.5f;
        const float halfHeight = height * 0.5f;

        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == owner)
            {
                continue;
            }

            //skip objects on ignored layers
            bool isIgnored = false;
            for(const auto& ignoredLayer : IgnoredLayers)
            {
                if(gameObject->getLayerName() == ignoredLayer)
                {
                    isIgnored = true;
                    break;
                }
            }
            if (isIgnored)
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
            const float objHalfWidth = objWidth * 0.5f;
            const float objHalfHeight = objHeight * 0.5f;

            if (x_position - halfWidth < objX + objHalfWidth &&
                x_position + halfWidth > objX - objHalfWidth &&
                y_position - halfHeight < objY + objHalfHeight &&
                y_position + halfHeight > objY - objHalfHeight)
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
                    collisionBox.setPosition(sf::Vector2f(
                        owner->getX() - (collisionBox.getSize().x * 0.5f),
                        owner->getY() - (collisionBox.getSize().y * 0.5f)
                    ));
                    //draw with alpha to make it semi-transparent
                    collisionBox.setFillColor(sf::Color(255, 0, 0, 100));
                    renderer.draw(collisionBox);
                    break;
                case ColliderShape::Circle:
                    collisionCircle.setPosition(sf::Vector2f(
                        owner->getX() - collisionCircle.getRadius(),
                        owner->getY() - collisionCircle.getRadius()
                    ));
                    collisionCircle.setFillColor(sf::Color(255, 0, 0, 100));
                    renderer.draw(collisionCircle);
                    break;
                case ColliderShape::Convex:
                    collisionConvex.setPosition(sf::Vector2f(
                        owner->getX() - (width * 0.5f),
                        owner->getY() - (height * 0.5f)
                    ));
                    collisionConvex.setFillColor(sf::Color(255, 0, 0, 100));
                    renderer.draw(collisionConvex);
                    break;
            }
        }
    }

    void CharacterController::move(float deltaTime, float& x_position, float& y_position, const std::vector<GE::GameObject*>& gameObjects)
    {
        velocity.x = 0.0f;
        if (Y_movement_enabled)
        {
            velocity.y = 0.0f;
        }

        const float movementMultiplier = animator ? animator->getMovementMultiplier() : 1.0f;

        if (isRollLocked)
        {
            velocity = rollVelocity;
        }
        else if (ControlledByPlayer)
        {
            const bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
            const bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
            const bool upPressed = Y_movement_enabled && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
            const bool downPressed = Y_movement_enabled && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);

            // Enforce strict 4-direction movement: when both axes are pressed, vertical has priority,
            // otherwise the horizontal axis wins. This avoids diagonal motion and keeps facingDirection
            // aligned with the actual movement axis.
            if (upPressed || downPressed)
            {
                if (upPressed && !downPressed)
                {
                    velocity.y -= speed * movementMultiplier;
                    facingDirection = 2;
                }
                else if (downPressed && !upPressed)
                {
                    velocity.y += speed * movementMultiplier;
                    facingDirection = -2;
                }
            }
            else if (leftPressed || rightPressed)
            {
                if (leftPressed && !rightPressed)
                {
                    velocity.x -= speed * movementMultiplier;
                    facingDirection = -1;
                }
                else if (rightPressed && !leftPressed)
                {
                    velocity.x += speed * movementMultiplier;
                    facingDirection = 1;
                }
            }
        }
        else
        {
            const bool hasHorizontal = std::abs(moveIntent.x) > 0.1f;
            const bool hasVertical = Y_movement_enabled &&
                                    std::abs(moveIntent.y) > 0.1f;

            // Movimento X
            if (hasHorizontal)
            {
                velocity.x = moveIntent.x * speed * movementMultiplier;
            }

            // Movimento Y
            if (hasVertical)
            {
                velocity.y = moveIntent.y * speed * movementMultiplier;
            }

            // La direzione dell'animazione viene scelta separatamente
            if (hasHorizontal || hasVertical)
            {
                if (std::abs(moveIntent.x) > std::abs(moveIntent.y))
                {
                    // Movimento prevalentemente orizzontale
                    if (moveIntent.x < 0.0f)
                        facingDirection = -1; // Left
                    else
                        facingDirection = 1;  // Right
                }
                else
                {
                    // Movimento prevalentemente verticale
                    if (moveIntent.y < 0.0f)
                        facingDirection = 2;  // Up
                    else
                        facingDirection = -2; // Down
                }
            }
        }

        if (ControlledByPlayer && !Y_movement_enabled && onGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            velocity.y = jumpVelocity;
            onGround = false;
        }

        const float halfWidth = width * 0.5f;
        const float halfHeight = height * 0.5f;

        float newX = x_position + velocity.x * deltaTime;
        float newY = y_position;

        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == owner)
            {
                continue;
            }

            //skip objects on ignored layers
            bool isIgnored = false;
            for (const auto& ignoredLayer : IgnoredLayers)
            {
                if (gameObject->getLayerName() == ignoredLayer)
                {
                    isIgnored = true;
                    break;
                }
            }
            if (isIgnored)
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
            const float objHalfWidth = objWidth * 0.5f;
            const float objHalfHeight = objHeight * 0.5f;

            if (newX - halfWidth < objX + objHalfWidth &&
                newX + halfWidth > objX - objHalfWidth &&
                y_position - halfHeight < objY + objHalfHeight &&
                y_position + halfHeight > objY - objHalfHeight)
            {
                if (velocity.x > 0.0f)
                {
                    newX = objX - objHalfWidth - halfWidth;
                }
                else if (velocity.x < 0.0f)
                {
                    newX = objX + objHalfWidth + halfWidth;
                }

                velocity.x = 0.0f;
            }
        }

        x_position = newX;

        if (!Y_movement_enabled)
        {
            velocity.y += gravity * deltaTime;
        }

        newY = y_position + velocity.y * deltaTime;
        onGround = false;

        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == owner)
            {
                continue;
            }

            //skip objects on ignored layers
            bool isIgnored = false;
            for (const auto& ignoredLayer : IgnoredLayers)
            {
                if (gameObject->getLayerName() == ignoredLayer)
                {
                    isIgnored = true;
                    break;
                }
            }
            if (isIgnored)
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
            const float objHalfWidth = objWidth * 0.5f;
            const float objHalfHeight = objHeight * 0.5f;

            if (x_position - halfWidth < objX + objHalfWidth &&
                x_position + halfWidth > objX - objHalfWidth &&
                newY - halfHeight < objY + objHalfHeight &&
                newY + halfHeight > objY - objHalfHeight)
            {
                if (velocity.y > 0.0f)
                {
                    newY = objY - objHalfHeight - halfHeight;
                    velocity.y = 0.0f;
                    onGround = true;
                }
                else if (velocity.y < 0.0f)
                {
                    newY = objY + objHalfHeight + halfHeight;
                    velocity.y = 0.0f;
                }
            }
        }

        y_position = newY;
    }

    void CharacterController::setDebugPrint(bool value)
    {
        debugPrint = value;
    }

    void CharacterController::lockRollMovement()
    {
            isRollLocked = true;

            // Cattura la direzione di movimento corrente (WASD attivi in quel momento)
            sf::Vector2f dir(0.0f, 0.0f);

            if (ControlledByPlayer)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dir.x -= 1.0f;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dir.x += 1.0f;
                if (Y_movement_enabled && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dir.y -= 1.0f;
                if (Y_movement_enabled && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dir.y += 1.0f;
            }

            // Se non si sta premendo nulla, usa la direzione verso cui il personaggio è rivolto
            if (dir.x == 0.0f && dir.y == 0.0f)
            {
                dir.x = static_cast<float>(facingDirection);
            }

            // Normalizza per non avere impulso più forte in diagonale
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (length > 0.0f)
            {
                dir.x /= length;
                dir.y /= length;
            }

            rollVelocity = dir * rollSpeed;
    }

    void CharacterController::unlockRollMovement()
    {
        isRollLocked = false;
        rollVelocity = {0.0f, 0.0f};
    }

    
}
