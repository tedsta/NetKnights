#include "Systems/ServerCharacterPredictionSystem.h"

#include <Fission/Rendering/Transform.h>
#include <Fission/Rendering/Sprite.h>

#include "Components/CharacterAnimation.h"
#include "Components/Character.h"
#include "Components/ProjectileWeapon.h"
#include "Components/MeleeWeapon.h"

ServerCharacterPredictionSystem::ServerCharacterPredictionSystem(fsn::EntityManager& entityMgr, ServerNetworkLayer& networkLayer) :
    fsn::ComponentSystem(entityMgr), mNetworkLayer(networkLayer), mTick(0), mTicksBeforeInputUpdate(InputUpdateInterval)
{
    //ctor
}

void ServerCharacterPredictionSystem::processEntity(const fsn::EntityRef& entity, const float dt)
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

void ServerCharacterPredictionSystem::end(const float dt)
{
    mTick++;
    mTicksBeforeInputUpdate--;

    if (mTicksBeforeInputUpdate == 0)
    {
        mTicksBeforeInputUpdate = InputUpdateInterval;
        mNetworkLayer.sendCharacterInputs(mInputEventsToSend);
        mInputEventsToSend.clear();
    }
}

void ServerCharacterPredictionSystem::onEntityAdded(const fsn::EntityRef& entity)
{
    mStates[entity.getID()] = CharacterStateList();
    mInputs[entity.getID()] = CharacterInputList();
}

void ServerCharacterPredictionSystem::onEntityRemoved(const fsn::EntityRef& entity)
{
    mStates.erase(entity.getID());
    mInputs.erase(entity.getID());
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

    if (!inputs.empty())
    {
        inputs.back().receivedLimit = true;

        if (inputs.back().ticksStepped < event.input.ticksSinceLastInput)
        {
            inputs.back().ticksLeft = event.input.ticksSinceLastInput-inputs.back().ticksStepped;
        }
        else
        {
            inputs.back().ticksLeft = 0;
        }
    }
    inputs.push_back(CharacterInputAt{event.input, mTick});
}

void ServerCharacterPredictionSystem::setNewState(const fsn::EntityRef& entity, const CharacterState& state)
{
    CharacterStateList& states = mStates[entity.getID()];

    for (auto state = states.begin(); state+1 != states.end(); state++)
    {
        *state = *(state+1);
    }

    states.back() = state;

    // Put the entity at the new state
    auto& transform = entity.getComponent<fsn::Transform>();
    auto& sprite = entity.getComponent<fsn::Sprite>();
    auto& charAnim = entity.getComponent<CharacterAnimation>();
    auto& weap = entity.getComponent<MeleeWeapon>();

    transform.setPosition(state.position);

    if (state.up)
    {
        weap.direction.y = -1.f;
    }
    else if (state.down)
    {
        weap.direction.y = 1.f;
    }
    else
    {
        weap.direction.y = 0.f;
    }

    if (state.left)
    {
        transform.setScale(sf::Vector2f(-2.f, 2.f));
        weap.direction.x = -1.f;
    }
    else if (state.right)
    {
        transform.setScale(sf::Vector2f(2.f, 2.f));
        weap.direction.x = 1.f;
    }
    else
    {
        weap.direction.x = 0.f;
    }

    weap.attemptAttack = state.firing;

    if (state.moving)
    {
        charAnim.setAnimation("walk", true);
    }
    else
    {
        charAnim.setAnimation("idle", true);
    }
}
