#include "PlayerManager.h"

#include <Fission/Rendering/Transform.h>
#include <Fission/Rendering/Sprite.h>

#include "EntityTags.h"

#include "Components/Character.h"
#include "Components/CharacterAnimation.h"
#include "Components/HitBox.h"
#include "Components/HitPoints.h"
#include "Components/MeleeWeapon.h"
#include "Components/ProjectileWeapon.h"

PlayerManager::PlayerManager(fsn::EntityManager& entityMgr) : mEntityManager(entityMgr)
{
    //ctor
}

bool PlayerManager::login(const std::string& name, const std::string& pass, int netID)
{
    Player* player = findPlayerByName(name);

    // Error - player doesn't exist
    if (!player)
        return false;

    // Incorrected password
    if (player->pass != pass)
        return false;

    player->netID = netID;

    return true;
}

const fsn::EntityRef& PlayerManager::spawnPlayer(int netID)
{
    auto entity = mEntityManager.createEntityRef(mEntityManager.createEntity(true));
    entity.addComponent<fsn::Transform>();
    entity.addComponent<fsn::Sprite>("Content/Textures/Characters/microknight.png", 40, 20);
    entity.addComponent<Character>();
    entity.addComponent<CharacterAnimation>();
    entity.addComponent<HitBox>();
    entity.addComponent<HitPoints>();
    entity.addComponent<MeleeWeapon>();

    entity.setTag(etags::Player);
    entity.getComponent<fsn::Transform>().setScale(2.f, 2.f);
    entity.getComponent<fsn::Sprite>().setFrameLoop(4, 6);

    auto& anim = entity.getComponent<CharacterAnimation>();
    anim.addAnimation("idle", FrameLoop(1, 1));
    anim.addAnimation("walk", FrameLoop(4, 6));

    mPlayers.push_back(Player{mNextID++, netID, "Player", "password", entity});

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
