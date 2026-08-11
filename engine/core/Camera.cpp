#include "Camera.hpp"
#include "GameObject.hpp"
#include <algorithm>
#include "Input.hpp"

namespace GE
{
    Camera::Camera()
        : x_position(0.0f), y_position(0.0f), zoom(1.0f), smoothSpeed(5.0f), target(nullptr),
          view(sf::FloatRect({0.0f, 0.0f}, {800.0f, 600.0f}))
    {
        syncView();
    }

    Camera::Camera(float x, float y, float z)
        : x_position(x), y_position(y), zoom(z), smoothSpeed(5.0f), target(nullptr),
          view(sf::FloatRect({0.0f, 0.0f}, {800.0f, 600.0f}))
    {
        syncView();
    }

    void Camera::setPosition(float x, float y)
    {
        x_position = x;
        y_position = y;
        syncView();
    }

    void Camera::setZoom(float z)
    {
        if (z <= 0.0f)
        {
            return;
        }

        zoom = z;
        syncView();
    }

    void Camera::syncView()
    {
        // Keep the logical view size at 800x600 and scale with zoom.
        const float viewWidth = 800.0f / zoom;
        const float viewHeight = 600.0f / zoom;
        view.setSize(sf::Vector2f(viewWidth, viewHeight));
        view.setCenter(sf::Vector2f(x_position, y_position));
    }

    void Camera::applyLetterbox(int windowWidth, int windowHeight)
    {
        if (windowWidth <= 0 || windowHeight <= 0)
        {
            return;
        }

        const float windowRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
        const float viewRatio = view.getSize().x / view.getSize().y;
        float sizeX = 1.f;
        float sizeY = 1.f;
        float posX = 0.f;
        float posY = 0.f;

        if (windowRatio < viewRatio)
        {
            sizeY = windowRatio / viewRatio;
            posY = (1.f - sizeY) / 2.f;
        }
        else
        {
            sizeX = viewRatio / windowRatio;
            posX = (1.f - sizeX) / 2.f;
        }

        view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
    }


    void Camera::update(float deltaTime)
    {
        if (target != nullptr)
        {
            // Center the camera on the target's bounds, not its top-left corner.
            const float targetWidth = target->getTransform().getWidth();
            const float targetHeight = target->getTransform().getHeight();
            const float targetX = target->getX() + targetWidth * 0.5f;
            const float targetY = target->getY() + targetHeight * 0.5f;

            // Linear interpolation with smoothSpeed for a delayed, smooth follow.
            const float t = std::clamp(smoothSpeed * deltaTime, 0.0f, 1.0f);
            x_position = x_position + (targetX - x_position) * t;
            y_position = y_position + (targetY - y_position) * t;

        }

        float wheel = Input::getMouseWheelDelta();
        if (wheel != 0.0f)
        {
            // Adjust zoom based on mouse wheel input.
            const float zoomFactor = 1.1f; // Zoom in/out factor
            if (wheel > 0.0f)
            {
                //clamp the zoom to a maximum value to prevent excessive zooming in
                if (zoom * zoomFactor > 2.0f) // Maximum zoom level
                {
                    setZoom(2.0f);
                }
                else
                {
                    setZoom(zoom * zoomFactor);
                }
            }
            else
            {
                //clamp the zoom to a minimum value to prevent excessive zooming out
                if (zoom / zoomFactor < 0.1f) // Minimum zoom level
                {
                    setZoom(0.5f);
                }
                else
                {
                    setZoom(zoom / zoomFactor);
                }
            }
        }

        syncView();
    }

    void Camera::applyTo(sf::RenderWindow& window)
    {
        syncView();

        const auto windowSize = window.getSize();
        applyLetterbox(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));

        window.setView(view);
    }

}
