#ifndef CHARACTERPREDICTIONSYSTEM_H
#define CHARACTERPREDICTIONSYSTEM_H

#include <array>
#include <unordered_map>
#include <vector>

#include <Fission/Core/Systems/ComponentSystem.h>

#include "CharacterMover.h"
#include "Events.h"

class CharacterPredictionSystem : public fsn::ComponentSystem
{
    protected:
        // Represents character input at a specific game tick
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

    public:
        CharacterPredictionSystem(fsn::EntityManager& entityMgr);

        void end(const float dt);

        void onEntityAdded(const fsn::EntityRef& entity);
        void onEntityRemoved(const fsn::EntityRef& entity);

        // Step a non-local character
        void stepNonLocalCharacter(const fsn::EntityRef& entity);

        // Add a non-local input
        void addNonLocalInput(CharacterInputList& inputs, const CharacterInput& input);

    // Protected so our derived server and client character prediction systems can access all of the things
    protected:
        void setNewState(const fsn::EntityRef& entity, const CharacterState& state);

        // The number of ticks to delay all input processing by. This leaves some wiggle room for
        // latency, reducing the chance of oversimulating an input.
        int mStepDelay;

        std::unordered_map<int, CharacterStateList> mStates;
        std::unordered_map<int, CharacterInputList> mInputs;

        std::size_t mTick;
};

#endif // CHARACTERPREDICTIONSYSTEM_H
