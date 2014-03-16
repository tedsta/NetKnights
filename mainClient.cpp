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

int main()
{
    // Initialize the resource manager.
    fsn::ResourceManager::init();

    // Register the components with the component type manager
    fsn::ComponentTypeManager::add<fsn::Transform>();
    fsn::ComponentTypeManager::add<fsn::Sprite>();

    // Setup the engine, render manager, and fake connection.
    fsn::Engine engine(1.f/60.f);
    fsn::EntityManager& entityMgr = engine.getEntityManager();
    fsn::EventManager& eventMgr = engine.getEventManager();

    fsn::RenderManager renderMgr(800, 600, "Fission Example", 1, nullptr);
    fsn::Connection conn(eventMgr);

    renderMgr.getWindow().setKeyRepeatEnabled(false);

    // Setup our systems - only the input systems for now
    fsn::SpriteRenderSystem spriteSys(entityMgr, &renderMgr);
    fsn::InputSystem inputSys(&renderMgr.getWindow());

    engine.addSystem(spriteSys);
    engine.addSystem(inputSys);;

    // Run the main loop.
    sf::Clock clock;
    while (renderMgr.getWindow().isOpen())
    {
        float dt = clock.restart().asSeconds();

        conn.update();
        engine.update(dt);
        renderMgr.render();
    }

    return 0;
}
