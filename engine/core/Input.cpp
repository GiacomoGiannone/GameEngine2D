#include "Input.hpp"
#include <iostream>
#include "Engine.hpp"
#include "Scene.hpp"
#include "Graphics/Renderer.hpp"

namespace GE
{
    bool Input::isKeyPressed(int key)
    {
        //wrap SFML function to check if a key is pressed
        return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
    }

    bool Input::isMouseButtonPressed(MouseButton button)
    {
        switch (button)
        {
            case MouseButton::Left:
                // Check if the left mouse button is pressed.
                return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            case MouseButton::Right:
                // Check if the right mouse button is pressed.
                return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            case MouseButton::Middle:
                // Check if the middle mouse button is pressed.
                return sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);
            default:
                return false; // Unknown button
        }
        return false; // Placeholder implementation
    }

    std::pair<double, double> Input::getMousePosition()
    {
        // Implementation to get the current position of the mouse cursor.
        // This would typically involve querying the underlying windowing system.
        return std::make_pair(0.0, 0.0); // Placeholder implementation
    }

    void Input::setMousePosition(double x, double y)
    {
        // Implementation to set the mouse cursor position.
        // This would typically involve querying the underlying windowing system.
    }

    void Input::update()
    {
        //keep the internal state previousLeft and currentLeft to check wheter we are pressing the left mouse button or clicked it just once
        previousLeft = currentLeft;
        currentLeft = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        previousRight = currentRight;
        currentRight = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

        mouseWheelDelta = 0.0f; // Reset the mouse wheel delta for the current frame

        auto* window = Engine::getInstance()->getRenderer()->getWindow();
        while (const auto event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window->close();
            }
            else if (event->is<sf::Event::MouseWheelScrolled>())
            {
                auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>();
                mouseWheelDelta += wheel->delta; // Accumulate the mouse wheel delta for the current frame
            }
        }
    }

    bool Input::isMouseButtonJustPressed(MouseButton button)
    {
        switch(button)
        {
            case MouseButton::Left:
                return currentLeft && !previousLeft;

            case MouseButton::Right:
                return currentRight && !previousRight;

            default:
                return false;
        }
    }

    float Input::getMouseWheelDelta()
    {
        return mouseWheelDelta;
    }
    bool Input::previousLeft = false;
    bool Input::currentLeft = false;
    bool Input::previousRight = false;
    bool Input::currentRight = false;
    float Input::mouseWheelDelta = 0.0f; // Initialize the mouse wheel delta
}