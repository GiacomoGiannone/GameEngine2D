#include "Input.hpp"

namespace GE
{
    bool Input::isKeyPressed(int key)
    {
        // Implementation to check if the specified key is pressed.
        // This would typically involve querying the underlying windowing system.
        return false; // Placeholder implementation
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
    }

    bool Input::isMouseButtonJustPressed(MouseButton button)
    {
        switch(button)
        {
            case MouseButton::Left:
                return currentLeft && !previousLeft;

            default:
                return false;
        }
    }
    bool Input::previousLeft = false;
    bool Input::currentLeft = false;
}