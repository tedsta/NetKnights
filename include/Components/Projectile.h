#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <Fission/Core/Component.h>

class Projectile : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Projectile() {}

        int damage;
};

#endif // PROJECTILE_H
