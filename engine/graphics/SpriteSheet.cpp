#include "SpriteSheet.hpp"

namespace GE
{
    sf::IntRect SpriteSheet::getFrame(int index)
    {
        int col = index % (texture.getSize().x / frameWidth);
        int row = index / (texture.getSize().x / frameWidth);
        return sf::IntRect(sf::Vector2i(col * frameWidth, row * frameHeight), sf::Vector2i(frameWidth, frameHeight));
    }

    AnimationClip SpriteSheet::createClip(std::initializer_list<int> frameIndices, float frameDuration, bool loop)
    {
        AnimationClip clip;
        clip.frameDuration = frameDuration;
        clip.loop = loop;

        for (int index : frameIndices)
        {
            clip.frames.push_back(getFrame(index));
        }

        return clip;
    }
}