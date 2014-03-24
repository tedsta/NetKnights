#ifndef STAMINA_H
#define STAMINA_H

#include <Fission/Core/Component.h>

class Stamina : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Stamina(std::size_t stamina = 100) : stamina(stamina)
        {
        }

        void serialize(fsn::Packet& packet)
        {
            packet << *this;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> *this;
        }

        std::size_t stamina;
};

#endif // STAMINA_H
