#include "Systems/ProjectileWeaponSystem.h"

#include <Fission/Rendering/Transform.h>
#include <Fission/Rendering/Sprite.h>

#include "Components/Projectile.h"
#include "Components/ProjectileWeapon.h"
#include "Components/Velocity.h"

ProjectileWeaponSystem::ProjectileWeaponSystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr),
    mEntityManager(entityMgr)
{
    mAspect.all<ProjectileWeapon>();
}

void ProjectileWeaponSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    auto& weap = entity.getComponent<ProjectileWeapon>();

    if (weap.attemptFire && weap.coolDownLeft <= 0)
    {
        sf::Vector2f dir = weap.direction*weap.fireSpeed;

        auto proj = mEntityManager.createEntityRef(mEntityManager.createEntity());
        proj.addComponent<fsn::Transform>(entity.getComponent<fsn::Transform>().getPosition());
        proj.addComponent<fsn::Sprite>("Content/Textures/bullet.png");
        proj.addComponent<Velocity>(dir.x, dir.y);
        proj.addComponent<Projectile>();

        weap.coolDownLeft = weap.coolDown;
    }
    else
    {
        weap.coolDownLeft -= dt;
    }
}
