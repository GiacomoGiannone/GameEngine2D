#pragma once

#include "core/Component.hpp"
#include "SFML/Graphics.hpp"
#include "TextureManager.hpp"

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
            sf::Texture& texture;
            Transform& transform;
            const std::string textureId;
        public:
            SpriteRenderer(const std::string& textureId, const std::filesystem::path& texturePath, Transform& transform);
            ~SpriteRenderer();

            void update(float deltaTime) override;
            void render(Renderer& renderer) override;
    };
}