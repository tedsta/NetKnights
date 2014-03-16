#ifndef HITBOX_H
#define HITBOX_H

#include <SFML/Graphics/Rect.hpp>

#include <Fission/Core/Component.h>

class HitBox : public fsn::Component
{
    FISSION_COMPONENT

    public:
        HitBox() : rect(0.f, 0.f, 32.f, 32.f) {}

        void serialize(fsn::Packet& packet)
        {
            packet << *this;
        }

        void deserialize(fsn::Packet& packet)
        {
            packet >> *this;
        }

        sf::FloatRect rect;
};

#endif // HITBOX_H
