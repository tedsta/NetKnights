#ifndef PLAYERINPUTSYSTEM_H
#define PLAYERINPUTSYSTEM_H

#include <Fission/Core/EntityRef.h>
#include <Fission/Core/EventManager.h>
#include <Fission/Core/Systems/System.h>
#include <Fission/Input/IKeyboardListener.h>

#include "Events.h"
#include "CharacterMover.h"

// PlayerInputSystem listens for input and applies creates input events for the local player.
// Because it is not processing entities and components, it is not a ComponentSystem - just a
// regular System. It doesn't need to process any entities, it just needs to be able to update
// itself. Because it needs to listen for keyboard input, it also inherits from the keyboard
// listener interface.
class PlayerInputSystem : public fsn::System, public fsn::IKeyboardListener
{
    public:
        PlayerInputSystem(fsn::EventManager& eventMgr);

        // Update. All we need to do here is increment mTick.
        void update(const float dt);

        // IKeyboardListener callbacks
        bool onKeyPressed(sf::Keyboard::Key key);
        bool onKeyReleased(sf::Keyboard::Key key);

        // Callback for when the local player is spawned
        void onLocalPlayerSpawned(const LocalPlayerSpawnedEvent& event);

    private:
        // Store the event manager so we can fire out input events
        fsn::EventManager& mEventManager;

        // Store the local player's EntityRef so we can correspond CharacterInputEvents to the
        // local player
        fsn::EntityRef mLocalPlayer;

        // Current state of the player's input, which we use to construct CharacterInputEvents
        CharacterInput mInput;

        // The current game tick. We need to track the game ticks so that we can calculate the
        // durations of each input.
        std::size_t mTick;

        // The tick that the last input occurred at. Used for calculating durations of each input.
        std::size_t mLastInputTick;
};

#endif // PLAYERINPUTSYSTEM_H
