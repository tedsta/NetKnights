#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <string>
#include <vector>

#include <Fission/Core/EntityManager.h>
#include <Fission/Core/EntityRef.h>

struct Player
{
    std::size_t ID;
    int netID;
    std::string name;
    fsn::EntityRef entity;
};

class PlayerManager
{
    public:
        PlayerManager(fsn::EntityManager& entityMgr);

        bool login(const std::string& name, int netID);

        const fsn::EntityRef& spawnPlayer(int netID);

        Player* findPlayerByID(std::size_t ID);
        Player* findPlayerByNetID(int netID);
        Player* findPlayerByName(const std::string& name);

    private:
        fsn::EntityManager& mEntityManager;

        std::vector<Player> mPlayers;

        std::size_t mNextID; // Next player's ID
};

#endif // PLAYERMANAGER_H
