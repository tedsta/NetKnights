#include "PlayerManager.h"

#include <Fission/Rendering/Transform.h>
#include <Fission/Rendering/Sprite.h>

#include "AnimationEnums.h"
#include "EntityTags.h"

#include "Components/Character.h"
#include "Components/CharacterAnimation.h"
#include "Components/HitBox.h"
#include "Components/HitPoints.h"
#include "Components/MeleeWeapon.h"
#include "Components/Stamina.h"
#include "Components/Team.h"

PlayerManager::PlayerManager(fsn::EntityManager& entityMgr) : mEntityManager(entityMgr)
{
    //ctor
}

bool PlayerManager::login(const std::string& name, int netID)
{
    Player* player = findPlayerByName(name);

    // Error - player doesn't exist
    if (!player)
        return false;

    player->netID = netID;

    return true;
}

const fsn::EntityRef& PlayerManager::spawnPlayer(int netID)
{
    auto entity = mEntityManager.createEntityRef(mEntityManager.createEntity(true));
    entity.addComponent<fsn::Transform>();
    entity.addComponent<fsn::Sprite>("Content/Textures/Characters/microknight.png", 40, 20);
    entity.addComponent<CharacterAnimation>();
    entity.addComponent<Character>();
    entity.addComponent<HitBox>();
    entity.addComponent<HitPoints>(100);
    entity.addComponent<MeleeWeapon>();
    entity.addComponent<Stamina>();
    entity.addComponent<Team>();
    entity.setTag(etags::Player);

    auto& transform = entity.getComponent<fsn::Transform>();
    transform.setOrigin(16.f, 16.f);
    transform.setScale(2.f, 2.f);

    auto& anim = entity.getComponent<CharacterAnimation>();
    anim.addAnimation(KnightAnimation::Idle, FrameLoop(0, 0));
    anim.addAnimation(KnightAnimation::Walk, FrameLoop(4, 6));
    anim.addAnimation(KnightAnimation::Guard, FrameLoop(7, 7));
    anim.addAnimation(KnightAnimation::Attack, FrameLoop(1, 3));
    anim.addAnimation(KnightAnimation::Die, FrameLoop(18, 19));
    anim.setAnimation(KnightAnimation::Idle);

    mPlayers.push_back(Player{mNextID++, netID, "Player", entity});

    return mPlayers.back().entity;
}

Player* PlayerManager::findPlayerByID(std::size_t ID)
{
    for (auto& player : mPlayers)
    {
        if (player.ID == ID)
            return &player;
    }

    return nullptr;
}

Player* PlayerManager::findPlayerByNetID(int netID)
{
    for (auto& player : mPlayers)
    {
        if (player.netID == netID)
            return &player;
    }

    return nullptr;
}

Player* PlayerManager::findPlayerByName(const std::string& name)
{
    for (auto& player : mPlayers)
    {
        if (player.name == name)
            return &player;
    }

    return nullptr;
}
