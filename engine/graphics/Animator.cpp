#include "Animator.hpp"

#include "SpriteRenderer.hpp"

namespace GE
{
    Animator::Animator(SpriteRenderer& spriteRenderer) : spriteRenderer(spriteRenderer)
    {

    }

    void Animator::addAnimation(const std::string& name,const AnimationClip& animation)
    {
        animations[name] = animation;
    }

    void Animator::play(const std::string& name)
    {
        if(currentAnimation == name)
            return;


        auto it = animations.find(name);

        if(it == animations.end())
            return;


        currentAnimation = name;

        currentFrame = 0;

        elapsedTime = 0.0f;


        spriteRenderer.setTextureRect(
            it->second.frames[0]
        );
    }

    void Animator::update(float deltaTime)
    {
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
    }

    void Animator::render(Renderer& renderer)
    {
        // Animator doesn't render anything itself - the SpriteRenderer handles that
        (void)renderer;
    }
}
