#include "Systems/ClientCharacterPredictionSystem.h"

#include <Fission/Rendering/Transform.h>

ClientCharacterPredictionSystem::ClientCharacterPredictionSystem(fsn::EntityManager& entityMgr) : CharacterPredictionSystem(entityMgr)
{
    //ctor
}

void ClientCharacterPredictionSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    if (entity == mLocalPlayer)
    {
        // If this is the local player, simply predict the next state with our CharacterMover

        if (!mInputs[entity.getID()].empty())
        {
            // Step player's physics
            mPredictedState = CharacterMover::step(mPredictedState,
                                                   createCharacterAction(entity, mPredictedState, mInputs[entity.getID()].back().input));
            setNewState(entity, mPredictedState);
        }
    }
    else
    {
        stepNonLocalCharacter(entity);
    }
}

void ClientCharacterPredictionSystem::onLocalPlayerSpawned(const LocalPlayerSpawnedEvent& event)
{
    // Set the local player
    mLocalPlayer = event.playerEntity;

    // Predicted state position starts at the player's current position
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

    // Local player's input - no special processing needs to happen. Simply insert the input into
    // our list of inputs.
    if (event.characterEntity == mLocalPlayer)
    {
        inputs.push_back(CharacterInputAt(event.input, mTick));
    }
    else
    {
        addNonLocalInput(inputs, event.input);
    }
}
