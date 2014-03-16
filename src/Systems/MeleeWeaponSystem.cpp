#include "Systems/MeleeWeaponSystem.h"

#include <Fission/Rendering/Transform.h>

#include "Components/MeleeWeapon.h"

MeleeWeaponSystem::MeleeWeaponSystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr),
    mEntityManager(entityMgr)
{
    mAspect.all<MeleeWeapon>();
}

void MeleeWeaponSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    auto& weap = entity.getComponent<MeleeWeapon>();

    /*if (weap.attemptAttack && weap.hitDelayLeft <= 0)
    {
        sf::Vector2f dir = weap.direction*weap.fireSpeed;

        auto proj = mEntityManager.createEntityRef(mEntityManager.createEntity());
        proj.addComponent<fsn::Transform>(entity.getComponent<fsn::Transform>().getPosition());
        //proj.addComponent<HitBox>(dir.x, dir.y);
        proj.addComponent<MeleeWeapon>();

        weap.hitDelayLeft = weap.hitDelay;
    }
    else
    {
        weap.hitDelayLeft -= dt;
    }*/
}
