#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
    std::cout << "SFML test avviato\n";


    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "GameEngine Test"
    );


    while(window.isOpen())
    {
        while(auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }


        window.clear(sf::Color::Black);

        window.display();
    }


    return 0;
}