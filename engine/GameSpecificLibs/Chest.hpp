#pragma once

#include "core/Component.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>

namespace GE
{
    class GameObject;
    class SpriteRenderer;
    class Renderer;
}

namespace GE
{
    class ChestLogic : public GE::Component
    {
    public:
        enum class ChestState { Closed, Open };

    private:
        ChestState state{ChestState::Closed};
        float interactionRange{40.0f};
        const GE::GameObject* player{nullptr};
        bool wasKeyPressed{false};

        std::string openTextureId;
        std::filesystem::path openTexturePath;

        std::string lootName;
        sf::Text lootText;

    public:
        ChestLogic(const std::string& openTextureId,
                   const std::filesystem::path& openTexturePath,
                   const std::string& lootName);

        ChestLogic(const std::string& openTextureId,
                   const std::filesystem::path& openTexturePath);
        virtual ~ChestLogic();

        void setPlayer(const GE::GameObject* p) { player = p; }
        void setInteractionRange(float range) { interactionRange = range; }
        ChestState getState() const { return state; }
        bool isOpen() const { return state == ChestState::Open; }
        void setLoot(const std::string& lootItem);
        const std::string& getLoot() const { return lootName; }

        void open();

        void update(float deltaTime) override;
        void render(Renderer& renderer) override;
    };
}