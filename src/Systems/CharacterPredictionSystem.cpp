#include "Systems/CharacterPredictionSystem.h"

#include <iostream>

#include <Fission/Rendering/Transform.h>

#include "AnimationEnums.h"

#include "Components/Character.h"
#include "Components/CharacterAnimation.h"
#include "Components/MeleeWeapon.h"
#include "Components/Stamina.h"

CharacterPredictionSystem::CharacterPredictionSystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr),
    mStepDelay(10), mTick(0)
{
   all<fsn::Transform, Character>();
}

void CharacterPredictionSystem::end(const float dt)
{
    mTick++;
}

void CharacterPredictionSystem::onEntityAdded(const fsn::EntityRef& entity)
{
    // Initialize the states and inputs lists for this character
    mStates[entity.getID()] = CharacterStateList();
    mInputs[entity.getID()] = CharacterInputList();

    // Populate the state list with the initial entries
    for (auto& state : mStates[entity.getID()])
    {
        state.stamina = entity.getComponent<Stamina>().stamina;
        state.position = entity.getComponent<fsn::Transform>().getPosition();
        state.sequence = 0;
    }
}

void CharacterPredictionSystem::onEntityRemoved(const fsn::EntityRef& entity)
{
    mStates.erase(entity.getID());
    mInputs.erase(entity.getID());
}

void CharacterPredictionSystem::stepNonLocalCharacter(const fsn::EntityRef& entity)
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

        if ((input.receivedLimit && input.ticksLeft > 0) || input.tick <= mTick-mStepDelay)
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

void CharacterPredictionSystem::addNonLocalInput(CharacterInputList& inputs, const CharacterInput& input)
{
    // Which tick this input occurred at
    std::size_t tick;

    if (!inputs.empty())
    {
        // If there's recent input, this input occurred at the last inputs' tick + duration
        tick = inputs.back().tick + input.ticksSinceLastInput;

        inputs.back().receivedLimit = true;

        if (inputs.back().ticksStepped < input.ticksSinceLastInput)
        {
            inputs.back().ticksLeft = input.ticksSinceLastInput-inputs.back().ticksStepped;
            std::cout << "Need to simulate " << inputs.back().ticksLeft << " more ticks totalling "
                      << inputs.back().ticksStepped+inputs.back().ticksLeft << std::endl;
        }
        else if (inputs.back().ticksStepped > input.ticksSinceLastInput)
        {
            std::cout << "Overshot input by " << inputs.back().ticksStepped-input.ticksSinceLastInput << " ticks\n";
            inputs.back().ticksLeft = 0;
        }
    }
    else
        tick = mTick; // No input to reference, it occurred at current tick

    inputs.push_back(CharacterInputAt(input, tick));
}

void CharacterPredictionSystem::setNewState(const fsn::EntityRef& entity, const CharacterState& state)
{
    CharacterStateList& states = mStates[entity.getID()];

    // Move all states in the list down one
    for (auto state = states.begin(); state+1 != states.end(); state++)
    {
        *state = *(state+1);
    }
    states.back() = state; // Place the new state at the top of the list

    // Put the entity at the new state
    auto& transform = entity.getComponent<fsn::Transform>();
    auto& anim = entity.getComponent<CharacterAnimation>();
    auto& weap = entity.getComponent<MeleeWeapon>();
    auto& stamina = entity.getComponent<Stamina>();

    stamina.stamina = state.stamina;

    transform.setPosition(state.position);

    weap.attack = false;

    if (state.direction == Direction::Left)
    {
        transform.setScale(-2.f, 2.f);
    }
    else if (state.direction == Direction::Right)
    {
        transform.setScale(2.f, 2.f);
    }

    if (state.moveState == MoveState::Idle)
    {
        anim.setAnimation(KnightAnimation::Idle);
    }
    else if (state.moveState == MoveState::Walking)
    {
        anim.setAnimation(KnightAnimation::Walk);
    }
    else if (state.moveState == MoveState::Guarding)
    {
        anim.setAnimation(KnightAnimation::Guard);
    }
    else if (state.moveState == MoveState::Attacking || state.moveState == MoveState::DashingAttacking)
    {
        weap.attack = true;
        anim.setAnimation(KnightAnimation::Attack, false);
    }
}
