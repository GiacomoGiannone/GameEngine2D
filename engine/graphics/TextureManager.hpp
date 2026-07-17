#pragma once

#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>

namespace GE
{
    class TextureManager
    {
        private:
            struct TextureEntry
            {
                std::unique_ptr<sf::Texture> texture;
                std::size_t referenceCount{0};
            };

            std::unordered_map<std::string, TextureEntry> textures;
            //singleton pattern
            TextureManager();
            ~TextureManager();
        public:
            static TextureManager& getInstance();
            sf::Texture& loadTexture(const std::string& id, const std::filesystem::path& path);
            sf::Texture& getTexture(const std::string& id);
            bool exists(const std::string& id) const;
            void unloadTexture(const std::string& id);
            // Add a texture from an existing sf::Image
            void addTexture(const std::string& id, const sf::Image& image);
    };
}