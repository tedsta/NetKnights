#include "Systems/VelocitySystem.h"

#include <Fission/Rendering/Transform.h>

#include "Components/Velocity.h"

VelocitySystem::VelocitySystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr)
{
    mAspect.all<fsn::Transform, Velocity>();
}

void VelocitySystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    auto& transform = entity.getComponent<fsn::Transform>();
    auto& vel = entity.getComponent<Velocity>();

    transform.move(vel.x*dt, vel.y*dt);
}
