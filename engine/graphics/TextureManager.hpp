#pragma once

#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>

namespace GE
{
    class TextureManager
    {
        private:
            std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
            //singleton pattern
            TextureManager* textureManagerInstance;
            TextureManager();
            ~TextureManager();
        public:
            static TextureManager* getInstance();
            sf::Texture& loadTexture(const std::string& id, const std::filesystem::path& path);
            sf::Texture& getTexture(const std::string& id);
            bool exists(const std::string& id) const;
            void unloadTexture(const std::string& id);
    };
}