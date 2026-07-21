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

    enum class PlaybackMode
    {
        Forward,
        Reverse,
        PingPong
    };

    struct AnimationClip
    {
        sf::Texture* texture;
        std::vector<sf::IntRect> frames;

        float frameDuration;
        bool loop;

        PlaybackMode playbackMode = PlaybackMode::Forward;
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
            int currentFrame = 0;
            float elapsedTime = 0.0f;
            int direction;
        public:
            Animator(SpriteRenderer& spriteRenderer);
            void addAnimation(const std::string& name, const AnimationClip& animation);
            void play(const std::string& name);
            void update(float deltaTime) override;
            void render(Renderer& renderer) override;
            void addTransition(const std::string& fromAnimation, const std::string& toAnimation, std::function<bool()> condition);
            void setFlipped(bool flipped);
            const std::string& getCurrentAnimation() const;
            int getCurrentFrame() const;
            AnimationClip& getAnimation(const std::string& name) { return animations.at(name); }
    };

}
