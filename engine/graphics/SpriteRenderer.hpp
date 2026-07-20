#pragma once

#include "core/Component.hpp"
#include "SFML/Graphics.hpp"
#include "TextureManager.hpp"
#include <iostream>

namespace GE
{
    class Transform;
}

namespace GE
{
    class SpriteRenderer : public Component
    {
        private:
            //use the texture manager to load the texture
            sf::Texture* texture;
            Transform& transform;
            const std::string textureId;
            sf::Sprite* sprite;
        public:
            SpriteRenderer() = default;
            SpriteRenderer(const std::string& textureId, const std::filesystem::path& texturePath, 
                Transform& transform, int UpperCorner = 0, int LowerCorner = 0, int Width = 0, int Height = 0);
            ~SpriteRenderer();

            void setScale(float scaleX, float scaleY);

            const sf::Texture& getTexture() const;
            void setTextureRect(const sf::IntRect& rect);
            void setTexture(sf::Texture& texture);

            void update(float deltaTime) override;
            void render(Renderer& renderer) override;
    };
}