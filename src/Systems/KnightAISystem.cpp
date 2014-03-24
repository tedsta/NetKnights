#include "Systems/KnightAISystem.h"

#include <Fission/Util/math.h>

#include <Fission/Rendering/Transform.h>

#include "Events.h"

#include "Components/Team.h"
#include "Components/KnightAI.h"

KnightAISystem::KnightAISystem(fsn::EntityManager& entityMgr, fsn::EventManager& eventMgr) : fsn::ComponentSystem(entityMgr),
    mEventManager(eventMgr), mTick(0)
{
    all<fsn::Transform, KnightAI, Team>();

    mTargetables.all<Team>();
    entityMgr.addEntityObserver(&mTargetables);
}

void KnightAISystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    //this knight
    auto& ai = entity.getComponent<KnightAI>();
    auto& transform = entity.getComponent<fsn::Transform>();

    CharacterInput oldInput = ai.mInput;

    //no enemy - find new one
    if (ai.target.getID() == fsn::EntityRef::NullID)
    {
        auto& targetables = mTargetables.getActiveEntities();

        for (auto& targetable : targetables)
        {
            if (targetable == entity) //don't target yourself!
                continue;

            ai.target = targetable;
            break;
        }
    }
    else //have target - chase him
    {
        auto& targetTransform = ai.target.getComponent<fsn::Transform>();

        if (fsn::length(targetTransform.getPosition() - transform.getPosition()) <= 20) //close enough to attack
        {
            ai.mInput.attack = true;

            ai.mInput.left = false;
            ai.mInput.right = false;
            ai.mInput.up = false;
            ai.mInput.down = false;
        }
        else
        {
            ai.mInput.attack = false;

            if (targetTransform.getPosition().x - transform.getPosition().x < -5) //target is to the left
            {
                ai.mInput.left = true;
            }
            else
            {
                ai.mInput.left = false;
            }

            if (targetTransform.getPosition().x - transform.getPosition().x > 5) //to the right
            {
                ai.mInput.right = true;
            }
            else
            {
                ai.mInput.right = false;
            }

            if (targetTransform.getPosition().y - transform.getPosition().y < -5) //above
            {
                ai.mInput.up = true;
            }
            else
            {
                ai.mInput.up = false;
            }

            if (targetTransform.getPosition().y - transform.getPosition().y > 5) //below
            {
                ai.mInput.down = true;
            }
            else
            {
                ai.mInput.down = false;
            }
        }
    }

    if (!ai.mInput.sameAs(oldInput)) // Input changed
    {
        ai.mInput.sequence++;
        ai.mInput.ticksSinceLastInput = mTick-ai.mLastInputTick;
        ai.mLastInputTick = mTick;

        mEventManager.fireEvent(CharacterInputEvent(entity, ai.mInput));
    }
}

void KnightAISystem::end(const float dt)
{
    mTick++;
}
