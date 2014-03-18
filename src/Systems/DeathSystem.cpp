#include "Systems/DeathSystem.h"

#include "Components/CharacterAnimation.h"
#include "Components/HitPoints.h"

DeathSystem::DeathSystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr)
{
    all<CharacterAnimation, HitPoints>();
}

void DeathSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    auto& anim = entity.getComponent<CharacterAnimation>();
    auto& hp = entity.getComponent<HitPoints>();

    if (hp.HP <= 0)
    {
        anim.setAnimation("die", false);
    }
}
