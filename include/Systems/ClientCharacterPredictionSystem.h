#ifndef CLIENTMOVEMENTPREDICTIONSYSTEM_H
#define CLIENTMOVEMENTPREDICTIONSYSTEM_H

#include <array>
#include <unordered_map>
#include <vector>

#include <Fission/Core/Systems/ComponentSystem.h>

#include "CharacterMover.h"
#include "Events.h"

class ClientCharacterPredictionSystem : public fsn::ComponentSystem
{
    static const std::size_t StepDelay = 10; // Number of ticks to delay non-local characters' states by

    public:
        ClientCharacterPredictionSystem(fsn::EntityManager& entityMgr);

        void processEntity(const fsn::EntityRef& entity, const float dt);
        void end(const float dt);

        void onEntityAdded(const fsn::EntityRef& entity);
        void onEntityRemoved(const fsn::EntityRef& entity);

        // Called when the local player spawns
        void onLocalPlayerSpawned(const LocalPlayerSpawnedEvent& event);

        // Called when a character applies input
        void onCharacterInput(const CharacterInputEvent& event);

        // Called when a character state is received
        void onCharacterState(const CharacterStateEvent& event);

    private:
        void setNewState(const fsn::EntityRef& entity, const CharacterState& state);

        struct CharacterInputAt
        {
            CharacterInputAt(){}

            CharacterInputAt(const CharacterInput& input, std::size_t tick) :
                input(input), tick(tick), ticksStepped(0), receivedLimit(false), ticksLeft(0)
            {
            }

            CharacterInput input;
            std::size_t tick;
            std::size_t ticksStepped; // Number of times ticks input has been applied

            bool receivedLimit;     // If we've received the end of this input
            std::size_t ticksLeft;  // How many ticks left to apply the input
        };

        typedef std::array<CharacterState, 100> CharacterStateList;
        typedef std::vector<CharacterInputAt> CharacterInputList;

        fsn::EntityRef mLocalPlayer;

        std::unordered_map<int, CharacterStateList> mStates;
        std::unordered_map<int, CharacterInputList> mInputs;

        CharacterState mPredictedState;
        CharacterState mAuthoritativeState;

        std::size_t mTick;
};

#endif // CLIENTMOVEMENTPREDICTIONSYSTEM_H
