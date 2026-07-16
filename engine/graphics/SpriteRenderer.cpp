#include "SpriteRenderer.hpp"
#include "core/Transform.hpp"
#include "Renderer.hpp"

namespace GE
{
    SpriteRenderer::SpriteRenderer(const std::string& textureId, const std::filesystem::path& texturePath, Transform& transform)
        : Component(), texture(TextureManager::getInstance()->loadTexture(textureId, texturePath)), transform(transform), textureId(textureId)
    {
    }

    void SpriteRenderer::update(float deltaTime)
    {
        // No update logic needed for the sprite renderer.
    }

    void SpriteRenderer::render(Renderer& renderer)
    {
        sf::Sprite sprite(texture);
        sprite.setPosition(sf::Vector2f(transform.getX(), transform.getY()));
        renderer.draw(sprite);
    }

    SpriteRenderer::~SpriteRenderer()
    {
        //call the texture manager to unload the texture
        TextureManager::getInstance()->unloadTexture(textureId);
    }
}