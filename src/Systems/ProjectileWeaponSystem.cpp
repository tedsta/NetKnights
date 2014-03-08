#include "Systems/ProjectileWeaponSystem.h"

#include "Components/ProjectileWeapon.h"

ProjectileWeaponSystem::ProjectileWeaponSystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr)
{
    mAspect.all<ProjectileWeapon>();
}

void ProjectileWeaponSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    auto& weap = entity.getComponent<ProjectileWeapon>();

    if (weap.attemptFire && weap.coolDownLeft <= 0)
    {
        std::cout << "fire\n";
        weap.coolDownLeft = weap.coolDown;
    }
    else
    {
        weap.coolDownLeft -= dt;
    }
}
