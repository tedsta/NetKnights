#ifndef VELOCITYSYSTEM_H
#define VELOCITYSYSTEM_H

#include <Fission/Core/Systems/ComponentSystem.h>

class VelocitySystem : public fsn::ComponentSystem
{
    public:
        VelocitySystem(fsn::EntityManager& entityMgr);

        void processEntity(const fsn::EntityRef& entity, const float dt);

    private:
};

#endif // VELOCITYSYSTEM_H
