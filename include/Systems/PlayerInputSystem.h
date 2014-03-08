#ifndef PLAYERINPUTSYSTEM_H
#define PLAYERINPUTSYSTEM_H

#include <Fission/Core/EntityRef.h>
#include <Fission/Core/EventManager.h>
#include <Fission/Core/Systems/System.h>
#include <Fission/Input/IKeyboardListener.h>

#include "Events.h"
#include "CharacterMover.h"

class PlayerInputSystem : public fsn::System, public fsn::IKeyboardListener
{
    public:
        PlayerInputSystem(fsn::EventManager& eventMgr);

        void update(const float dt);

        bool onKeyPressed(sf::Keyboard::Key key);
        bool onKeyReleased(sf::Keyboard::Key key);

        void onLocalPlayerSpawned(const LocalPlayerSpawnedEvent& event);

    private:
        fsn::EventManager& mEventManager;

        fsn::EntityRef mLocalPlayer;

        CharacterInput mInput; // Current input state

        std::size_t mTick;
        std::size_t mLastInputTick;
};

#endif // PLAYERINPUTSYSTEM_H
