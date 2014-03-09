#include "Systems/VelocitySystem.h"

#include "Components/Velocity.h"

VelocitySystem::VelocitySystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr)
{
    mAspect.all<Velocity>();
}

void VelocitySystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
}
