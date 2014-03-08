#include "Systems/PlayerInputSystem.h"

#include "Components/ProjectileWeapon.h"

PlayerInputSystem::PlayerInputSystem(fsn::EventManager& eventMgr) : mEventManager(eventMgr), mTick(0), mLastInputTick(0)
{
    //ctor
}

void PlayerInputSystem::update(const float dt)
{
    mTick++;
}

bool PlayerInputSystem::onKeyPressed(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::Key::W:
        {
            mInput.sequence++;
            mInput.ticksSinceLastInput = mTick-mLastInputTick;
            mLastInputTick = mTick;

            mInput.up = true;
            mEventManager.fireEvent(CharacterInputEvent(mLocalPlayer, mInput));
            break;
        }

        case sf::Keyboard::Key::S:
        {
            mInput.sequence++;
            mInput.ticksSinceLastInput = mTick-mLastInputTick;
            mLastInputTick = mTick;

            mInput.down = true;
            mEventManager.fireEvent(CharacterInputEvent(mLocalPlayer, mInput));
            break;
        }

        case sf::Keyboard::Key::A:
        {
            mInput.sequence++;
            mInput.ticksSinceLastInput = mTick-mLastInputTick;
            mLastInputTick = mTick;

            mInput.left = true;
            mEventManager.fireEvent(CharacterInputEvent(mLocalPlayer, mInput));
            break;
        }

        case sf::Keyboard::Key::D:
        {
            mInput.sequence++;
            mInput.ticksSinceLastInput = mTick-mLastInputTick;
            mLastInputTick = mTick;

            mInput.right = true;
            mEventManager.fireEvent(CharacterInputEvent(mLocalPlayer, mInput));
            break;
        }

        case sf::Keyboard::Key::Space:
        {
            /*mInput.sequence++;
            mInput.ticksSinceLastInput = mTick-mLastInputTick;
            mLastInputTick = mTick;

            mInput.right = true;
            mEventManager.fireEvent(CharacterInputEvent(mLocalPlayer, mInput));*/

            mLocalPlayer.getComponent<ProjectileWeapon>().attemptFire = true;

            break;
        }

        default:
            break;
    }

    return false;
}

bool PlayerInputSystem::onKeyReleased(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::Key::W:
        {
            mInput.sequence++;
            mInput.ticksSinceLastInput = mTick-mLastInputTick;
            mLastInputTick = mTick;

            mInput.up = false;
            mEventManager.fireEvent(CharacterInputEvent(mLocalPlayer, mInput));
            break;
        }

        case sf::Keyboard::Key::S:
        {
            mInput.sequence++;
            mInput.ticksSinceLastInput = mTick-mLastInputTick;
            mLastInputTick = mTick;

            mInput.down = false;
            mEventManager.fireEvent(CharacterInputEvent(mLocalPlayer, mInput));
            break;
        }

        case sf::Keyboard::Key::A:
        {
            mInput.sequence++;
            mInput.ticksSinceLastInput = mTick-mLastInputTick;
            mLastInputTick = mTick;

            mInput.left = false;
            mEventManager.fireEvent(CharacterInputEvent(mLocalPlayer, mInput));
            break;
        }

        case sf::Keyboard::Key::D:
        {
            mInput.sequence++;
            mInput.ticksSinceLastInput = mTick-mLastInputTick;
            mLastInputTick = mTick;

            mInput.right = false;
            mEventManager.fireEvent(CharacterInputEvent(mLocalPlayer, mInput));
            break;
        }

        case sf::Keyboard::Key::Space:
        {
            /*mInput.sequence++;
            mInput.ticksSinceLastInput = mTick-mLastInputTick;
            mLastInputTick = mTick;

            mInput.right = true;
            mEventManager.fireEvent(CharacterInputEvent(mLocalPlayer, mInput));*/

            mLocalPlayer.getComponent<ProjectileWeapon>().attemptFire = false;

            break;
        }

        default:
            break;
    }

    return false;
}

void PlayerInputSystem::onLocalPlayerSpawned(const LocalPlayerSpawnedEvent& event)
{
    mLocalPlayer = event.playerEntity;
}
