#include "RectangleRenderer.hpp"

#include "Renderer.hpp"

namespace GE
{
    RectangleRenderer::RectangleRenderer(float width, float height, const sf::Color& color, Transform& transform) : transform(transform)
    {
        rectangle.setSize({width, height});
        rectangle.setFillColor(color);
        this->transform.setWidth(width);
        this->transform.setHeight(height);
    }

    void RectangleRenderer::update(float deltaTime)
    {
        (void)deltaTime;
    }

    void RectangleRenderer::render(Renderer& renderer)
    {
        rectangle.setPosition({transform.getX(), transform.getY()});
        renderer.draw(rectangle);
    }
}