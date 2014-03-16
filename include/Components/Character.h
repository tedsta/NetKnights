#ifndef CHARACTER_H
#define CHARACTER_H

#include <Fission/Core/Component.h>

class Character : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Character(std::size_t characterID = 0) {}

        void serialize(fsn::Packet& packet)
        {
            packet << *this;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> *this;
        }

        std::size_t characterID; // Character ID
};

#endif // CHARACTER_H
