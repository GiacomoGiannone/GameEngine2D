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
        currentFrame = 0;
        elapsedTime = 0;


        auto& animation = it->second;

        frameCounter++;
        if(frameCounter % 180 == 0)
        {
            std::cout << "Playing animation: " << name << std::endl;
            std::cout << "Texture size: "
                << animation.texture->getSize().x
                << "x"
                << animation.texture->getSize().y
                << std::endl;
        }


        spriteRenderer.setTexture(
            *animation.texture
        );


        spriteRenderer.setTextureRect(
            animation.frames[0]
        );
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

        currentFrame++;

        if(currentFrame >= animation.frames.size())
        {
            if(animation.loop)
            {
                currentFrame = 0;
            }
            else
            {
                currentFrame = animation.frames.size() - 1;
            }
        }

        spriteRenderer.setTextureRect(
            animation.frames[currentFrame]
        );

        auto rect = animation.frames[currentFrame];

        std::cout
            << "Frame "
            << currentFrame
            << " rect: "
            << rect.position.x << ", "
            << rect.position.y
            << " size "
            << rect.size.x << "x"
            << rect.size.y
            << std::endl;
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
}

//init frame counter
int GE::Animator::frameCounter = 0;

