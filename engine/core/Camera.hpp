#pragma once

#include <SFML/Graphics.hpp>

namespace GE
{
    class GameObject;
}

namespace GE
{
    // Wrapper around SFML's sf::View for world camera control.
    class Camera
    {
    private:
        float x_position;
        float y_position;
        float zoom;
        float smoothSpeed;
        // Which GameObject the camera should follow (if any).
        const GE::GameObject* target;
        sf::View view;

        void syncView();
        // Keep the logical 800x600 aspect ratio by letterboxing the viewport.
        void applyLetterbox(int windowWidth, int windowHeight);

    public:

        Camera();
        Camera(float x, float y, float z);
        virtual ~Camera() = default;

        void setPosition(float x, float y);
        void setZoom(float z);
        void setSmoothSpeed(float speed) { smoothSpeed = speed; }
        float getX() const { return x_position; }
        float getY() const { return y_position; }
        float getZoom() const { return zoom; }
        float getSmoothSpeed() const { return smoothSpeed; }
        const GE::GameObject* getTarget() const { return target; }
        void setTarget(const GE::GameObject* obj) { target = obj; }
        void setViewportSize(int width, int height) { view.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height))); }

        // Follow the target (if set) and keep the SFML view in sync.
        void update(float deltaTime);
        // Apply this camera's view to the render window.
        void applyTo(sf::RenderWindow& window);
    };
}