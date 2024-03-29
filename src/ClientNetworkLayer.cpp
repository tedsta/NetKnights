#include "ClientNetworkLayer.h"

#include <iostream>

#include <Fission/Core/EntityRef.h>
#include <Fission/Rendering/Sprite.h>

#include "Components/Character.h"
#include "Components/CharacterAnimation.h"
#include "Components/HitPoints.h"
#include "Components/MeleeWeapon.h"

#include "AnimationEnums.h"
#include "Events.h"
#include "PacketTypes.h"

ClientNetworkLayer::ClientNetworkLayer(fsn::EventManager& eventMgr, fsn::EntityManager& entityMgr, fsn::Connection& conn) :
    mEventManager(eventMgr), mEntityManager(entityMgr), mConnection(conn)
{
    //ctor
}

void ClientNetworkLayer::handlePacket(fsn::Packet& packet, int netID)
{
    PacketID packetID;
    packet >> packetID;

    switch (packetID)
    {
        case CharacterSpawned:
        {
            int netID;

            packet >> netID;
            auto entity = mEntityManager.createEntityRef(mEntityManager.deserializeEntity(packet));

            if (netID == mConnection.getNetID())
            {
                mLocalEntity = entity;
                mEventManager.fireEvent(LocalPlayerSpawnedEvent(entity));
            }

            break;
        }

        case CharacterDied:
        {
            std::size_t entityUniqueID;
            packet >> entityUniqueID;

            auto entity = mEntityManager.createEntityRef(mEntityManager.getEntityIDFromUniqueID(entityUniqueID));

            if (entity.exists())
            {
                // Remove alive character part of entity
                entity.removeComponent<Character>();
                entity.removeComponent<HitPoints>();
                entity.removeComponent<MeleeWeapon>();

                // Apply death animation
                auto& anim = entity.getComponent<CharacterAnimation>();
                anim.setAnimation(KnightAnimation::Die, false);
            }

            break;
        }

        case CharacterInputs:
        {
            std::size_t count;
            packet >> count;

            // Note: this assumes that each characters' input has been sorted by sequence number
            for (std::size_t i = 0; i < count; i++)
            {
                std::size_t uniqueID;
                CharacterInput input;

                packet >> uniqueID;
                packet >> input;

                auto entity = mEntityManager.createEntityRef(mEntityManager.getEntityIDFromUniqueID(uniqueID));

                if (entity.exists() && entity != mLocalEntity)
                    mEventManager.fireEvent(CharacterInputEvent(entity, input));
            }

            break;
        }

        case CreateEntities:
        {
            std::size_t count;
            packet >> count;

            for (std::size_t i = 0; i < count; i++)
                mEntityManager.deserializeEntity(packet);

            break;
        }

        default:
        {
            break;
        }
    }
}

void ClientNetworkLayer::onCharacterInput(const CharacterInputEvent& event)
{
    if (event.characterEntity == mLocalEntity)
        sendPlayerInput(event.input);
}

void ClientNetworkLayer::login(const std::string& name, const std::string& pass)
{
    fsn::Packet packet;
    packet << PacketID(Login);
    packet << name;
    packet << pass;

    mConnection.send(packet, getHndID());
}

void ClientNetworkLayer::sendPlayerInput(const CharacterInput& input)
{
    fsn::Packet packet;
    packet << PacketID(PlayerInputed);
    packet << input;
    mConnection.send(packet, getHndID(), 0, 0, false);
}
