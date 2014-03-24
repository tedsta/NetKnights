#ifndef SERVERCHARACTERPREDICTIONSYSTEM_H
#define SERVERCHARACTERPREDICTIONSYSTEM_H

#include "Systems/CharacterPredictionSystem.h"

#include "ServerNetworkLayer.h"

class ServerCharacterPredictionSystem : public CharacterPredictionSystem
{
    public:
        ServerCharacterPredictionSystem(fsn::EntityManager& entityMgr, ServerNetworkLayer& networkLayer);

        void processEntity(const fsn::EntityRef& entity, const float dt);
        void end(const float dt);

        // Called when a character applies input
        void onCharacterInput(const CharacterInputEvent& event);

    private:
        // The interval in ticks between each input update packet.
        int mInputUpdateInterval;

        ServerNetworkLayer& mNetworkLayer;

        std::vector<CharacterInputEvent> mInputEventsToSend;

        std::size_t mTicksBeforeInputUpdate;
};

#endif // SERVERCHARACTERPREDICTIONSYSTEM_H
