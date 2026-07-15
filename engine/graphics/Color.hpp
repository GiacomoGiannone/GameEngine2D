#pragma once

//wrapper for sf::Color to be used in the engine

#include <SFML/Graphics/Color.hpp>
#include <cstdint>

namespace GE
{
    class Color 
    {
    public:
        using Channel = std::uint8_t;

        Color() = default;

        Color(Channel red, Channel green, Channel blue, Channel alpha = 255)
            : red(red), green(green), blue(blue), alpha(alpha)
        {
        }

        Color(const sf::Color& color)
            : red(color.r), green(color.g), blue(color.b), alpha(color.a)
        {
        }

        [[nodiscard]] Channel getRed() const { return red; }
        [[nodiscard]] Channel getGreen() const { return green; }
        [[nodiscard]] Channel getBlue() const { return blue; }
        [[nodiscard]] Channel getAlpha() const { return alpha; }

        void setRed(Channel value) { red = value; }
        void setGreen(Channel value) { green = value; }
        void setBlue(Channel value) { blue = value; }
        void setAlpha(Channel value) { alpha = value; }

        [[nodiscard]] sf::Color toSFML() const
        {
            return sf::Color(red, green, blue, alpha);
        }

        [[nodiscard]] operator sf::Color() const
        {
            return toSFML();
        }

    private:
        Channel red{0};
        Channel green{0};
        Channel blue{0};
        Channel alpha{255};
    };

} // namespace GE