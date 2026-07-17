#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <unordered_map>
#include <string>

#include "core/Component.hpp"

namespace GE
{
    class SpriteRenderer;
    class Renderer;

    struct AnimationClip
    {
        std::vector<sf::IntRect> frames;

        float frameDuration;
        bool loop;
    };


    class Animator : public Component
    {
        private:
            std::unordered_map<std::string, AnimationClip> animations;
            SpriteRenderer& spriteRenderer;
            std::string currentAnimation;
            size_t currentFrame = 0;
            float elapsedTime = 0.0f;
        public:
            Animator(SpriteRenderer& spriteRenderer);
            void addAnimation(const std::string& name, const AnimationClip& animation);
            void play(const std::string& name);
            void update(float deltaTime) override;
            void render(Renderer& renderer) override;
    };

}
