#ifndef HITPOINTS_H
#define HITPOINTS_H

#include <Fission/Core/Component.h>

class HitPoints : public fsn::Component
{
    FISSION_COMPONENT

    public:
        HitPoints() {}

        int HP;
};

#endif // HITPOINTS_H
