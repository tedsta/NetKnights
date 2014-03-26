#include "Systems/StaminaReplenishSystem.h"

#include "Components/Stamina.h"

StaminaReplenishSystem::StaminaReplenishSystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr),
    mCoolDown(0)
{
    all<Stamina>();
}

void StaminaReplenishSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    auto& stamina = entity.getComponent<Stamina>();

    if (mCoolDown == 0)
    {
        mCoolDown = 6; // 10 stamina per second

        stamina.addStamina(1);
    }
    else
    {
        mCoolDown--;
    }
}
