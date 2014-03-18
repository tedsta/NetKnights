#ifndef DEATHSYSTEM_H
#define DEATHSYSTEM_H

#include <Fission/Core/Systems/ComponentSystem.h>

class DeathSystem : public fsn::ComponentSystem
{
    public:
        DeathSystem(fsn::EntityManager& entityMgr);

        void processEntity(const fsn::EntityRef& entity, const float dt);
};

#endif // DEATHSYSTEM_H
