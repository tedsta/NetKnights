#ifndef CLIENTNETWORKLAYER_H
#define CLIENTNETWORKLAYER_H

#include <Fission/Core/EntityManager.h>

#include <Fission/Network/Connection.h>

#include "Events.h"

class ClientNetworkLayer : public fsn::IPacketHandler
{
    public:
        ClientNetworkLayer(fsn::EventManager& eventMgr, fsn::EntityManager& entityMgr, fsn::Connection& conn);

        void handlePacket(fsn::Packet& packet, int netID);

        void onCharacterInput(const CharacterInputEvent& event);

        void login(const std::string& name, const std::string& pass);
        void sendPlayerInput(const CharacterInput& input);

    private:
        fsn::EventManager& mEventManager;
        fsn::EntityManager& mEntityManager;
        fsn::Connection& mConnection;

        fsn::EntityRef mLocalEntity;
};

#endif // CLIENTNETWORKLAYER_H
