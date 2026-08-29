#include "CollisionBox.hpp"
#include "Graphics/Renderer.hpp"
#include "GameObject.hpp"

namespace GE
{
    void CollisionBox::render(Renderer& renderer)
    {
        if (!debugPrint || getOwner() == nullptr) return;

            box.setPosition(sf::Vector2f(
                getOwner()->getX() - (box.getSize().x * 0.5f),
                getOwner()->getY() - (box.getSize().y * 0.5f)
            ));
            box.setFillColor(sf::Color(0, 0, 255, 100)); // blu, per distinguerlo dal rosso dell'attacco
            renderer.draw(box);
    }
}