#ifndef HITPOINTS_H
#define HITPOINTS_H

#include <Fission/Core/Component.h>

class HitPoints : public fsn::Component
{
    FISSION_COMPONENT

    public:
        HitPoints() : HP(100) {}

        void serialize(fsn::Packet& packet)
        {
            packet << *this;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> *this;
        }

        int HP;
};

#endif // HITPOINTS_H
