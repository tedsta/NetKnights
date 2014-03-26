#ifndef SHIELD_H
#define SHIELD_H

#include <Fission/Core/Component.h>

class Shield : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Shield() : guard(false) {}

        bool guard;
};

#endif // SHIELD_H
