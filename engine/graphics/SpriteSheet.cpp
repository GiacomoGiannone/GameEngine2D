#include "SpriteSheet.hpp"

#include <iostream>

namespace GE
{
    SpriteSheet::SpriteSheet(
        const std::string& textureId,
        const std::string& path,

        int cellWidth,
        int cellHeight,

        int offsetX,
        int offsetY,

        int spriteWidth,
        int spriteHeight
    )
    :
    texture(TextureManager::getInstance().loadTexture(textureId,path)),
    cellWidth(cellWidth),
    cellHeight(cellHeight),
    offsetX(offsetX),
    offsetY(offsetY),
    spriteWidth(spriteWidth),
    spriteHeight(spriteHeight)
    {

        columns = texture.getSize().x / cellWidth;
        rows = texture.getSize().y / cellHeight;

    }

    sf::IntRect SpriteSheet::getFrame(int index)
    {
        int col = index % columns;
        int row = index / columns;


        int cellX = col * cellWidth;
        int cellY = row * cellHeight;


        return sf::IntRect(
            {
                cellX + offsetX,
                cellY + offsetY
            },
            {
                spriteWidth,
                spriteHeight
            }
        );
    }

    AnimationClip GE::SpriteSheet::createClip(std::initializer_list<int> frameIndices,float frameDuration,bool loop)
    {
        std::cout 
            << "Creating animation clip with frame duration: "
            << frameDuration
            << " and loop: "
            << loop
            << std::endl;

        AnimationClip clip;

        clip.texture = &texture;
        clip.frameDuration = frameDuration;
        clip.loop = loop;


        std::cout << "frames: ";

        for(int i : frameIndices)
        {
            std::cout << i << " ";
        }

        std::cout << std::endl;


        for(int index : frameIndices)
        {
            clip.frames.push_back(
                getFrame(index)
            );
        }


        std::cout 
            << "Created animation clip with "
            << clip.frames.size()
            << " frames."
            << std::endl;


        return clip;
    }
}