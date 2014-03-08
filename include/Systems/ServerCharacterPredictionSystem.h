#ifndef SERVERMOVEMENTPREDICTIONSYSTEM_H
#define SERVERMOVEMENTPREDICTIONSYSTEM_H

#include <array>
#include <unordered_map>
#include <vector>

#include <Fission/Core/Systems/ComponentSystem.h>

#include "CharacterMover.h"
#include "Events.h"
#include "ServerNetworkLayer.h"

class ServerCharacterPredictionSystem : public fsn::ComponentSystem
{
    static const std::size_t StepDelay = 10; // Character step delay in ticks
    static const std::size_t InputUpdateInterval = 12;

    public:
        ServerCharacterPredictionSystem(fsn::EntityManager& entityMgr, ServerNetworkLayer& networkLayer);

        void processEntity(const fsn::EntityRef& entity, const float dt);
        void end(const float dt);

        void onEntityAdded(const fsn::EntityRef& entity);
        void onEntityRemoved(const fsn::EntityRef& entity);

        void onCharacterInput(const CharacterInputEvent& event);

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

        ServerNetworkLayer& mNetworkLayer;

        std::vector<CharacterInputEvent> mInputEventsToSend;
        std::unordered_map<int, CharacterStateList> mStates;
        std::unordered_map<int, CharacterInputList> mInputs;

        std::size_t mTick;
        std::size_t mTicksBeforeInputUpdate;
};

#endif // SERVERMOVEMENTPREDICTIONSYSTEM_H
