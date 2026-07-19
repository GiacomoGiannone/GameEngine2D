#include "SpriteSheet.hpp"

#include <iostream>

namespace GE
{
    sf::IntRect SpriteSheet::getFrame(int index, int cropLeft, int cropRight, int cropTop, int cropBottom)
    {
        int col = index % columns;
        int row = index / columns;
        return {
        {
            col * cellWidth + regionX,
            row * cellHeight + regionY
        },
        {
            regionWidth,
            regionHeight
        }
    };
    }

    AnimationClip SpriteSheet::createClip(std::initializer_list<int> frameIndices, float frameDuration, bool loop,
                                            int cropLeft, int cropRight, int cropTop, int cropBottom)
    {
        std::cout << "Creating animation clip with frame duration: " << frameDuration << " and loop: " << loop << std::endl;
        AnimationClip clip;
        clip.texture = &texture;
        clip.frameDuration = frameDuration;
        clip.loop = loop;

         std::cout << "frames: ";

        for(int i : frameIndices)
            std::cout << i << ' ';

        std::cout << std::endl;

        for (int index : frameIndices)
        {
            clip.frames.push_back(getFrame(index, cropLeft, cropRight, cropTop, cropBottom));
        }
        std::cout << "Created animation clip with " << clip.frames.size() << " frames." << std::endl;
        return clip;
    }
}