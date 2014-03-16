#ifndef MELEEWEAPONSYSTEM_H
#define MELEEWEAPONSYSTEM_H

#include <Fission/Core/Systems/ComponentSystem.h>
#include <Fission/Core/FilteredEntityBucket.h>

class MeleeWeaponSystem : public fsn::ComponentSystem
{
    public:
        MeleeWeaponSystem(fsn::EntityManager& entityMgr);

        void processEntity(const fsn::EntityRef& entity, const float dt);

    private:
        fsn::EntityManager& mEntityManager;

        fsn::FilteredEntityBucket mHittableEntities;
};

#endif // MELEEWEAPONSYSTEM_H
