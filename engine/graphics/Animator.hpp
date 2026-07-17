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
        sf::Texture* texture;
        std::vector<sf::IntRect> frames;

        float frameDuration;
        bool loop;
    };

    struct AnimationState
    {
        std::string name;
        AnimationClip clip;
    };

    struct AnimationTransition
    {
        std::string fromAnimation;
        std::string toAnimation;
        std::function<bool()> condition;
    };


    class Animator : public Component
    {
        private:
            std::unordered_map<std::string, AnimationClip> animations;
            SpriteRenderer& spriteRenderer;
            std::vector<AnimationTransition> transitions;
            std::string currentAnimation;
            size_t currentFrame = 0;
            float elapsedTime = 0.0f;
        public:
            Animator(SpriteRenderer& spriteRenderer);
            void addAnimation(const std::string& name, const AnimationClip& animation);
            void play(const std::string& name);
            void update(float deltaTime) override;
            void render(Renderer& renderer) override;
            void addTransition(const std::string& fromAnimation, const std::string& toAnimation, std::function<bool()> condition);
    };

}
