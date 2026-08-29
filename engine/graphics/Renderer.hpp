#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace GE
{
    class Renderer
    {
        private:
            sf::RenderWindow* window;
        public:
            Renderer(int width, int height, const std::string& title);
            virtual ~Renderer() = default;

            void clear();
            void display();
            void draw(const sf::Drawable& drawable);
            bool isOpen() const;
            sf::RenderWindow* getWindow() const;    
            const sf::View& getView() { return window->getView(); }
            void setView(const sf::View& view) { window->setView(view); }
            void resetView() { window->setView(window->getDefaultView()); }
    };
}

