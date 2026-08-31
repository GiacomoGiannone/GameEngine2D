#include "AudioManager.hpp"
#include <algorithm>

namespace GE
{
    AudioManager& AudioManager::getInstance()
    {
        static AudioManager instance;
        return instance;
    }

    bool AudioManager::playMusic(const std::string& id,const std::string& path,bool loop)
    {
        music = std::make_unique<sf::Music>();

        if (!music->openFromFile(path))
        {
            return false;
        }

        music->setLooping(loop);
        music->setVolume(musicVolume);
        music->play();

        return true;
    }

    void AudioManager::stopMusic()
    {
        if (music)
        {
            music->stop();
        }
    }

    void AudioManager::pauseMusic()
    {
        if (music)
        {
            music->pause();
        }
    }

    void AudioManager::resumeMusic()
    {
        if (music)
        {
            music->play();
        }
    }

    bool AudioManager::loadSound(const std::string& id,const std::string& path)
    {
        auto buffer = std::make_shared<sf::SoundBuffer>();

        if (!buffer->loadFromFile(path))
        {
            return false;
        }

        soundBuffers[id] = buffer;

        return true;
    }

    void AudioManager::playSound(const std::string& id)
    {
        auto it = soundBuffers.find(id);

        if (it == soundBuffers.end())
            return;

        auto sound = std::make_unique<sf::Sound>(*it->second);

        sound->setVolume(soundVolume);
        sound->play();

        sounds.push_back(std::move(sound));
    }

    void AudioManager::update()
    {
        sounds.erase(
            std::remove_if(
                sounds.begin(),
                sounds.end(),
                [](const auto& sound)
                {
                    return sound->getStatus() == sf::Sound::Status::Stopped;
                }
            ),
            sounds.end()
        );
    }
}