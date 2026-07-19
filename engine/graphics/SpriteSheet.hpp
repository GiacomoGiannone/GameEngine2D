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

            int columns;
            int rows;

            int cellWidth;
            int cellHeight;

            int regionX;
            int regionY;

            int regionWidth;
            int regionHeight;
        public:
    SpriteSheet(const std::string& textureId, const std::string& path, int cellWidth, int cellHeight, int regionX = 0, int regionY = 0, int regionWidth = -1, int regionHeight = -1)
        : texture(TextureManager::getInstance().loadTexture(textureId, path)),
          cellWidth(cellWidth), cellHeight(cellHeight),
          columns(texture.getSize().x / cellWidth),
          rows(texture.getSize().y / cellHeight),
          regionX(regionX),
          regionY(regionY),
          regionWidth(regionWidth == -1 ? cellWidth : regionWidth),
          regionHeight(regionHeight == -1 ? cellHeight : regionHeight)
    {}
                
            sf::IntRect getFrame(int index, int cropLeft, int cropRight, int cropTop, int cropBottom);

            AnimationClip createClip(std::initializer_list<int> frameIndices, float frameDuration, bool loop,
                                    int cropLeft, int cropRight, int cropTop, int cropBottom);
    };
}