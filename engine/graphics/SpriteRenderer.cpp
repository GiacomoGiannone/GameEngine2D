#include "SpriteRenderer.hpp"
#include "core/Transform.hpp"
#include "Renderer.hpp"

namespace GE
{
    SpriteRenderer::SpriteRenderer(const std::string& textureId, const std::filesystem::path& texturePath, Transform& transform, int UpperCorner, int LowerCorner, int Width, int Height)
        : Component(), texture(TextureManager::getInstance()->loadTexture(textureId, texturePath)), transform(transform), textureId(textureId)
    {
        if (Width > 0 && Height > 0)
        {
            sprite = new sf::Sprite(texture, sf::IntRect{{UpperCorner, LowerCorner}, {Width, Height}});
        }
        else
        {
            sprite = new sf::Sprite(texture);
        }
    }

    void SpriteRenderer::update(float deltaTime)
    {
        // No update logic needed for the sprite renderer.
    }

    void SpriteRenderer::render(Renderer& renderer)
    {

        sprite->setPosition(sf::Vector2f(transform.getX(), transform.getY()));
        renderer.draw(*sprite);
    }

    SpriteRenderer::~SpriteRenderer()
    {
        //call the texture manager to unload the texture
        TextureManager::getInstance()->unloadTexture(textureId);
    }
}