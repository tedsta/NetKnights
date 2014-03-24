#ifndef DEATHSYSTEM_H
#define DEATHSYSTEM_H

#include <Fission/Core/EventManager.h>
#include <Fission/Core/Systems/ComponentSystem.h>

class DeathSystem : public fsn::ComponentSystem
{
    public:
        DeathSystem(fsn::EntityManager& entityMgr, fsn::EventManager& eventMgr);

        void processEntity(const fsn::EntityRef& entity, const float dt);

    private:
        fsn::EventManager& mEventManager;
};

#endif // DEATHSYSTEM_H
