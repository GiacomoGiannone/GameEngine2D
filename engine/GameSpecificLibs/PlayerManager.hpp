#pragma once

namespace GE
{
    class GameObject;
}

namespace GE
{
    class PlayerManager
    {
    private:
        static PlayerManager* instance;
        GameObject* player;
        PlayerManager() : player(nullptr) {}
    public:
        static PlayerManager* getInstance()
        {
            if (!instance)
            {
                instance = new PlayerManager();
            }
            return instance;
        }

        void setPlayer(GameObject* p) { player = p; }
        GameObject* getPlayer() const { return player; }
        bool DestroyPlayer();
        void Update();
    };
}