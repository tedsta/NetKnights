#include "Systems/MeleeWeaponSystem.h"

#include <Fission/Rendering/Transform.h>

#include "Components/MeleeWeapon.h"
#include "Components/HitBox.h"
#include "Components/HitPoints.h"

MeleeWeaponSystem::MeleeWeaponSystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr),
    mEntityManager(entityMgr)
{
    all<fsn::Transform, MeleeWeapon>();

    mHittableEntities.all<fsn::Transform, HitBox, HitPoints>();
    entityMgr.addEntityObserver(&mHittableEntities);
}

void MeleeWeaponSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    auto& weapTransform = entity.getComponent<fsn::Transform>();
    auto& weap = entity.getComponent<MeleeWeapon>();

    if (weap.attemptAttack && weap.hitDelayLeft <= 0)
    {
        auto& hittables = mHittableEntities.getActiveEntities();

        for (auto& hittable : hittables)
        {
            if (hittable == entity)
                continue;

            auto& hTransform = hittable.getComponent<fsn::Transform>();
            auto& hitBox = hittable.getComponent<HitBox>();
            auto& hp = hittable.getComponent<HitPoints>();

            sf::FloatRect hittableArea = hitBox.rect;
            hittableArea.left += hTransform.getPosition().x-(hTransform.getOrigin().x*hTransform.getScale().x);
            hittableArea.top += hTransform.getPosition().y-(hTransform.getOrigin().y*hTransform.getScale().y);

            sf::FloatRect weapArea = weap.hitBox;
            weapArea.left += weapTransform.getPosition().x-(weapTransform.getOrigin().x*weapTransform.getScale().x);
            weapArea.top += weapTransform.getPosition().y-(weapTransform.getOrigin().y*weapTransform.getScale().y);

            if (hittableArea.intersects(weapArea))
            {
                hp.HP -= weap.damage;
            }
        }

        weap.hitDelayLeft = weap.hitDelay;
    }
    else
    {
        weap.hitDelayLeft -= dt;
    }
}
