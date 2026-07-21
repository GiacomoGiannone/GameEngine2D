#pragma once

#include <utility> // for std::pair
#include <SFML/Window.hpp> // for sf::Keyboard and sf::Mouse

namespace GE
{
    enum class MouseButton
    {
        Left,
        Right,
        Middle
    };
    // Input class is responsible for handling user input events.
    // It provides an interface to query the state of keys and mouse buttons.
    class Input
    {
        private:
            // Internal state to track mouse button presses.
            static bool previousLeft;
            static bool currentLeft;
        public:
            // Check if a specific key is currently pressed.
            static bool isKeyPressed(int key);

            // Check if a specific mouse button is currently pressed.
            static bool isMouseButtonPressed(MouseButton button);
            static bool isMouseButtonJustPressed(MouseButton button); //returns true only if the button was just pressed in this frame

            // Get the current position of the mouse cursor.
            static std::pair<double, double> getMousePosition();

            // Set the mouse cursor position.
            static void setMousePosition(double x, double y);

            static void update();
    };

} // namespace GE