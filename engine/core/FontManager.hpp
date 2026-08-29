// FontManager.hpp — stesso pattern del tuo TextureManager
#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <filesystem>

namespace GE
{
    class FontManager
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;
        FontManager() = default;

    public:
        static FontManager& getInstance()
        {
            static FontManager instance;
            return instance;
        }

        sf::Font& loadFont(const std::string& id, const std::filesystem::path& path)
        {
            auto it = fonts.find(id);
            if (it != fonts.end())
                return *it->second;

            auto font = std::make_unique<sf::Font>();
            if (!font->openFromFile(path))  // in SFML 3 è openFromFile, non loadFromFile
            {
                // gestisci l'errore come preferisci
            }

            auto& ref = *font;
            fonts[id] = std::move(font);
            return ref;
        }

        sf::Font& getFont(const std::string& id) { return *fonts.at(id); }
    };
}