#include "Systems/ClientCharacterPredictionSystem.h"

#include <iostream>

#include <Fission/Rendering/Transform.h>

#include "Components/Character.h"
#include "Components/ProjectileWeapon.h"
#include "Components/MeleeWeapon.h"

ClientCharacterPredictionSystem::ClientCharacterPredictionSystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr),
    mTick(0)
{
   all<fsn::Transform, Character>();
}

void ClientCharacterPredictionSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    if (entity == mLocalPlayer)
    {
        if (!mInputs[entity.getID()].empty())
        {
            mPredictedState = CharacterMover::step(mPredictedState, mInputs[entity.getID()].back().input); // Step player's physics
            setNewState(entity, mPredictedState);
        }
    }
    else
    {
        CharacterInputList::iterator removeUpTo = mInputs[entity.getID()].end();
        for (auto it = mInputs[entity.getID()].begin(); it != mInputs[entity.getID()].end(); it++)
        {
            CharacterInputAt& input = *it;

            if (input.receivedLimit && input.ticksLeft == 0)
            {
                removeUpTo = it;
                continue;
            }

            if ((input.receivedLimit && input.ticksLeft > 0) || input.tick <= mTick-StepDelay)
            {
                auto newState = CharacterMover::step(mStates[entity.getID()].back(), input.input); // Step player's physics
                setNewState(entity, newState);

                input.ticksStepped++;
                if (input.receivedLimit)
                    input.ticksLeft--;

                break;
            }
        }

        if (!mInputs[entity.getID()].empty() && removeUpTo != mInputs[entity.getID()].end())
        {
            mInputs[entity.getID()].erase(mInputs[entity.getID()].begin(), removeUpTo+1);
        }
    }
}

void ClientCharacterPredictionSystem::end(const float dt)
{
    mTick++;
}

void ClientCharacterPredictionSystem::onEntityAdded(const fsn::EntityRef& entity)
{
    mStates[entity.getID()] = CharacterStateList();
    mInputs[entity.getID()] = CharacterInputList();

    for (auto& state : mStates[entity.getID()])
    {
        state.position = entity.getComponent<fsn::Transform>().getPosition();
        state.sequence = 0;
    }
}

void ClientCharacterPredictionSystem::onEntityRemoved(const fsn::EntityRef& entity)
{
    mStates.erase(entity.getID());
    mInputs.erase(entity.getID());
}

void ClientCharacterPredictionSystem::onLocalPlayerSpawned(const LocalPlayerSpawnedEvent& event)
{
    mLocalPlayer = event.playerEntity;
    mPredictedState.position = mLocalPlayer.getComponent<fsn::Transform>().getPosition();
}

void ClientCharacterPredictionSystem::onCharacterInput(const CharacterInputEvent& event)
{
    auto& inputs = mInputs[event.characterEntity.getID()];

    // This is old input, discard
    if (!inputs.empty() && event.input.sequence <= inputs.back().input.sequence)
    {
        return;
    }

    if (event.characterEntity == mLocalPlayer)
    {
        inputs.push_back(CharacterInputAt(event.input, mTick));
    }
    else
    {
        // Which tick this input occurred at
        std::size_t tick;

        if (!inputs.empty())
        {
            // If there's recent input, this input occurred at the last inputs' tick + duration
            tick = inputs.back().tick + event.input.ticksSinceLastInput;

            inputs.back().receivedLimit = true;

            if (inputs.back().ticksStepped < event.input.ticksSinceLastInput)
            {
                inputs.back().ticksLeft = event.input.ticksSinceLastInput-inputs.back().ticksStepped;
                std::cout << "Need to simulate " << inputs.back().ticksLeft << " more ticks totalling "
                          << inputs.back().ticksStepped+inputs.back().ticksLeft << std::endl;
            }
            else
            {
                std::cout << "Overshot input by " << inputs.back().ticksStepped-event.input.ticksSinceLastInput << " ticks\n";
                inputs.back().ticksLeft = 0;
            }
        }
        else
            tick = mTick; // No input to reference, it occurred at current tick

        inputs.push_back(CharacterInputAt(event.input, tick));
    }
}

void ClientCharacterPredictionSystem::onCharacterState(const CharacterStateEvent& event)
{
}

void ClientCharacterPredictionSystem::setNewState(const fsn::EntityRef& entity, const CharacterState& state)
{
    CharacterStateList& states = mStates[entity.getID()];

    for (auto state = states.begin(); state+1 != states.end(); state++)
    {
        *state = *(state+1);
    }

    states.back() = state;

    // Put the entity at the new state
    entity.getComponent<fsn::Transform>().setPosition(state.position);

    auto& proj = entity.getComponent<MeleeWeapon>();

    if (state.up)
    {
        proj.direction.y = -1.f;
    }
    else if (state.down)
    {
        proj.direction.y = 1.f;
    }
    else
    {
        proj.direction.y = 0.f;
    }

    if (state.left)
    {
        proj.direction.x = -1.f;
    }
    else if (state.right)
    {
        proj.direction.x = 1.f;
    }
    else
    {
        proj.direction.x = 0.f;
    }

    proj.attemptAttack = state.firing;
}
