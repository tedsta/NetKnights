#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <Fission/Core/Component.h>

class MeleeWeapon : public fsn::Component
{
    FISSION_COMPONENT

    public:
        MeleeWeapon() : hitBox(0.f, 0.f, 32.f, 32.f), direction(1, 0), damage(25), hitDelay(0.1), hitDelayLeft(hitDelay), attemptAttack(false) {}

        void serialize(fsn::Packet& packet)
        {
            packet << *this;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> *this;
        }

        sf::FloatRect hitBox;
        sf::Vector2f direction;
        int damage;
        float hitDelay;
        float hitDelayLeft;
        bool attemptAttack;
        bool attacking;
};

#endif // MELEEWEAPON_H
