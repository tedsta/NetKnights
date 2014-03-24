#ifndef KNIGHTAISYSTEM_H
#define KNIGHTAISYSTEM_H

#include <Fission/Core/Systems/ComponentSystem.h>

class KnightAISystem : public fsn::ComponentSystem
{
    public:
        KnightAISystem(fsn::EntityManager& entityMgr, fsn::EventManager& eventMgr);

        void processEntity(const fsn::EntityRef& entity, const float dt);
        void end(const float dt);

    private:
        fsn::EventManager& mEventManager;

        fsn::FilteredEntityBucket mTargetables;

        std::size_t mTick;
};

#endif // KNIGHTAISYSTEM_H
