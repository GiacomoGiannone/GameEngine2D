#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

namespace GE
{
    class AudioManager
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBuffers;
        std::vector<std::unique_ptr<sf::Sound>> sounds;

        std::unique_ptr<sf::Music> music;

        float musicVolume = 100.0f;
        float soundVolume = 100.0f;

        AudioManager() = default;

    public:
        AudioManager(const AudioManager&) = delete;
        AudioManager& operator=(const AudioManager&) = delete;

        static AudioManager& getInstance();

        // MUSICA
        bool playMusic(
            const std::string& id,
            const std::string& path,
            bool loop = true
        );

        void stopMusic();
        void pauseMusic();
        void resumeMusic();

        void setMusicVolume(float volume);
        float getMusicVolume() const;

        // SOUND EFFECT
        bool loadSound(
            const std::string& id,
            const std::string& path
        );

        void playSound(const std::string& id);

        void setSoundVolume(float volume);
        float getSoundVolume() const;
        void update();
    };
}