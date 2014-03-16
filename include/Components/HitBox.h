#ifndef HITBOX_H
#define HITBOX_H

#include <SFML/Graphics/Rect.hpp>

#include <Fission/Core/Component.h>

class HitBox : public fsn::Component
{
    FISSION_COMPONENT

    public:
        HitBox() {}

        sf::FloatRect rect;
};

#endif // HITBOX_H
