#include "Animator.hpp"

#include "SpriteRenderer.hpp"

#include <iostream>

namespace GE
{
    Animator::Animator(SpriteRenderer& spriteRenderer) : spriteRenderer(spriteRenderer)
    {

    }

    void Animator::addAnimation(const std::string& name,const AnimationClip& animation)
    {
        std::cout << "Adding animation: " << name << std::endl;
        animations[name] = animation;
    }

    void Animator::play(const std::string& name)
    {
        auto it = animations.find(name);

        if(it == animations.end())
        {
            std::cout << "Animation not found: " << name << std::endl;
            return;
        }

        currentAnimation = name;
        elapsedTime = 0.f;

        auto& animation = it->second;

        switch(animation.playbackMode)
        {
            case PlaybackMode::Forward:
            case PlaybackMode::PingPong:
                currentFrame = 0;
                direction = 1;
                break;

            case PlaybackMode::Reverse:
                currentFrame = animation.frames.size() - 1;
                direction = -1;
                break;
        }

        spriteRenderer.setTexture(*animation.texture);
        spriteRenderer.setTextureRect(animation.frames[currentFrame]);
    }

    void Animator::update(float deltaTime)
    {
        //print the state of the animator
        //std::cout << "Current Animation: " << currentAnimation << ", Current Frame: "<< currentFrame << ", Elapsed Time: " << elapsedTime << std::endl;
        for(auto& transition : transitions)
        {
            if(transition.fromAnimation == currentAnimation)
            {
                if(transition.condition())
                {
                    play(transition.toAnimation);
                    break;
                }
            }
        }   

        if(currentAnimation.empty())
            return;

        auto& animation = animations[currentAnimation];

        if(animation.frames.empty())
            return;

        elapsedTime += deltaTime;

        if(elapsedTime < animation.frameDuration)
            return;

        elapsedTime = 0.0f;

        currentFrame+= direction;

        /////////////////////////////////////////
        //CHECK FOR ANIMATION EVENTS
        for(auto& event : animation.events)
        {
            if(event.startFrame <= static_cast<std::size_t>(currentFrame) && event.endFrame >= static_cast<std::size_t>(currentFrame))
            {
                event.callback();
            }
        }

        /////////////////////////////////////////
        //FORWARD PLAYBACK MODE
        if(animation.playbackMode == PlaybackMode::Forward)
        {
            if(currentFrame >= static_cast<int>(animation.frames.size()))
            {
                if(animation.loop)
                    currentFrame = 0;
                else
                    currentFrame = static_cast<int>(animation.frames.size()) - 1;
            }
        }

        /////////////////////////////////////////
        //REVERSE PLAYBACK MODE
        else if(animation.playbackMode == PlaybackMode::Reverse)
        {
            if(currentFrame < 0)
            {
                if(animation.loop)
                    currentFrame = static_cast<int>(animation.frames.size()) - 1;
                else
                    currentFrame = 0;
            }
        }

        /////////////////////////////////////////
        //PINGPONG PLAYBACK MODE
        else if(animation.playbackMode == PlaybackMode::PingPong)
        {
            if(currentFrame >= static_cast<int>(animation.frames.size()))
            {
                direction = -1;
                currentFrame = static_cast<int>(animation.frames.size()) - 2; // Go back to the second last frame
            }
            else if(currentFrame < 0)
            {
                direction = 1;
                currentFrame = 1; // Go forward to the second frame
            }
        }

        spriteRenderer.setTextureRect(
            animation.frames[currentFrame]
        );

        auto rect = animation.frames[currentFrame];
    }

    void Animator::render(Renderer& renderer)
    {
        // Animator doesn't render anything itself - the SpriteRenderer handles that
        (void)renderer;
    }

    void Animator::addTransition(const std::string& fromAnimation, const std::string& toAnimation, std::function<bool()> condition)
    {
        transitions.push_back({fromAnimation, toAnimation, condition});
    }

    void Animator::setFlipped(bool flipped)
    {
        spriteRenderer.setScale(
            flipped ? -1.0f : 1.0f,
            1.0f
        );
    }

    const std::string& Animator::getCurrentAnimation() const
    {
        return currentAnimation;
    }

    int Animator::getCurrentFrame() const
    {
        return currentFrame;
    }

    float Animator::getMovementMultiplier() const
    {
        if(currentAnimation.empty())
            return 1.0f;

        auto& animation = animations.at(currentAnimation);
        return animation.movementMultiplier;
    }
}

