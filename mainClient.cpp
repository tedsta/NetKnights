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
#include "Components/MeleeWeapon.h"
#include "Components/Stamina.h"

#include "Systems/CharacterAnimationSystem.h"
#include "Systems/ClientCharacterPredictionSystem.h"
#include "Systems/HPInvulnerabilitySystem.h"
#include "Systems/MeleeWeaponSystem.h"
#include "Systems/PlayerInputSystem.h"

#include "Events.h"

#include "ClientNetworkLayer.h"

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
    fsn::ComponentTypeManager::add<MeleeWeapon>();
    fsn::ComponentTypeManager::add<Stamina>();

    // Setup the engine, render manager, and fake connection.
    fsn::Engine engine(1.f/60.f);
    fsn::EntityManager& entityMgr = engine.getEntityManager();
    fsn::EventManager& eventMgr = engine.getEventManager();

    fsn::RenderManager renderMgr(800, 600, "Monster Hunter", 5, fsn::ResourceManager::get()->getFont("Content/Fonts/font.ttf"));
    fsn::Connection conn(eventMgr);

    renderMgr.getWindow().setKeyRepeatEnabled(false);

    // Setup our systems - only the input systems for now
    fsn::SpriteRenderSystem spriteSys(entityMgr, &renderMgr);
    fsn::InputSystem inputSys(&renderMgr.getWindow());
    PlayerInputSystem playerInputSys(eventMgr);
    ClientCharacterPredictionSystem charPredictSys(entityMgr);
    CharacterAnimationSystem charAnimSys(entityMgr);
    MeleeWeaponSystem meleeWeapSys(entityMgr);
    HPInvulnerabilitySystem hpInvulSys(entityMgr);

    engine.addSystem(spriteSys);
    engine.addSystem(inputSys);
    engine.addSystem(playerInputSys);
    engine.addSystem(charPredictSys);
    engine.addSystem(charAnimSys);
    engine.addSystem(meleeWeapSys);
    engine.addSystem(hpInvulSys);

    // Game specific manager type stuff
    ClientNetworkLayer networkLayer(eventMgr, entityMgr, conn);

    // Register network handlers
    conn.registerHandler(0, &networkLayer);

    // Input listeners
    inputSys.addKeyboardListener(&playerInputSys);

    // Register everything with the EventManager
    eventMgr.addListener(&PlayerInputSystem::onLocalPlayerSpawned, playerInputSys);

    eventMgr.addListener(&ClientCharacterPredictionSystem::onLocalPlayerSpawned, charPredictSys);
    eventMgr.addListener(&ClientCharacterPredictionSystem::onCharacterInput, charPredictSys);

    eventMgr.addListener(&ClientNetworkLayer::onCharacterInput, networkLayer);

    // Connect
    conn.connectClient("Teddy-PC", 54300);
    networkLayer.login("player", "asdf");

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
