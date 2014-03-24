#include "Systems/ServerCharacterPredictionSystem.h"

ServerCharacterPredictionSystem::ServerCharacterPredictionSystem(fsn::EntityManager& entityMgr, ServerNetworkLayer& networkLayer) :
    CharacterPredictionSystem(entityMgr), mInputUpdateInterval(12), mNetworkLayer(networkLayer), mTicksBeforeInputUpdate(mInputUpdateInterval)
{
    //ctor
}

void ServerCharacterPredictionSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    stepNonLocalCharacter(entity);
}

void ServerCharacterPredictionSystem::end(const float dt)
{
    CharacterPredictionSystem::end(dt);

    // Getting closer to the next update
    mTicksBeforeInputUpdate--;

    // If there's no more ticks before the next update... it's time to update!
    if (mTicksBeforeInputUpdate == 0)
    {
        // Reset the update timer
        mTicksBeforeInputUpdate = mInputUpdateInterval;

        // Send the updates
        mNetworkLayer.sendCharacterInputs(mInputEventsToSend);

        // Clear the inputs to send after we've sent them
        mInputEventsToSend.clear();
    }
}

void ServerCharacterPredictionSystem::onCharacterInput(const CharacterInputEvent& event)
{
    auto& inputs = mInputs[event.characterEntity.getID()];

    // This is old input, discard
    if (!inputs.empty() && event.input.sequence <= inputs.back().input.sequence)
    {
        return;
    }

    mInputEventsToSend.push_back(event);

    addNonLocalInput(inputs, event.input);
}
