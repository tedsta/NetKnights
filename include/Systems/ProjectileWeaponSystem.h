#ifndef PROJECTILEWEAPONSYSTEM_H
#define PROJECTILEWEAPONSYSTEM_H

#include <Fission/Core/Systems/ComponentSystem.h>

class ProjectileWeaponSystem : public fsn::ComponentSystem
{
    public:
        ProjectileWeaponSystem(fsn::EntityManager& entityMgr);

        void processEntity(const fsn::EntityRef& entity, const float dt);

    private:
        fsn::EntityManager& mEntityManager;
};

#endif // PROJECTILEWEAPONSYSTEM_H
