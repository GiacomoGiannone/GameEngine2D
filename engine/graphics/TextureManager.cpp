#include "TextureManager.hpp"

namespace GE
{
    TextureManager::TextureManager()
    {
    }

    TextureManager::~TextureManager()
    {
        //erase every texture in the map
        textures.clear();
    }

    TextureManager* TextureManager::getInstance()
    {
        static TextureManager instance;
        return &instance;
    }

    sf::Texture& TextureManager::loadTexture(const std::string& id, const std::filesystem::path& path)
    {
        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(path.string()))
        {
            throw std::runtime_error("Failed to load texture: " + path.string());
        }
        textures[id] = std::move(texture);
        return *textures[id];
    }

    sf::Texture& TextureManager::getTexture(const std::string& id)
    {
        auto it = textures.find(id);
        if (it == textures.end())
        {
            throw std::runtime_error("Texture not found: " + id);
        }
        return *it->second;
    }

    bool TextureManager::exists(const std::string& id) const
    {
        return textures.find(id) != textures.end();
    }

    void TextureManager::unloadTexture(const std::string& id)
    {
        textures.erase(id);
    }
}