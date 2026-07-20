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

        int cellWidth;
        int cellHeight;

        int columns;
        int rows;

        // posizione dello sprite dentro la cella
        int offsetX;
        int offsetY;

        // dimensione reale del personaggio
        int spriteWidth;
        int spriteHeight;


    public:

        SpriteSheet(
            const std::string& textureId,
            const std::string& path,

            int cellWidth,
            int cellHeight,

            int offsetX,
            int offsetY,

            int spriteWidth,
            int spriteHeight
        );


        sf::IntRect getFrame(int index);
        AnimationClip createClip(
            std::initializer_list<int> frameIndices,
            float frameDuration,
            bool loop
        );
    };
}