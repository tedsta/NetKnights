#ifndef STAMINAREPLENISHSYSTEM_H
#define STAMINAREPLENISHSYSTEM_H

#include <Fission/Core/Systems/ComponentSystem.h>

class StaminaReplenishSystem : public fsn::ComponentSystem
{
    public:
        StaminaReplenishSystem(fsn::EntityManager& entityMgr);

        void processEntity(const fsn::EntityRef& entity, const float dt);

    private:
        std::size_t mCoolDown;
};

#endif // STAMINAREPLENISHSYSTEM_H
