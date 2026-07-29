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

    TextureManager& TextureManager::getInstance()
    {
        static TextureManager instance;
        return instance;
    }

    void TextureManager::addTexture(const std::string& id, const sf::Image& image)
    {
        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromImage(image))
        {
            throw std::runtime_error("Failed to create texture from image: " + id);
        }
        texture->setSmooth(true);
        textures[id] = TextureEntry{std::move(texture), 1};
    }

    sf::Texture& TextureManager::loadTexture(const std::string& id, const std::filesystem::path& path)
    {
        auto it = textures.find(id);

        if(it != textures.end())
        {
            ++it->second.referenceCount;
            return *it->second.texture;
        }


        auto texture = std::make_unique<sf::Texture>();

        if(!texture->loadFromFile(path.string()))
        {
            throw std::runtime_error(
                "Failed to load texture: " + path.string()
            );
        }

        texture->setSmooth(true); // Enable smoothing for better visual quality
        auto& ref = *texture;

        textures[id] = TextureEntry{std::move(texture), 1};

        return ref;
    }

    sf::Texture& TextureManager::getTexture(const std::string& id)
    {
        auto it = textures.find(id);
        if (it == textures.end())
        {
            throw std::runtime_error("Texture not found: " + id);
        }
        return *it->second.texture;
    }

    bool TextureManager::exists(const std::string& id) const
    {
        return textures.find(id) != textures.end();
    }

    void TextureManager::unloadTexture(const std::string& id)
    {
        auto it = textures.find(id);

        if (it == textures.end())
        {
            return;
        }

        if (it->second.referenceCount > 0)
        {
            --it->second.referenceCount;
        }

        if (it->second.referenceCount == 0)
        {
            textures.erase(it);
        }
    }
}