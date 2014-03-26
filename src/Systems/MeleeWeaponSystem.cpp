#include "Systems/MeleeWeaponSystem.h"

#include <Fission/Rendering/Transform.h>

#include "Components/MeleeWeapon.h"
#include "Components/HitBox.h"
#include "Components/HitPoints.h"
#include "Components/Shield.h"
#include "Components/Stamina.h"

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

    if (weap.attack && weap.attackDurationLeft > 0)
    {
        auto& hittables = mHittableEntities.getActiveEntities();

        for (auto& hittable : hittables)
        {
            if (hittable == entity)
                continue;

            auto& hTransform = hittable.getComponent<fsn::Transform>();
            auto& hitBox = hittable.getComponent<HitBox>();
            auto& hp = hittable.getComponent<HitPoints>();

            // Can't touch this
            if (!hp.canDamage(entity))
                continue;

            sf::FloatRect hittableArea = hitBox.rect;
            hittableArea.left += hTransform.getPosition().x;
            hittableArea.top += hTransform.getPosition().y;

            sf::FloatRect weapArea = weap.hitBox;
            weapArea.left += weapTransform.getPosition().x;
            weapArea.top += weapTransform.getPosition().y;

            if (hittableArea.intersects(weapArea))
            {
                if (hittable.hasComponent<Shield>() && hittable.hasComponent<Stamina>())
                {
                    auto& shield = hittable.getComponent<Shield>();
                    auto& stamina = hittable.getComponent<Stamina>();

                    if (shield.guard && stamina.takeStamina(25))
                    {
                        // Block the attack
                        hp.addInvulnerability(20, entity);
                        continue;
                    }
                }

                hp.damage(weap.damage, 20, entity);
            }
        }

        weap.attackDurationLeft--;
    }
    else
    {
        weap.attackDurationLeft = weap.attackDuration;
        weap.attack = false;
    }
}
