#ifndef CLIENTCHARACTERPREDICTIONSYSTEM_H
#define CLIENTCHARACTERPREDICTIONSYSTEM_H

#include "Systems/CharacterPredictionSystem.h"

class ClientCharacterPredictionSystem : public CharacterPredictionSystem
{
    public:
        ClientCharacterPredictionSystem(fsn::EntityManager& entityMgr);

        void processEntity(const fsn::EntityRef& entity, const float dt);

        // Called when the local player spawns
        void onLocalPlayerSpawned(const LocalPlayerSpawnedEvent& event);

        // Called when a character applies input
        void onCharacterInput(const CharacterInputEvent& event);

    private:
        // The client's local player
        fsn::EntityRef mLocalPlayer;

        // Predicted and authoritative states for local player. These will be useful later to
        // interpolate between the predicted state and the corrected state to avoid jerkiness.
        CharacterState mPredictedState;
        CharacterState mAuthoritativeState;
};

#endif // CLIENTCHARACTERPREDICTIONSYSTEM_H
