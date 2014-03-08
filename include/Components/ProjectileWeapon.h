#ifndef PROJECTILEWEAPON_H
#define PROJECTILEWEAPON_H

#include <SFML/System/Vector2.hpp>

#include <Fission/Core/Component.h>

class ProjectileWeapon : public fsn::Component
{
    FISSION_COMPONENT

    public:
        ProjectileWeapon() : coolDown(0.1f), coolDownLeft(0.f), direction(1.f, 0.f), fireVelocity(400.f), attemptFire(false)
        {
        }

        float coolDown;
        float coolDownLeft;
        sf::Vector2f direction;
        float fireVelocity;
        bool attemptFire;
};

#endif // PROJECTILEWEAPON_H
