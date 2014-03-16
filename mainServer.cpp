#include <sstream>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

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

#include "GUI/GUIManager.h"
#include "GUI/Widgets/Button.h"
#include "GUI/Widgets/RenderArea.h"
#include "GUI/Widgets/Window.h"

#include "Components/Character.h"
#include "Components/CharacterAnimation.h"
#include "Components/HitBox.h"
#include "Components/HitPoints.h"
#include "Components/MeleeWeapon.h"
#include "Components/Network.h"
#include "Components/Projectile.h"
#include "Components/ProjectileWeapon.h"
#include "Components/Velocity.h"

#include "Systems/ServerCharacterPredictionSystem.h"
#include "Systems/ProjectileWeaponSystem.h"
#include "Systems/CharacterAnimationSystem.h"
#include "Systems/MeleeWeaponSystem.h"
#include "Systems/VelocitySystem.h"

#include "Events.h"

#include "PlayerManager.h"
#include "ServerNetworkLayer.h"

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
    fsn::ComponentTypeManager::add<Network>();
    fsn::ComponentTypeManager::add<Projectile>();
    fsn::ComponentTypeManager::add<ProjectileWeapon>();
    fsn::ComponentTypeManager::add<Velocity>();

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
    ServerCharacterPredictionSystem serverMoveSys(entityMgr, networkLayer);
    CharacterAnimationSystem charAnimSys(entityMgr);
    ProjectileWeaponSystem projWeaponSys(entityMgr);
    MeleeWeaponSystem meleeWeaponSys(entityMgr);
    VelocitySystem velSys(entityMgr);

    engine.addSystem(spriteSys);
    engine.addSystem(inputSys);
    engine.addSystem(serverMoveSys);
    engine.addSystem(charAnimSys);
    engine.addSystem(projWeaponSys);
    engine.addSystem(meleeWeaponSys);
    engine.addSystem(velSys);

    // Register everything with the EventManager
    eventMgr.addListener(&ServerCharacterPredictionSystem::onCharacterInput, serverMoveSys);

    // Register network handlers
    conn.registerHandler(0, &networkLayer);

    // Finally begin hosting the server
    conn.hostServer(54300);

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
