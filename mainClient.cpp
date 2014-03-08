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
#include "Components/Network.h"

#include "Systems/PlayerInputSystem.h"
#include "Systems/ClientCharacterPredictionSystem.h"

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
    fsn::ComponentTypeManager::add<Network>();

    // Setup the engine, render manager, and fake connection.
    fsn::Engine engine(1.f/60.f);
    fsn::EntityManager& entityMgr = engine.getEntityManager();
    fsn::EventManager& eventMgr = engine.getEventManager();

    fsn::RenderManager renderMgr(800, 600, "Monster Hunter", 5, fsn::ResourceManager::get()->getFont("Content/Fonts/font.ttf"));
    fsn::Connection conn(eventMgr);
    gui::GUIManager guiMgr(renderMgr.getWindow().getSize().x, renderMgr.getWindow().getSize().y);

    renderMgr.getWindow().setKeyRepeatEnabled(false);

    renderMgr.addOverlay(&guiMgr);

    // Setup our systems - only the input systems for now
    fsn::SpriteRenderSystem spriteSys(entityMgr, &renderMgr);
    fsn::InputSystem inputSys(&renderMgr.getWindow());
    PlayerInputSystem playerInputSys(eventMgr);
    ClientCharacterPredictionSystem clientMoveSys(entityMgr);

    engine.addSystem(spriteSys);
    engine.addSystem(inputSys);
    engine.addSystem(playerInputSys);
    engine.addSystem(clientMoveSys);

    // Game specific manager type stuff
    ClientNetworkLayer networkLayer(eventMgr, entityMgr, conn);

    // Register network handlers
    conn.registerHandler(0, &networkLayer);

    // Input listeners
    //inputSys.addKeyboardListener(&guiMgr);
    inputSys.addMouseListener(&guiMgr);
    inputSys.addKeyboardListener(&playerInputSys);

    // Register everything with the EventManager
    eventMgr.addListener(&PlayerInputSystem::onLocalPlayerSpawned, playerInputSys);

    eventMgr.addListener(&ClientCharacterPredictionSystem::onLocalPlayerSpawned, clientMoveSys);
    eventMgr.addListener(&ClientCharacterPredictionSystem::onCharacterInput, clientMoveSys);
    eventMgr.addListener(&ClientCharacterPredictionSystem::onCharacterState, clientMoveSys);

    eventMgr.addListener(&ClientNetworkLayer::onCharacterInput, networkLayer);

    // Make a window
    gui::Window window(128, 100);
    window.setPosition(sf::Vector2i(100, 200));
    guiMgr.getRoot().addChild(&window);

    gui::Button button;
    button.setPosition(sf::Vector2i(10, 10));
    window.addChild(&button);

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
