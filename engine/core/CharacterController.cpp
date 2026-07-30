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
        : speed(speed), width(width), height(height), colliderShape(ColliderShape::Rectangle), velocity(0.0f, 0.0f), gravity(900.0f), 
        jumpVelocity(-350.0f), onGround(false), worldObjects(nullptr), originalHeight(height)
    {
        setRectangleShape(width, height);
    }

    CharacterController::CharacterController(float speed, float radius)
        : speed(speed), width(radius * 2.0f), height(radius * 2.0f), colliderShape(ColliderShape::Circle), velocity(0.0f, 0.0f), gravity(900.0f), jumpVelocity(-350.0f), onGround(false), worldObjects(nullptr), originalHeight(radius * 2.0f)
    {
        setCircleShape(radius);
    }

    CharacterController::CharacterController(float speed, const std::vector<sf::Vector2f>& points)
        : speed(speed), width(0.0f), height(0.0f), colliderShape(ColliderShape::Convex), velocity(0.0f, 0.0f), gravity(900.0f), jumpVelocity(-350.0f), onGround(false), worldObjects(nullptr), originalHeight(0.0f)
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
            const float objWidth = objBox->getWidth()*0.5f; // Adjusted for half-width
            const float objHeight = objBox->getHeight()*0.5f; // Adjusted for half-height

            float left = x_position - objWidth;
            float right = x_position + objWidth;
            float top = y_position - objHeight;
            float bottom = y_position + objHeight;

            if (left < objX + objWidth && right > objX &&
                top < objY + objHeight && bottom > objY)
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
                    // 1. Imposta la posizione della hitbox, ma SOTTRAI metà larghezza e altezza 
                    // per centrarla sulle coordinate (X, Y) del personaggio.
                    collisionBox.setPosition(sf::Vector2f(
                        owner->getX() - (collisionBox.getSize().x * 0.5f), 
                        owner->getY() - (collisionBox.getSize().y * 0.5f)
                    ));
                    //draw with alpha to make it semi-transparent
                    collisionBox.setFillColor(sf::Color(255, 0, 0, 100));
                    renderer.draw(collisionBox);
                    break;
                case ColliderShape::Circle:
                    // 2. Per il cerchio, devi sottrarre il raggio (che è width/2 oppure height/2)
                    collisionCircle.setPosition(sf::Vector2f(
                        owner->getX() - (collisionCircle.getRadius()),
                        owner->getY() - (collisionCircle.getRadius())
                    ));
                    collisionCircle.setFillColor(sf::Color(255, 0, 0, 100));
                    renderer.draw(collisionCircle);
                    break;
                case ColliderShape::Convex:
                    // 3. Per il poligono convesso, sottrai metà larghezza e altezza calcolate
                    // (sf::ConvexShape non ha un metodo setOrigin(), quindi devi spostare la posizione)
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
        // Resetta la velocità
        velocity.x = 0.0f;
        if (Y_movement_enabled) velocity.y = 0.0f;
        //float runningMultiplier = isRunning ? 1.5f : 1.0f; // Adjust speed if running

        //get movement multiplier from animator if available
        float movementMultiplier = 1.0f;
        if(animator)
        {
            movementMultiplier = animator->getMovementMultiplier();
        }

        //holding SHIFT key makes the character run
        // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
        // {
        //     isRunning = true;
        // }
        // else
        // {
        //     isRunning = false;
        // }
        
        if(isRollLocked)
        {
            velocity = rollVelocity; // Maintain the roll velocity
        }
        else
        {
            // --- GESTIONE INPUT ---
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                velocity.x -= speed * movementMultiplier  ;
                if (animator) animator->setFlipped(true);
                facingDirection = -1; // Facing left
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                velocity.x += speed * movementMultiplier  ;
                if (animator) animator->setFlipped(false);
                facingDirection = 1; // Facing right
            }

            if (Y_movement_enabled && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) velocity.y -= speed * movementMultiplier ;
            if (Y_movement_enabled && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) velocity.y += speed * movementMultiplier  ;

            if (!Y_movement_enabled && onGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                velocity.y = jumpVelocity;
                onGround = false;
            }
        }

        // --- CALCOLO DEI BORDI (Partendo dal presupposto che X e Y siano CENTRO) ---
        // Se il motore passa X e Y come angolo, questa matematica li tratta comunque come centro.
        // Il rettangolo piccolo nell'immagine si allineerà perché stiamo calcolando tutto da qui.
        const float halfW = width * 0.5f;
        const float halfH = height * 0.5f;

        float newX = x_position + velocity.x * deltaTime;
        float newY = y_position; // Y verrà processata dopo

        // --- COLLISIONE SU X ---
        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == owner) continue;

            const CollisionBox* objBox = nullptr;
            if (!getCollisionBox(gameObject, objBox)) continue;

            const float objX = gameObject->getX();
            const float objY = gameObject->getY();
            const float objW = objBox->getWidth();
            const float objH = objBox->getHeight();

            // Calcoliamo i bordi ASSOLUTI del personaggio per questo frame (X)
            float myLeft   = newX - halfW;
            float myRight  = newX + halfW;
            float myTop    = y_position - halfH;
            float myBottom = y_position + halfH;

            // Calcoliamo i bordi ASSOLUTI dell'oggetto
            const float objHalfW = objW * 0.5f;
            const float objHalfH = objH * 0.5f;
            float otherLeft   = objX - objHalfW;
            float otherRight  = objX + objHalfW;
            float otherTop    = objY - objHalfH;
            float otherBottom = objY + objHalfH;

            // Controllo intersezione
            if (myLeft < otherRight && myRight > otherLeft &&
                myTop < otherBottom && myBottom > otherTop)
            {
                // Se andiamo a destra, spingici a sinistra dell'oggetto
                if (velocity.x > 0.0f)
                {
                    newX = otherLeft - halfW; 
                }
                // Se andiamo a sinistra, spingici a destra dell'oggetto
                else if (velocity.x < 0.0f)
                {
                    newX = otherRight + halfW;
                }
                velocity.x = 0.0f;
            }
        }

        x_position = newX; // Aggiorna X

        // --- GRAVITÀ E MOVIMENTO Y ---
        if (!Y_movement_enabled) velocity.y += gravity * deltaTime;
        
        newY = y_position + velocity.y * deltaTime;
        onGround = false;

        // --- COLLISIONE SU Y ---
        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == owner) continue;

            const CollisionBox* objBox = nullptr;
            if (!getCollisionBox(gameObject, objBox)) continue;

            const float objX = gameObject->getX();
            const float objY = gameObject->getY();
            const float objW = objBox->getWidth();
            const float objH = objBox->getHeight();

            // Calcoliamo i bordi ASSOLUTI del personaggio per questo frame (Y, usando la X appena risolta)
            float myLeft   = x_position - halfW;
            float myRight  = x_position + halfW;
            float myTop    = newY - halfH;
            float myBottom = newY + halfH;

            // --- COLLISIONE SU Y ---
            const float objHalfW = objW * 0.5f;
            const float objHalfH = objH * 0.5f;
            float otherLeft   = objX - objHalfW;
            float otherRight  = objX + objHalfW;
            float otherTop    = objY - objHalfH;
            float otherBottom = objY + objHalfH;

            if (myLeft < otherRight && myRight > otherLeft &&
                myTop < otherBottom && myBottom > otherTop)
            {
                // Caduta
                if (velocity.y > 0.0f)
                {
                    newY = otherTop - halfH;
                    velocity.y = 0.0f;
                    onGround = true;
                }
                // Salto (testa che sbatte)
                else if (velocity.y < 0.0f)
                {
                    newY = otherBottom + halfH;
                    velocity.y = 0.0f;
                }
            }
        }

        y_position = newY; // Aggiorna Y
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dir.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dir.x += 1.0f;
        if (Y_movement_enabled && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dir.y -= 1.0f;
        if (Y_movement_enabled && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dir.y += 1.0f;

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
