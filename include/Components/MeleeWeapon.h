#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <Fission/Core/Component.h>

class MeleeWeapon : public fsn::Component
{
    FISSION_COMPONENT

    public:
        MeleeWeapon() {}

        sf::FloatRect hitBox;
        sf::Vector2f direction;
        int damage;
        float hitDelay;
        float hitDelayLeft;
        bool attemptAttack;
};

#endif // MELEEWEAPON_H
