#ifndef MONSTER_HUNTER_EVENTS_H_INCLUDED
#define MONSTER_HUNTER_EVENTS_H_INCLUDED

#include <Fission/Core/EntityRef.h>
#include <Fission/Core/EventManager.h>

#include "CharacterMover.h"

// Event for when the local player spawns
struct LocalPlayerSpawnedEvent : public fsn::EventData
{
    LocalPlayerSpawnedEvent(const fsn::EntityRef& playerEntity) :
        playerEntity(playerEntity)
    {
    }

    fsn::EntityRef playerEntity;
};

// Event for when non-local characters spawn
struct CharacterSpawnedEvent : public fsn::EventData
{
    CharacterSpawnedEvent(const fsn::EntityRef& characterEntity) :
        characterEntity(characterEntity)
    {
    }

    fsn::EntityRef characterEntity;

};

// Character input change event
struct CharacterInputEvent : public fsn::EventData
{
    CharacterInputEvent(const fsn::EntityRef& characterEntity, const CharacterInput& input) :
        characterEntity(characterEntity), input(input)
    {
    }

    fsn::EntityRef characterEntity;
    CharacterInput input;
};

// Character state change event
struct CharacterStateEvent : public fsn::EventData
{
    CharacterStateEvent(const fsn::EntityRef& characterEntity, const CharacterState& state) :
        characterEntity(characterEntity), state(state)
    {
    }

    fsn::EntityRef characterEntity;
    CharacterState state;
};

#endif // MONSTER_HUNTER_EVENTS_H_INCLUDED
