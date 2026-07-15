#include "Renderer.hpp"

namespace GE
{
    Renderer::Renderer(int width, int height, const std::string& title)
    {
        window = new sf::RenderWindow(
            sf::VideoMode(sf::Vector2u{
                static_cast<unsigned int>(width),
                static_cast<unsigned int>(height)
            }),
            title
        );
        window->setFramerateLimit(60);
    }

    void Renderer::clear()
    {
        window->clear();
    }

    void Renderer::display()
    {
        window->display();
    }

    void Renderer::draw(const sf::Drawable& drawable)
    {
        window->draw(drawable);
    }

    bool Renderer::isOpen() const
    {
        return window->isOpen();
    }

    sf::RenderWindow* Renderer::getWindow() const
    {
        return window;
    }
}