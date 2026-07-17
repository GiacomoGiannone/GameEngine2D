#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Animator.hpp"
#include "TextureManager.hpp"
#include <iostream>

namespace GE
{
    class SpriteSheet
    {
        private:
            sf::Texture& texture;
            int frameWidth;
            int frameHeight;
            int columns;
            int rows;
        public:
    SpriteSheet(const std::string& textureId, const std::string& path, int frameWidth, int frameHeight)
        : texture(TextureManager::getInstance().loadTexture(textureId, path)),
          frameWidth(frameWidth), frameHeight(frameHeight),
          columns(texture.getSize().x / frameWidth),
          rows(texture.getSize().y / frameHeight)
    {
        std::cout << texture.getSize().x << " "
              << texture.getSize().y << std::endl;

    std::cout << frameWidth << " "
              << frameHeight << std::endl;

    std::cout << columns << " "
              << rows << std::endl;
    }
            
            sf::IntRect getFrame(int index);

            AnimationClip createClip(std::initializer_list<int> frameIndices, float frameDuration, bool loop);
    };
}