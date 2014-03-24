#include "ServerNetworkLayer.h"

#include "CharacterMover.h"
#include "EntityTags.h"
#include "Events.h"
#include "PacketTypes.h"

ServerNetworkLayer::ServerNetworkLayer(fsn::EventManager& eventMgr, fsn::EntityManager& entityMgr, fsn::Connection& conn, PlayerManager& playerMgr) :
    mEventManager(eventMgr), mEntityManager(entityMgr), mConnection(conn), mPlayerManager(playerMgr)
{
    //ctor
}

void ServerNetworkLayer::handlePacket(fsn::Packet& packet, int netID)
{
    PacketID packetID;
    packet >> packetID;

    switch (packetID)
    {
        case Login:
        {
            std::string name;
            std::string pass;

            packet >> name >> pass;

            std::cout << name << " logged in.\n";

            // Tell the new client about all of the other players
            sendEntities(mEntityManager.getEntitiesWithTag(etags::Player), netID);

            // Spawn the player
            auto playerEntity = mPlayerManager.spawnPlayer(netID);

            // Construct a packet for the newly spawned player
            fsn::Packet spawnPacket;
            spawnPacket << PacketID(CharacterSpawned);
            spawnPacket << netID;
            playerEntity.serialize(spawnPacket);
            mConnection.send(spawnPacket, getHndID());

            mEventManager.fireEvent(CharacterSpawnedEvent(playerEntity));

            break;
        }

        case PlayerInputed:
        {
            auto player = mPlayerManager.findPlayerByNetID(netID);
            if (!player || !player->entity.exists()) // Invalid player
                break;

            CharacterInput input;

            packet >> input;

            mEventManager.fireEvent(CharacterInputEvent(player->entity, input));

            break;
        }

        default:
        {
            break;
        }
    }
}

void ServerNetworkLayer::onCharacterDeath(const CharacterDeathEvent& event)
{
    fsn::Packet packet;
    packet << PacketID(CharacterDied);
    packet << event.characterEntity.getUniqueID();

    mConnection.send(packet, getHndID());
}

void ServerNetworkLayer::sendEntities(const std::vector<fsn::EntityRef>& entities, int netID, int excludeID)
{
    fsn::Packet packet;
    packet << PacketID(CreateEntities);
    packet << entities.size();
    for (auto& entity : entities)
        entity.serialize(packet);
    mConnection.send(packet, getHndID(), netID, excludeID);
}

void ServerNetworkLayer::sendCharacterInputs(const std::vector<CharacterInputEvent>& inputs, int netID, int excludeID)
{
    if (inputs.size() == 0)
        return;

    fsn::Packet packet;
    packet << PacketID(CharacterInputs);
    packet << inputs.size();

    for (auto& input : inputs)
    {
        packet << input.characterEntity.getUniqueID();
        packet << input.input;
    }

    mConnection.send(packet, getHndID(), netID, excludeID, false);
}
