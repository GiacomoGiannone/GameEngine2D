#include "SpriteRenderer.hpp"
#include "core/Transform.hpp"
#include "Renderer.hpp"

namespace GE
{
    SpriteRenderer::SpriteRenderer(const std::string& textureId, const std::filesystem::path& texturePath, Transform& transform, int UpperCorner, int LowerCorner, int Width, int Height)
        : texture(&TextureManager::getInstance().loadTexture(textureId, texturePath)), transform(transform), textureId(textureId)
    {
        sprite = new sf::Sprite(*texture);
        sprite->setTexture(*texture);

        if (Width > 0 && Height > 0)
        {
            sprite->setTextureRect(sf::IntRect(sf::Vector2i(UpperCorner, LowerCorner), sf::Vector2i(Width, Height)));
        }

        // --- AGGIUNGI QUESTA RIGA ---
        sprite->setOrigin({Width / 2.0f, Height / 2.0f});
        // ---------------------------
    }

    const sf::Texture& SpriteRenderer::getTexture() const
    {
        return *texture;
    }

    void SpriteRenderer::setTextureRect(const sf::IntRect& rect)
    {
        sprite->setTextureRect(rect);

        sprite->setOrigin({rect.size.x / 2.f,rect.size.y / 2.f});
    }

    void SpriteRenderer::update(float deltaTime)
    {
        // No update logic needed for the sprite renderer.
        auto r = sprite->getTextureRect();

    std::cout
        << r.position.x << " "
        << r.position.y << " "
        << r.size.x << " "
        << r.size.y
        << std::endl;
    }

    void SpriteRenderer::render(Renderer& renderer)
    {
        sprite->setPosition({transform.getX(), transform.getY()});
        renderer.draw(*sprite);

        // DEBUG
        sf::RectangleShape outline;

        const auto rect = sprite->getTextureRect();

        outline.setSize({
            static_cast<float>(rect.size.x),
            static_cast<float>(rect.size.y)
        });

        outline.setOrigin({rect.size.x / 2.f, rect.size.y / 2.f});
        outline.setPosition(sprite->getPosition());

        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineThickness(1.f);
        outline.setOutlineColor(sf::Color::Red);

        renderer.draw(outline);

        sf::CircleShape pivot(3.f);
        pivot.setFillColor(sf::Color::Green);

        pivot.setOrigin({3.f, 3.f});
        pivot.setPosition(sprite->getPosition());

        renderer.draw(pivot);
    }

    SpriteRenderer::~SpriteRenderer()
    {
        // Call the texture manager to unload the texture
        TextureManager::getInstance().unloadTexture(textureId);
        delete sprite;
    }

    void SpriteRenderer::setTexture(sf::Texture& texture)
    {
        this->texture = &texture;
        sprite->setTexture(texture);
    }

    void SpriteRenderer::setScale(float scaleX, float scaleY)
    {
        sprite->setScale({scaleX, scaleY});
    }
}