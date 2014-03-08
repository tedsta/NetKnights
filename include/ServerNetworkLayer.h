#ifndef SERVERNETWORKLAYER_H
#define SERVERNETWORKLAYER_H

#include <Fission/Core/EntityRef.h>
#include <Fission/Network/Connection.h>

#include "Events.h"
#include "PlayerManager.h"

class ServerNetworkLayer : public fsn::IPacketHandler
{
    public:
        ServerNetworkLayer(fsn::EventManager& eventMgr, fsn::EntityManager& entityMgr, fsn::Connection& conn, PlayerManager& playerMgr);

        void handlePacket(fsn::Packet& packet, int netID);

        void sendEntities(const std::vector<fsn::EntityRef>& entities, int netID = 0, int excludeID = 0);
        void sendCharacterInputs(const std::vector<CharacterInputEvent>& inputs, int netID = 0, int excludeID = 0);

    private:
        fsn::EventManager& mEventManager;
        fsn::EntityManager& mEntityManager;
        fsn::Connection& mConnection;
        PlayerManager& mPlayerManager;
};

#endif // SERVERNETWORKLAYER_H
