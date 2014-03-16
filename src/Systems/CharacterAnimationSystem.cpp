#include "Systems/CharacterAnimationSystem.h"

#include <Fission/Rendering/Sprite.h>

#include "Components/CharacterAnimation.h"

CharacterAnimationSystem::CharacterAnimationSystem(fsn::EntityManager& entityMgr) : fsn::ComponentSystem(entityMgr)
{
    all<fsn::Sprite, CharacterAnimation>();
}

void CharacterAnimationSystem::processEntity(const fsn::EntityRef& entity, const float dt)
{
    auto& sprite = entity.getComponent<fsn::Sprite>();
    auto& charAnim = entity.getComponent<CharacterAnimation>();

    sprite.setFrameLoop(charAnim.getCurrentAnimation().start, charAnim.getCurrentAnimation().stop);
    sprite.setLoopAnim(charAnim.isLooping());
}
