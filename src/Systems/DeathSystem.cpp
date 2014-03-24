#include "Systems/DeathSystem.h"

#include "Components/Character.h"
#include "Components/CharacterAnimation.h"
#include "Components/HitPoints.h"
#include "Components/MeleeWeapon.h"

#include "AnimationEnums.h"
#include "Events.h"

DeathSystem::DeathSystem(fsn::EntityManager& entityMgr, fsn::EventManager& eventMgr) : fsn::ComponentSystem(entityMgr),
    mEventManager(eventMgr)
{
    all<Character, CharacterAnimation, HitPoints>();
}

void DeathSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    auto& hp = entity.getComponent<HitPoints>();

    if (hp.getHP() <= 0)
    {
        // Remove alive character part of entity
        entity.removeComponent<Character>();
        entity.removeComponent<HitPoints>();
        entity.removeComponent<MeleeWeapon>();

        // Apply death animation
        auto& anim = entity.getComponent<CharacterAnimation>();
        anim.setAnimation(KnightAnimation::Die, false);

        mEventManager.fireEvent(CharacterDeathEvent(entity));
    }
}
