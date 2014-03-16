#ifndef CHARACTERANIMATIONSYSTEM_H
#define CHARACTERANIMATIONSYSTEM_H

#include <Fission/Core/Systems/ComponentSystem.h>

class CharacterAnimationSystem : public fsn::ComponentSystem
{
    public:
        CharacterAnimationSystem(fsn::EntityManager& entityMgr);

        void processEntity(const fsn::EntityRef& entity, const float dt);

    private:
};

#endif // CHARACTERANIMATIONSYSTEM_H
