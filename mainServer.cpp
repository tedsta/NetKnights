#include <sstream>

#include <Fission/Core/Engine.h>
#include <Fission/Core/EntityManager.h>
#include <Fission/Core/ResourceManager.h>
#include <Fission/Core/ComponentTypeManager.h>

#include <Fission/Rendering/RenderManager.h>
#include <Fission/Rendering/Transform.h>
#include <Fission/Rendering/Sprite.h>
#include <Fission/Rendering/SpriteRenderSystem.h>

#include <Fission/Input/InputSystem.h>

#include <Fission/Network/Connection.h>

#include "Components/Character.h"
#include "Components/CharacterAnimation.h"
#include "Components/HitBox.h"
#include "Components/HitPoints.h"
#include "Components/KnightAI.h"
#include "Components/MeleeWeapon.h"
#include "Components/Shield.h"
#include "Components/Stamina.h"
#include "Components/Team.h"

#include "Systems/CharacterAnimationSystem.h"
#include "Systems/DeathSystem.h"
#include "Systems/HPInvulnerabilitySystem.h"
#include "Systems/KnightAISystem.h"
#include "Systems/MeleeWeaponSystem.h"
#include "Systems/ServerCharacterPredictionSystem.h"
#include "Systems/StaminaReplenishSystem.h"

#include "Events.h"

#include "PlayerManager.h"
#include "ServerNetworkLayer.h"

#include "AnimationEnums.h"
#include "EntityTags.h"

int main()
{
    // Initialize the resource manager.
    fsn::ResourceManager::init();

    // Register the components with the component type manager
    fsn::ComponentTypeManager::add<fsn::Transform>();
    fsn::ComponentTypeManager::add<fsn::Sprite>();

    fsn::ComponentTypeManager::add<Character>();
    fsn::ComponentTypeManager::add<CharacterAnimation>();
    fsn::ComponentTypeManager::add<HitBox>();
    fsn::ComponentTypeManager::add<HitPoints>();
    fsn::ComponentTypeManager::add<KnightAI>();
    fsn::ComponentTypeManager::add<MeleeWeapon>();
    fsn::ComponentTypeManager::add<Shield>();
    fsn::ComponentTypeManager::add<Stamina>();
    fsn::ComponentTypeManager::add<Team>();

    // Setup the engine, render manager, and fake connection.
    fsn::Engine engine(1.f/60.f);
    fsn::EntityManager& entityMgr = engine.getEntityManager();
    fsn::EventManager& eventMgr = engine.getEventManager();

    fsn::RenderManager renderMgr(800, 600, "Monster Hunter", 5, fsn::ResourceManager::get()->getFont("Content/Fonts/font.ttf"));
    fsn::Connection conn(eventMgr);

    // Game specific manager type stuff
    PlayerManager playerMgr(entityMgr);
    ServerNetworkLayer networkLayer(eventMgr, entityMgr, conn, playerMgr);

    // Settings
    renderMgr.getWindow().setKeyRepeatEnabled(false);

    // Setup our systems - only the input systems for now
    fsn::SpriteRenderSystem spriteSys(entityMgr, &renderMgr);
    fsn::InputSystem inputSys(&renderMgr.getWindow());
    ServerCharacterPredictionSystem charPredictSys(entityMgr, networkLayer);
    CharacterAnimationSystem charAnimSys(entityMgr);
    MeleeWeaponSystem meleeWeapSys(entityMgr);
    DeathSystem deathSys(entityMgr, eventMgr);
    HPInvulnerabilitySystem hpInvulSys(entityMgr);
    KnightAISystem knightAISys(entityMgr, eventMgr);
    StaminaReplenishSystem staminaRepSys(entityMgr);

    engine.addSystem(spriteSys);
    engine.addSystem(inputSys);
    engine.addSystem(charPredictSys);
    engine.addSystem(charAnimSys);
    engine.addSystem(meleeWeapSys);
    engine.addSystem(deathSys);
    engine.addSystem(hpInvulSys);
    engine.addSystem(knightAISys);
    engine.addSystem(staminaRepSys);

    // Register everything with the EventManager
    eventMgr.addListener(&ServerCharacterPredictionSystem::onCharacterInput, charPredictSys);

    eventMgr.addListener(&ServerNetworkLayer::onCharacterDeath, networkLayer);

    // Register network handlers
    conn.registerHandler(0, &networkLayer);

    // Finally begin hosting the server
    conn.hostServer(54300);

    //make test ai knight
    auto entity = entityMgr.createEntityRef(entityMgr.createEntity(true));
    entity.addComponent<fsn::Transform>(sf::Vector2f(500, 300));
    entity.addComponent<fsn::Sprite>("Content/Textures/Characters/microknight.png", 40, 20);
    entity.addComponent<CharacterAnimation>();
    entity.addComponent<Character>();
    entity.addComponent<HitBox>();
    entity.addComponent<HitPoints>(100);
    entity.addComponent<KnightAI>();
    entity.addComponent<MeleeWeapon>();
    entity.addComponent<Shield>();
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

    // Run the main loop.
    sf::Clock clock;
    float accum = 0.f;
    int frames = 0;
    while (renderMgr.getWindow().isOpen())
    {
        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        accum += dt;
        if (accum >= 1.f)
        {
            std::stringstream fps;
            std::string sFps;
            fps << frames;
            fps >> sFps;
            renderMgr.getDebugDisplay().setMessage("FPS", sFps);
            accum = 0;
            frames = 0;
        }
        frames++;

        conn.update();
        engine.update(dt);
        renderMgr.render();
    }

    return 0;
}
